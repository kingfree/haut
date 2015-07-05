#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket()");
        exit(-1);
    }

    struct sockaddr_in server_addr, client_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERV_PORT);

    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind()");
        exit(-1);
    }

    if (listen(listen_fd, LISTENQ) == -1) {
        perror("listen()");
        exit(-1);
    }

    char buf[MAX_LEN];

    for (;;) {
        socklen_t len = sizeof(client_addr);

        int conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len);
        if (conn_fd == -1) {
            if (errno == EINTR) {
                continue;
            }
            else {
                perror("accept()");
                exit(-1);
            }
        }

        int child_pid = fork();
        if (child_pid == 0) {
            close(listen_fd);
            for (;;) {
                int n = 0;
                n = read(conn_fd, buf, MAX_LEN);
                if (n == -1) {
                    if (errno != EINTR) {
                        perror("read()");
                        exit(-1);
                    }
                }
                else if (n == 0) {
                    break;
                }

                int m = 0, r = 0;
                while (r < n) {
                    m = write(conn_fd, buf + r, n - r);
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
            }
            exit(0);
        }
        close(conn_fd);
    }

    return 0;
}
