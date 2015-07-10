#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <syslog.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <grp.h>
#include <uuid/uuid.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pwd.h>

/* 打印调试信息 */
void p(const char* fmt, ...)
{
    va_list p;
    char buf[128];
    
    va_start(p, fmt);
    vsnprintf(buf, sizeof(buf), fmt, p);
    fprintf(stderr, "调试: %s\n", buf);
    va_end(p);
    
    va_start(p, fmt);
    syslog(LOG_DEBUG, fmt, p);
    va_end(p);
}

/* 打印正常信息 */
void pp(const char* fmt, ...)
{
    va_list p;
    char buf[128];
    
    va_start(p, fmt);
    vsnprintf(buf, sizeof(buf), fmt, p);
    fprintf(stderr, "信息: %s\n", buf);
    va_end(p);

    va_start(p, fmt);
    syslog(LOG_INFO, fmt, p);
    va_end(p);
}

/* 打印错误信息 */
void pe(const char* fmt, ...)
{
    va_list p;
    char buf[128];
    
    va_start(p, fmt);
    vsnprintf(buf, sizeof(buf), fmt, p);
    fprintf(stderr, "错误: %s\n", buf);
    va_end(p);
    
    va_start(p, fmt);
    syslog(LOG_ERR, fmt, p);
    va_end(p);
}

#define MAXPATH 128

int timedout = 900 * 1000;
char buff[MAXPATH];

struct reply {
    struct reply* next;
    char line[1];
};

struct ftpstate {
    int ctrlsock;
    int datasock;
    int replycode;
    struct reply* firstreply;
    struct reply* lastreply;
    FILE* in;
    FILE* out;
    struct sockaddr_in peer;
    char cmd[MAXPATH + 32];
    char wd[MAXPATH + 1];
    char* renamefrom;
    int uid;
//    int epsvall;
    int loggedin;
    int guest;
    int restartat;
    int debug;
    int idletime;
    int passive;
    int dataport;
    int type;
};

/* 增加一行回复 */
void addreply(struct ftpstate* fs, int code, const char* line, ...)
{
    char buf[MAXPATH + 128];
    
    if (code) {
        fs->replycode = code;
    }
    
    va_list ap;
    va_start(ap, line);
    vsnprintf(buf, MAXPATH + 50, line, ap);
    va_end(ap);
    
    char* s = buf;
    while (*s) {
        char* e = s;
        while (*e && *e != '\n') {
            e++;
        }
        long l = e - s;
        
        struct reply* r = (struct reply*)malloc(sizeof(struct reply) + l);
        if (!r) {
            return;
        }
        memcpy(r->line, s, l);
        r->line[l] = 0;
        r->next = NULL;
        
        if (fs->lastreply) {
            fs->lastreply->next = r;
        } else {
            fs->firstreply = r;
        }
        fs->lastreply = r;
        
        s = e;
        if (*s) {
            s++;
        }
    }
}

/* 执行回复 */
void doreply(struct ftpstate* fs)
{
    struct reply* r = fs->firstreply;
    while (r) {
        struct reply* next = r->next;
        fprintf(fs->out, "%03d %s\r\n", fs->replycode, r->line);
        syslog(LOG_DEBUG, "%03d %s", fs->replycode, r->line);
        free(r);
        r = next;
    }
    
    fs->firstreply = fs->lastreply = NULL;
    fflush(fs->out);
}

/* 报告错误 */
void error(struct ftpstate* fs, int code, char* fmt, ...)
{
    va_list p;
    int err = errno;
    if (err == 0) {
        va_start(p, fmt);
        pe(fmt, p);
        va_end(p);
        va_start(p, fmt);
        addreply(fs, code, fmt, p);
        va_end(p);
    } else {
        char* errmsg = strerror(errno);
        char buf[128];
        va_start(p, fmt);
        vsnprintf(buf, sizeof(buf), fmt, p);
        va_end(p);
        pe("%s: %s", buf, errmsg);
        addreply(fs, code, "%s: %s", buf, errmsg);
    }
}

