/* include writen */
#include "unp.h"

ssize_t /* 向描述符中写出 "n" 个字节。 */
    writen(int fd, const void* vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char* ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0; /* 然后再次调用 write() */
            else
                return (-1); /* 错误 */
        }

        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n);
}
/* end writen */

void Writen(int fd, void* ptr, size_t nbytes)
{
    if (writen(fd, ptr, nbytes) != nbytes)
        err_sys("writen 错误");
}
