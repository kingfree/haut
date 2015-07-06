#include "network.h"

static int read_cnt;
static char* read_ptr;
static char read_buf[MAX_LEN];

static ssize_t my_read(int fd, char* p)
{
    if (read_cnt <= 0) {
    again:
        if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
            if (errno == EINTR)
                goto again;
            return -1;
        }
        else if (read_cnt == 0)
            return 0;
        read_ptr = read_buf;
    }

    read_cnt--;
    *p = *read_ptr++;
    return 1;
}

ssize_t readline(int fd, void* buf, size_t maxlen)
{
    char c;
    char* p = buf;
    ssize_t n;
    for (n = 1; n < maxlen; n++) {
        ssize_t m = my_read(fd, &c);
        if (m == 1) {
            *p++ = c;
            if (c == '\n') {
                break;
            }
        } else if (m == 0) {
            *p = 0;
            return n - 1;
        } else {
            return -1;
        }
    }
    *p = 0;
    return n;
}

ssize_t readlinebuf(void** vptrptr)
{
    if (read_cnt)
        *vptrptr = read_ptr;
    return read_cnt;
}
