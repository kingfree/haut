/* include readline */
#include "unp.h"

/* 超级慢版本 -- 仅用于示例 */
ssize_t readline(int fd, void* vptr, size_t maxlen)
{
    ssize_t n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++) {
    again:
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n') break; /* 检测到新行，类似 fgets() */
        } else if (rc == 0) {
            *ptr = 0;
            return (n - 1); /* EOF, 读入了 n - 1 字节 */
        } else {
            if (errno == EINTR) goto again;
            return (-1); /* 错误，errno 由 read() 设置 */
        }
    }

    *ptr = 0; /* 以空结束，类似 fgets() */
    return (n);
}
/* end readline */

ssize_t Readline(int fd, void* ptr, size_t maxlen)
{
    ssize_t n;

    if ((n = readline(fd, ptr, maxlen)) < 0) err_sys("readline error");
    return (n);
}
