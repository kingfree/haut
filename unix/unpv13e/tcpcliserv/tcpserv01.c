#include "unp.h"

int main(int argc, char** argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = Accept(listenfd, (SA*)&cliaddr, &clilen);

        if ((childpid = Fork()) == 0) { /* 子进程 */
            Close(listenfd); /* 关闭监听的套接字 */
            str_echo(connfd); /* 处理请求 */
            exit(0);
        }
        Close(connfd); /* 父进程关闭已连接的套接字 */
    }
}