/* 转换路径 */
int convert(struct ftpstate* fs, char* filename, char* buf)
{
    char buffer[2 * MAXPATH];
    char* p;
    
    if (*filename == '/' || *filename == '\\') {
        strcpy(buffer, filename);
    } else {
        strcpy(buffer, fs->wd);
        strcat(buffer, "/");
        strcat(buffer, filename);
    }
    
    if (realpath(buffer, buf) == NULL) {
        return -1;
    }
    
    for (p = buf; *p; p++) {
        if (*p == '\\') {
            *p = '/';
        }
    }
    
    return 0;
}

/* 登录 */
int login(struct ftpstate* fs, struct passwd* pw)
{
    if (initgroups(pw->pw_name, pw->pw_gid) < 0) {
        return -1;
    }
    if (setgid(pw->pw_gid) < 0) {
        return -1;
    }
    if (setuid(pw->pw_uid) < 0) {
        return -1;
    }
    
    fs->uid = pw->pw_uid;
    strcpy(fs->wd, pw->pw_dir);
    
    return 0;
}

/* 打开数据连接 */
int opendata(struct ftpstate* fs)
{
    struct sockaddr_in sin;
    int sock;
    
    if (fs->datasock < 0) {
        addreply(fs, 425, "无数据连接");
        return -1;
    }
    
    if (fs->passive) { // 被动模式
        fd_set rs;
        struct timeval tv;
        
        FD_ZERO(&rs);
        FD_SET(fs->datasock, &rs);
        tv.tv_sec = fs->idletime;
        tv.tv_usec = 0;
        if (select(fs->datasock + 1, &rs, NULL, NULL, &tv) < 0) { // 等待请求
            addreply(fs, 421, "超时 (已 %d 秒无连接)", fs->idletime);
            return -1;
        }
        
        socklen_t len = sizeof(sin);
        sock = accept(fs->datasock, (struct sockaddr*)&sin, &len);
        if (sock < 0) {
            error(fs, 421, "接受请求失败");
            close(fs->datasock);
            fs->datasock = -1;
            return -1;
        }
        
        if (!fs->guest && sin.sin_addr.s_addr != fs->peer.sin_addr.s_addr) {
            addreply(fs, 425, "连接必须来自 %s", inet_ntoa(fs->peer.sin_addr));
            close(sock);
            close(fs->datasock);
            fs->datasock = -1;
            return -1;
        }
        
        addreply(fs, 150, "接受到来自 %s:%d 的请求", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
    } else { // 主动模式
        sin.sin_addr.s_addr = fs->peer.sin_addr.s_addr;
        sin.sin_port = htons(fs->dataport);
        sin.sin_family = AF_INET;
        
        if (connect(fs->datasock, (struct sockaddr*)&sin, sizeof(sin)) < 0) { // 主动连接
            addreply(fs, 425, "无法打开数据连接到 %s:%d: %s", inet_ntoa(sin.sin_addr), fs->dataport, strerror(errno));
            close(fs->datasock);
            fs->datasock = -1;
            return -1;
        }
        
        sock = fs->datasock;
        fs->datasock = -1;
        addreply(fs, 150, "连接到 %s:%d", inet_ntoa(sin.sin_addr), fs->dataport);
    }
    
    return sock;
}

/* 验证用户 */
void douser(struct ftpstate* fs, char* username)
{
    struct passwd* pw;
    
    if (fs->loggedin) {
        if (username) {
            if (!fs->guest) {
                addreply(fs, 530, "您已登录");
            } else {
                addreply(fs, 230, "您已匿名登录");
            }
        }
        return;
    }

    if (username && strcmp(username, "ftp") != 0 && strcmp(username, "anonymous") != 0) {
        pw = getpwnam(username);
        if (pw == NULL) {
            addreply(fs, 331, "未知用户 %s ", username);
        } else {
            fs->uid = pw->pw_uid;
            addreply(fs, 331, "用户 %s 需要密码", pw->pw_name);
        }
        
        fs->loggedin = 0;
    } else {
        pw = getpwnam("ftp");
        if (!pw) {
            addreply(fs, 530, "不允许匿名用户");
        } else {
            if (login(fs, pw) < 0) {
                addreply(fs, 530, "匿名用户无法登录");
            } else {
                addreply(fs, 230, "匿名用户登录成功");
                fs->loggedin = fs->guest = 1;
                pp("匿名用户登录");
            }
        }
    }
}

/* 验证密码 */
void dopass(struct ftpstate* fs, char* password)
{
    struct passwd* pw;
    
    if (fs->uid < 0) {
        addreply(fs, 332, "需要用户");
    } else if ((pw = getpwuid(fs->uid)) == NULL) {
        addreply(fs, 331, "未知用户");
    } else if (strcmp(pw->pw_passwd, crypt(password, pw->pw_passwd)) == 0) {
        if (login(fs, pw) < 0) {
            addreply(fs, 530, "用户无法登录");
        } else {
            fs->loggedin = 1;
            addreply(fs, 230, "登陆成功。当前目录 %s", fs->wd);
            pp("用户 %s 已登录", pw->pw_name);
        }
    } else {
        // p("%s %s %s", password, pw->pw_passwd, crypt(password, pw->pw_passwd));
        // 在 OS X 中这招行不通
        addreply(fs, 530, "密码有误");
    }
}

/* 切换工作目录 */
void docwd(struct ftpstate* fs, char* dir)
{
    char newwd[MAXPATH];
    struct stat st;
    
    if (convert(fs, dir, newwd) < 0 || stat(newwd, &st) < 0) {
        addreply(fs, 530, "无法切换目录到 %s: %s", dir, strerror(errno));
        return;
    }
    
    if (!S_ISDIR(st.st_mode)) {
        addreply(fs, 530, "无此目录");
        return;
    }
    
    strcpy(fs->wd, newwd);
    addreply(fs, 250, "切换目录到 %s", fs->wd);
}

/* 列出目录文件 */
void dolist(struct ftpstate* fs, char* args)
{
    char dirname[MAXPATH];
    int show_list = 0;
    int show_all = 0;
    
    while (isspace(*args)) {
        args++;
    }
    
    while (*args == '-') {
        while (isalnum(*++args)) {
            switch (*args) {
                case 'l':
                    show_list = 1;
                    break;
                case 'a':
                    show_all = 1;
                    break;
                default:
                    break;
            }
        }
        
        while (isspace(*args)) {
            args++;
        }
    }
    
    if (convert(fs, args, dirname) < 0) {
        error(fs, 550, args);
        return;
    }
    
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        error(fs, 550, dirname);
        return;
    }
    
    int sock = opendata(fs);
    if (sock < 0) {
        closedir(dir);
        return;
    }
    doreply(fs);
    
    struct dirent* d;
    int total = 0;
    while ((d = readdir(dir)) != NULL) {
        char buf[MAXPATH + 128];
        
        if (!show_all && d->d_name[0] == '.') { // 不显示隐藏文件
            continue;
        }
        if (show_list) {
            struct stat st;
            char filename[MAXPATH * 2 + 2];
            strcpy(filename, dirname);
            strcat(filename, "/");
            strcat(filename, d->d_name);
            
            if (stat(filename, &st) < 0) {
                continue;
            }
            
            char perms[11];
            strcpy(perms, "----------");
            switch (st.st_mode & S_IFWHT) {
            case S_IFREG:
                perms[0] = '-';
                break;
            case S_IFLNK:
                perms[0] = 'l';
                break;
            case S_IFDIR:
                perms[0] = 'd';
                break;
            case S_IFBLK:
                perms[0] = 'b';
                break;
            case S_IFCHR:
                perms[0] = 'c';
                break;
            default:
                break;
            }
            if (st.st_mode & S_IRUSR) perms[1] = 'r';
            if (st.st_mode & S_IWUSR) perms[2] = 'w';
            if (st.st_mode & S_IXUSR) perms[3] = 'x';
            if (st.st_mode & S_IRGRP) perms[4] = 'r';
            if (st.st_mode & S_IWGRP) perms[5] = 'w';
            if (st.st_mode & S_IXGRP) perms[6] = 'x';
            if (st.st_mode & S_IROTH) perms[7] = 'r';
            if (st.st_mode & S_IWOTH) perms[8] = 'w';
            if (st.st_mode & S_IXOTH) perms[9] = 'x';
            
            struct tm* tm;
            if ((tm = localtime(&st.st_mtimespec.tv_sec)) == NULL) {
                continue;
            }
            char tms[20];
            strftime(tms, 20, "%G/%m/%d %T", tm);
            
            struct passwd* pwd = getpwuid(st.st_uid);
            struct group* grp = getgrgid(st.st_gid);
            if (pwd == NULL || grp == NULL) {
                continue;
            }

            sprintf(buf, "%10s %3d\t%s\t%s %7lld %s %s\r\n",
                    perms, st.st_nlink, pwd->pw_name, grp->gr_name, st.st_size, tms, d->d_name);
        } else {
            sprintf(buf, "%s\r\n", d->d_name);
        }
        send(sock, buf, strlen(buf), 0);
        total++;
    }
    addreply(fs, 226, "总计 %d", total);
    
    close(sock);
    closedir(dir);
}

