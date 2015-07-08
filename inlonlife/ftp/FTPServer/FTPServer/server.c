#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void p(const char* fmt, ...)
{
#ifdef DEBUG
    va_list p;
    char buf[128];
    
    va_start(p, fmt);
    
    vsnprintf(buf, sizeof(buf), fmt, p);
    fprintf(stderr, "FTPServer: %s\n", buf);
    
    va_end(p);
#endif
}

#define MAXPATH 128

int port = 9876;

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

void ftp_task()
{
    
}

int main(int argc, char* argv[])
{
    int ret;
    int listen_fd;
    struct sockaddr_in server;
    
    openlog("FTPServer", LOG_PID | LOG_NDELAY, LOG_FTP);
    
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        syslog(LOG_ERR, "创建套接字失败: %m");
        exit(-1);
    }
    
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    ret = bind(listen_fd, (struct sockaddr*)&server, sizeof(server));
    if (ret < 0) {
        syslog(LOG_ERR, "绑定套接字失败: %m");
        exit(-1);
    }
    
    ret = listen(listen_fd, 5); // 传统为 5
    if (ret < 0) {
        syslog(LOG_ERR, "监听套接字失败: %m");
        exit(-1);
    }
    syslog(LOG_INFO, "服务器启动在端口: %d", port);
    
    for (;;) {
        struct sockaddr_in client;
        socklen_t len;
        int connect_fd = accept(listen_fd, (struct sockaddr*)&client, &len);
        if (connect_fd < 0) {
            syslog(LOG_ERR, "接收连接失败: %m");
            exit(-1);
        }
        char buff[MAXPATH];
        syslog(LOG_INFO, "客户端请求 %s:%d", inet_ntop(AF_INET, &client.sin_addr, buff, sizeof(buff)), ntohs(client.sin_port));
        pid_t pid = fork();
        if (pid < 0) {
            syslog(LOG_ERR, "服务器进程创建失败: %m");
            exit(-1);
        } else if (pid == 0) {
            close(listen_fd);
            ftp_task();
            close(connect_fd);
            exit(0);
        }
        close(connect_fd);
    }
        
    return 0;
}
