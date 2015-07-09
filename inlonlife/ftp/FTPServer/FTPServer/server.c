#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
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
    vsprintf(buf, fmt, p);
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
    vsprintf(buf, fmt, p);
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
    vsprintf(buf, fmt, p);
    fprintf(stderr, "错误: %s\n", buf);
    va_end(p);
    
    va_start(p, fmt);
    syslog(LOG_ERR, fmt, p);
    va_end(p);
}

#define MAXPATH 128

int port = 21;
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
    int epsvall;
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
    if (strcmp(cmd, "user") == 0) {
        douser(fs, arg);
    } else if (strcmp(cmd, "pass") == 0) {
        dopass(fs, arg);
    } else if (strcmp(cmd, "quit") == 0) {
        addreply(fs, 221, "再见");
        return 0;
    } else if (strcmp(cmd, "noop") == 0) {
        addreply(fs, 200, "冒泡");
    } else if (strcmp(cmd, "syst") == 0) {
        struct utsname unameData;
        if (uname(&unameData) == 0) {
            addreply(fs, 215, "%s", unameData.sysname);
        }
    } else {
        addreply(fs, 500, "未知命令");
    }
    return 1;
}

/* FTP 服务器进程 */
void ftp_task(int fd)
{
    p("新服务器进程！");
    
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
        p("正在执行命令...");
        if (docmd(&state) <= 0) {
            break;
        }
        p("执行命令");
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
    
    p("正在创建套接字...");
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        pe("创建套接字失败: %m");
        exit(-1);
    }
    p("创建套接字");

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    p("正在绑定套接字...");
    ret = bind(listen_fd, (struct sockaddr*)&server, sizeof(server));
    if (ret < 0) {
        pe("绑定套接字失败: %m");
        exit(-1);
    }
    p("绑定套接字");
    
    p("正在监听套接字...");
    ret = listen(listen_fd, 5); // 传统为 5
    if (ret < 0) {
        pe("监听套接字失败: %m");
        exit(-1);
    }
    pp("服务器启动在 %s:%d", inet_ntop(AF_INET, &server.sin_addr, buff, sizeof(buff)), ntohs(server.sin_port));
    
    for (;;) {
        struct sockaddr_in client;
        socklen_t len;
        p("正在接收连接...");
        int connect_fd = accept(listen_fd, (struct sockaddr*)&client, &len);
        if (connect_fd < 0) {
            pe("接收连接失败: %m");
            exit(-1);
        }
        p("接收连接");

        pp("客户端请求 %s:%d", inet_ntop(AF_INET, &client.sin_addr, buff, sizeof(buff)), ntohs(client.sin_port));
        pid_t pid = fork();
        if (pid < 0) {
            pe("服务器进程创建失败: %m");
            exit(-1);
        } else if (pid == 0) {
            close(listen_fd);
            p("关闭监听描述符");
            ftp_task(connect_fd);
            close(connect_fd);
            p("关闭连接描述符");
            exit(0);
        }
        close(connect_fd);
    }
    
    return 0;
}