/* 取回文件 */
void doretr(struct ftpstate* fs, char* name)
{
    char filename[MAXPATH];
    struct stat st;
    char buf[4096];
    
    if (convert(fs, name, filename) < 0) {
        error(fs, 550, name);
        return;
    }
    
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        error(fs, 550, "无法打开 %s", name);
        return;
    }
    
    if (fstat(fd, &st)) {
        close(fd);
        error(fs, 451, "无法获取文件大小");
        return;
    }
    
    if (fs->restartat && fs->restartat > st.st_size) {
        addreply(fs, 451, "文件偏移位置 %d 大于文件大小 %d\n重设偏移为 0", fs->restartat, st.st_size);
        return;
    }
    
    if (!S_ISREG(st.st_mode)) {
        close(fd);
        addreply(fs, 450, "非常规文件");
        return;
    }
    
    int sock = opendata(fs);
    if (sock < 0) {
        close(fd);
        return;
    }
    
    if (fs->restartat == st.st_size) {
        close(fd);
        close(sock);
        addreply(fs, 226, "无可下载的数据\n重设偏移为 0");
        return;
    }

    doreply(fs);
    
    clock_t started = clock();
    int ofs = fs->restartat;
    if (ofs != 0) {
        lseek(fd, ofs, SEEK_SET);
    }
    
    while (ofs < st.st_size) {
        long n = st.st_size - ofs;
        if (n > sizeof(buf))
            n = sizeof(buf);
        
        n = read(fd, buf, n);
        if (n <= 0) {
            if (n == 0) {
                addreply(fs, 451, "意外的文件结束符");
            } else {
                error(fs, 451, "读取文件出错");
            }
            
            close(fd);
            close(sock);
            return;
        }
        
        if (send(sock, buf, n, 0) < 0) {
            addreply(fs, 426, "传送中止");
            close(fd);
            close(sock);
            return;
        }
        
        ofs += n;
    }
    
    clock_t ended = clock();
    
    double t = (ended - started) / 1000.0;
    addreply(fs, 226, "文件成功写出");
    
    double speed = 0.0;
    if (t != 0.0 && st.st_size - fs->restartat > 0) {
        speed = (st.st_size - fs->restartat) / t;
    }
    
    pp("用时 %.3f 秒 (服务器统计), 速度 %.2f %sb/s", t, speed > 524288 ? speed / 1048576 : speed / 1024, speed > 524288 ? "M" : "K");
    
    close(fd);
    close(sock);
    
    if (fs->restartat != 0) {
        fs->restartat = 0;
        addreply(fs, 0, "重设偏移为 0");
    }
}

