#include "network.h"

ssize_t readn(int fd, void* buf, size_t n)
{
    char* p = buf;
    size_t r = n;
    while (r > 0) {
        ssize_t m = read(fd, p, r);
        if (m < 0) {
            if (errno == EINTR) {
                m = 0;
            } else {
                return -1;
            }
        } else if (m == 0) {
            break;
        }
        r -= m;
        p += m;
    }
    return n - r;
}
