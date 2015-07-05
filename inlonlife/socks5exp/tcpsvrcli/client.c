#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define SERV_PORT 9877
#define LISTENQ 1024
#define MAX_LEN 1024

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

    struct sockaddr_in server_addr, client_addr;
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

    while (fgets(buf, MAX_LEN, stdin) != NULL) {
        int m = 0, n = strlen(buf), r = 0;
        while (r < n) {
            m = write(socket_fd, buf + r, n - r);
            if (m == -1) {
                if (errno != EINTR) {
                    perror("write()");
                    exit(-1);
                }
            }
            else if (m == 0) {
                break;
            }
            r += m;
        }
        fputs(buf, stdout);
    }

    return 0;
}