/* 回应端口 */
void doport(struct ftpstate* fs, unsigned int ip, unsigned int port)
{
    struct sockaddr_in sin;
    
    if (fs->datasock != -1) {
        close(fs->datasock);
        fs->datasock = -1;
    }
    
    fs->datasock = socket(AF_INET, SOCK_STREAM, 0);
    if (fs->datasock < 0) {
        error(fs, 425, "无法创建套接字");
        return;
    }
    
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(20); // 数据连接
    if (bind(fs->datasock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        error(fs, 220, "绑定套接字失败");
        close(fs->datasock);
        fs->datasock = -1;
        return;
    }
    
    if (fs->debug) {
        addreply(fs, 0, "数据连接端在 %s:%d", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));
    }
    
    fs->dataport = port;
    
    if (htonl(ip) != fs->peer.sin_addr.s_addr) {
        addreply(fs, 425, "不会打开到 %d.%d.%d.%d 的连接 (仅限 %s)",
                 (ip >> 24) & 255, (ip >> 16) & 255, (ip >> 8) & 255, ip & 255,
                 inet_ntoa(fs->peer.sin_addr));
        close(fs->datasock);
        fs->datasock = -1;
        return;
    }
    
    fs->passive = 0;
    
    addreply(fs, 200, "PORT 命令完成");
}

