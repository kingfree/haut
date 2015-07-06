#include "network.h"

ssize_t writen(int fd, const void* buf, size_t n)
{
    const char* p = buf;
    size_t r = n;
    while (r > 0) {
        ssize_t m = write(fd, p, r);
        if (m <= 0) {
            if (m < 0 && errno == EINTR) {
                m = 0;
            } else {
                return -1;
            }
        }
        r -= m;
        p += m;
    }
    return n;
}
