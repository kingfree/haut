#include "network.h"

int main(int argc, char const* argv[])
{
    if (argc < 2) {
        printf("用法: %s <IP地址>\n", argv[0]);
        exit(0);
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket()");
        exit(-1);
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) == 0) {
        perror("inet_pton()");
        exit(-1);
    }

    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect()");
        exit(-1);
    }

    char buf[MAX_LEN];
    char recv[MAX_LEN];

    while (fgets(buf, MAX_LEN, stdin) != NULL) {
        int m = writen(socket_fd, buf, strlen(buf));
        if (m < 0) {
            perror("writen()");
            exit(-1);
        }
        int n = readline(socket_fd, recv, MAX_LEN);
        if (n < 0) {
            perror("readline()");
            exit(-1);
        } else if (n == 0) {
            printf("服务器已关闭\n");
            exit(0);
        }
        fputs(recv, stdout);
    }

    return 0;
}