/* 被动模式 */
void dopasv(struct ftpstate* fs)
{
    struct sockaddr_in sin;
    unsigned int len;
    
    if (fs->datasock != -1) {
        close(fs->datasock);
        fs->datasock = -1;
    }
    
    len = sizeof(sin);
    if (getsockname(fs->ctrlsock, (struct sockaddr*)&sin, &len) < 0) {
        error(fs, 425, "无法获取套接字名");
        return;
    }
    
    fs->datasock = socket(AF_INET, SOCK_STREAM, 0);
    if (fs->datasock < 0) {
        error(fs, 425, "无法打开被动连接");
        return;
    }
    
    sin.sin_port = 0;
    if (bind(fs->datasock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        error(fs, 425, "无法绑定套接字");
        return;
    }
    
    len = sizeof(sin);
    if (getsockname(fs->datasock, (struct sockaddr*)&sin, &len) < 0) {
        error(fs, 425, "无法获取套接字名");
        return;
    }
    
    listen(fs->datasock, 1);
    
    unsigned int a = ntohl(sin.sin_addr.s_addr);
    unsigned int p = ntohs(sin.sin_port);
    addreply(fs, 227, "启用被动模式 (%d,%d,%d,%d,%d,%d)", (a >> 24) & 255, (a >> 16) & 255, (a >> 8) & 255, a & 255, (p >> 8) & 255, p & 255);
    
    fs->passive = 1;
}

/* 执行命令 */
int docmd(struct ftpstate* fs)
{
    char* arg;
    char* cmd;
    unsigned long cmdsize;
    int n = 0;
    
    if (!fgets(fs->cmd, sizeof(fs->cmd), fs->in)) {
        if (errno == ETIMEDOUT) {
            addreply(fs, 421, "超时 (%d 秒)", fs->idletime / 1000);
        }
        return -1;
    }
    cmd = fs->cmd;
    cmdsize = strlen(cmd);
    
    if (fs->debug) {
        addreply(fs, 0, "%s", cmd);
    }
    
    n = 0;
    while (isalpha(cmd[n]) && n < cmdsize) {
        cmd[n] = tolower(cmd[n]);
        n++;
    }
    
    if (!n) {
        addreply(fs, 221, "再见");
        return 0;
    }
    
    while (isspace(cmd[n]) && n < cmdsize) {
        cmd[n++] = '\0';
    }
    arg = cmd + n;
    
    while (cmd[n] && n < cmdsize) {
        n++;
    }
    n--;
    
    while (isspace(cmd[n])) {
        cmd[n--] = '\0';
    }
    
    pp("命令 [%s %s]", cmd, arg);
    if (strcmp(cmd, "user") == 0) {        // USER NAME
        douser(fs, arg);
    } else if (strcmp(cmd, "pass") == 0) { // PASSWORD
        dopass(fs, arg);
    } else if (strcmp(cmd, "quit") == 0) { // LOGOUT
        addreply(fs, 221, "再见");
        return 0;
    } else if (strcmp(cmd, "noop") == 0) { // NOOP
        addreply(fs, 200, "冒泡");
    } else if (strcmp(cmd, "syst") == 0) { // SYSTEM
        struct utsname unameData;
        if (uname(&unameData) == 0) {
            addreply(fs, 215, "%s", unameData.sysname);
        }
    } else if (strcmp(cmd, "feat") == 0) { // Feature
        addreply(fs, 500, "不支持 RFC 959 以外的命令");
    } else if (strcmp(cmd, "port") == 0) { // DATA PORT
        unsigned int a1, a2, a3, a4, p1, p2;
        if (sscanf(arg, "%u,%u,%u,%u,%u,%u", &a1, &a2, &a3, &a4, &p1, &p2) == 6
            && a1 < 256 && a2 < 256 && a3 < 256 && a4 < 256 && p1 < 256 && p2 < 256) {
            doport(fs, (a1 << 24) + (a2 << 16) + (a3 << 8) + a4, ((p1 << 8) + p2));
        } else {
            addreply(fs, 501, "语法错误");
        }
    } else if (strcmp(cmd, "pasv") == 0) { // PASSIVE
        dopasv(fs);
    } else if (strcmp(cmd, "pwd") == 0) {
        if (fs->loggedin) {
            addreply(fs, 257, "\"%s\"", fs->wd);
        } else {
            addreply(fs, 550, "未登录");
        }
    } else {
        goto login_logic;
    }
    return 1;
    
login_logic:
    douser(fs, NULL);
    
    if (strcmp(cmd, "cwd") == 0) {         // CHANGE WORKING DIRECTORY
        docwd(fs, arg);
    } else if (strcmp(cmd, "cdup") == 0) { // CHANGE TO PARENT DIRECTORY
        docwd(fs, "..");
    } else if (strcmp(cmd, "retr") == 0) { // RETRIEVE
        if (arg && *arg) {
            doretr(fs, arg);
        }
        else {
            addreply(fs, 501, "No file name");
        }
    } else if (strcmp(cmd, "list") == 0) { // LIST
        dolist(fs, (arg && *arg) ? arg : "-l");
    }
    else {
        addreply(fs, 500, "未知命令");
    }
    return 1;
}

/* FTP 服务器进程 */
void ftp_task(int fd)
{
//    p("新服务器进程！");
    
    struct ftpstate state;
    
    bzero(&state, sizeof(state));
    state.ctrlsock = fd;
    state.datasock = 1;
    state.uid = -1;
    
    socklen_t len = sizeof(state.peer);
    getpeername(fd, (struct sockaddr*)&state.peer, &len);
    
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timedout, sizeof(timedout));
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timedout, sizeof(timedout));
    
    state.in = fdopen(fd, "r");
    if (!state.in) {
        close(fd);
        pe("打开读描述符失败");
        return;
    }
    
    state.out = fdopen(dup(fd), "w");
    if (!state.out) {
        close(fd);
        pe("打开写描述符失败");
        return;
    }
    
    addreply(&state, 220, "欢迎");
    for (;;) {
        doreply(&state);
//        p("正在执行命令...");
        if (docmd(&state) <= 0) {
            break;
        }
//        p("执行命令");
    }
    doreply(&state);
    
    if (state.renamefrom)
        free(state.renamefrom);
    if (state.in)
        fclose(state.in);
    if (state.out)
        fclose(state.out);
    if (state.datasock != -1)
        close(state.datasock);
    
}

