#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

/* 如果你的操作系统已经定义了该函数原型
   就删掉这一行 */
int inet_aton(const char*, struct in_addr*);

/* include inet_pton */
int inet_pton(int family, const char* strptr, void* addrptr)
{
    if (family == AF_INET) {
        struct in_addr in_val;

        if (inet_aton(strptr, &in_val)) {
            memcpy(addrptr, &in_val, sizeof(struct in_addr));
            return (1);
        }
        return (0);
    }
    errno = EAFNOSUPPORT;
    return (-1);
}
/* end inet_pton */
