#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "socks5.h"
#define MAX_USER 10
#define BUFF_SIZE 1024
//#define AUTH_CODE 0x02
#define AUTH_CODE 0x00
#define TIME_OUT 6000000

#define PROXY_ADDR 0x7f000001
#define PROXY_PORT 7890
#define PROXY_USER "123456"
#define PROXY_PASS "123456"

void printchar(unsigned char* s, size_t l)
{
    printf("[");
    for (int i = 0; i < l; i++) {
        printf("%02x ", s[i]);
    }
    printf("\b]\n");
}

int main(int argc, char* argv[])
{
    
    ssize_t ret;
    size_t len;
    int proxySocket = socket(AF_INET, SOCK_STREAM, 0);
    printf("建立套接字 %zd\n", proxySocket);

    struct sockaddr_in saddr;
    saddr.sin_addr.s_addr = htonl(PROXY_ADDR);
    saddr.sin_port = htons(PROXY_PORT); //socks5 proxy port

    ret = connect(proxySocket, (struct sockaddr*)&saddr, sizeof(saddr));
    printf("连接到服务器 %zd\n", ret);

    unsigned char buffer[256], *ptrBuff;

    ptrBuff = buffer;
    *ptrBuff++ = 0x05; // VER
    *ptrBuff++ = 0x01; // NMETHODS
    *ptrBuff++ = 0x02; // METHODS[1]
    len = ptrBuff - buffer;

    printchar(buffer, len);
    ret = send(proxySocket, buffer, len, 0);
    if (ret != len) {
        perror("发送协商数据失败");
        exit(-1);
    }
    printf("发送协商数据 %zd\n", ret);
    bzero(buffer, sizeof(buffer));
    ret = recv(proxySocket, buffer, 2, 0);
    printchar(buffer, 2);
    printf("接收协商数据 %zd\n", ret);

    if (buffer[0] != 0x05 || buffer[1] != 0x02) { // VER | METHOD
        printf("不可认证\n");
        exit(-1);
    }
    puts("协商成功");

    ptrBuff = buffer;
    *ptrBuff++ = 0x01; // VER

    len = strlen(PROXY_USER);
    *ptrBuff++ = len; // ULEN
    strncpy((char*)ptrBuff, PROXY_USER, len); // UNAME
    ptrBuff += len;

    len = strlen(PROXY_PASS);
    *ptrBuff++ = len; // PLEN
    strncpy((char*)ptrBuff, PROXY_PASS, len); // PASSWD
    ptrBuff += len;

    len = ptrBuff - buffer;
    printchar(buffer, len);
    ret = send(proxySocket, buffer, len, 0);
    if (ret != len) {
        perror("发送认证数据失败");
        exit(-1);
    }
    printf("发送认证数据 %zd\n", ret);
    bzero(buffer, sizeof(buffer));
    ret = recv(proxySocket, buffer, 2, 0);
    printchar(buffer, 2);
    printf("接收认证数据 %zd\n", ret);

    if (buffer[0] != 0x01 || buffer[1] != 0x00) { // VER | STATUS
        printf("认证失败\n");
        exit(-1);
    }
    puts("认证成功");

    ptrBuff = buffer;
    *ptrBuff++ = 0x05; // VER
    *ptrBuff++ = 0x01; // CMD: CONNECT
    *ptrBuff++ = 0x00; // RSV
    *ptrBuff++ = 0x01; // ATYP: IP V4 address
    

    struct sockaddr_in dest_addr;
    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = (4040);

    memcpy(ptrBuff, &dest_addr.sin_addr.s_addr, sizeof(dest_addr.sin_addr)); // DST.ADDR
    ptrBuff += sizeof(dest_addr.sin_addr);

    *ptrBuff++ = (dest_addr.sin_port >> 8); // DST.PORT
    *ptrBuff++ = (dest_addr.sin_port & 0xFF);

    len = ptrBuff - buffer;
    printchar(buffer, len);
    ret = send(proxySocket, buffer, len, 0);
    if (ret != len) {
        perror("发送请求数据失败");
        exit(-1);
    }
    printf("发送请求数据 %zd\n", ret);
    ret = recv(proxySocket, buffer, 4, 0);
    printchar(buffer, 4);
    printf("接收请求数据 %zd\n", ret);
    
    if (buffer[0] != 0x05 || buffer[1] != 0x00) { // VER | REP
        printf("请求失败\n");
        exit(-1);
    }
    
    puts("请求连接成功");

    return 0;
}
