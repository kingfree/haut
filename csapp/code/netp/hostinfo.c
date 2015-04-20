#include <stdio.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#endif

int main(int argc, char *argv[])
{
    char **pp;
    struct in_addr addr;
    struct hostent *hostp;

    if (argc != 2) {
        fprintf(stderr, "用法: %s <域名或IP>\n", argv[0]);
        exit(0);
    }

    if (inet_aton(argv[1], &addr) != 0)
        hostp = gethostbyaddr((const char *) &addr, sizeof(addr), AF_INET);
    else
        hostp = gethostbyname(argv[1]);

    printf("主机名: %s\n", hostp->h_name);
    
    for (pp = hostp->h_aliases; *pp; pp++)
        printf("别名: %s\n", *pp);

    for (pp = hostp->h_addr_list; *pp; pp++) {
        addr.s_addr = ((struct in_addr *) *pp)->s_addr;
        printf("地址: %s\n", inet_ntoa(addr));
    }

    exit(0);
}
