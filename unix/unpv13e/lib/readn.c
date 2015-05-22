/* include readn */
#include "unp.h"

ssize_t /* 从描述符中读入 "n" 个字节。 */
    readn(int fd, void* vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char* ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)
                nread = 0; /* 然后再次调用 read() */
            else
                return (-1);
        }
        else if (nread == 0)
            break; /* EOF */

        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft); /* 返回 >= 0 */
}
/* end readn */

ssize_t
Readn(int fd, void* ptr, size_t nbytes)
{
    ssize_t n;

    if ((n = readn(fd, ptr, nbytes)) < 0)
        err_sys("readn 错误");
    return (n);
}
