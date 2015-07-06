#ifndef _NETWORK_H_
#define _NETWORK_H_

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

ssize_t readn(int fd, void* buf, size_t n);
ssize_t readline(int fd, void* buf, size_t maxlen);
ssize_t writen(int fd, const void* buf, size_t n);

#endif