int main(int argc, char* argv[])
{
    int ret;
    int listen_fd;
    struct sockaddr_in server;
    
    openlog("FTPServer", LOG_PID | LOG_NDELAY, LOG_FTP);
    
//    p("正在创建套接字...");
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        pe("创建套接字失败: %m");
        exit(-1);
    }
//    p("创建套接字");

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(21); // 命令连接
//    p("正在绑定套接字...");
    ret = bind(listen_fd, (struct sockaddr*)&server, sizeof(server));
    if (ret < 0) {
        pe("绑定套接字失败: %m");
        exit(-1);
    }
//    p("绑定套接字");
    
//    p("正在监听套接字...");
    ret = listen(listen_fd, 5); // 传统为 5
    if (ret < 0) {
        pe("监听套接字失败: %m");
        exit(-1);
    }
    pp("服务器启动在 %s:%d", inet_ntop(AF_INET, &server.sin_addr, buff, sizeof(buff)), ntohs(server.sin_port));
    
    for (;;) {
        struct sockaddr_in client;
        socklen_t len;
//        p("正在接收连接...");
        int connect_fd = accept(listen_fd, (struct sockaddr*)&client, &len);
        if (connect_fd < 0) {
            pe("接收连接失败: %m");
            exit(-1);
        }
//        p("接收连接");

        pp("客户端请求 %s:%d", inet_ntop(AF_INET, &client.sin_addr, buff, sizeof(buff)), ntohs(client.sin_port));
        pid_t pid = fork();
        if (pid < 0) {
            pe("服务器进程创建失败: %m");
            exit(-1);
        } else if (pid == 0) {
            close(listen_fd);
//            p("关闭监听描述符");
            ftp_task(connect_fd);
            close(connect_fd);
//            p("关闭连接描述符");
            exit(0);
        }
        close(connect_fd);
    }
    
    return 0;
}
