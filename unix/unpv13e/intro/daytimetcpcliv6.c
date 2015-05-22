#include "unp.h"

int main(int argc, char** argv)
{
    int sockfd, n;
    struct sockaddr_in6 servaddr;
    char recvline[MAXLINE + 1];

    if (argc != 2)
        err_quit("用法: daytimetcpcliv6 <IP地址>");

    if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
        err_sys("socket 错误");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(13); /* 时间服务器 */
    if (inet_pton(AF_INET6, argv[1], &servaddr.sin6_addr) <= 0)
        err_quit("inet_pton 错误 %s", argv[1]);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0)
        err_sys("connect 错误");

    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0; /* 以空结束 */
        if (fputs(recvline, stdout) == EOF)
            err_sys("fputs 错误");
    }
    if (n < 0)
        err_sys("read 错误");

    exit(0);
}
