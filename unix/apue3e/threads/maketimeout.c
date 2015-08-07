#include <sys/time.h>
#include <stdlib.h>

void maketimeout(struct timespec *tsp, long minutes)
{
    struct timeval now;

    /* 获取当前时间 */
    gettimeofday(&now, NULL);
    tsp->tv_sec = now.tv_sec;
    tsp->tv_nsec = now.tv_usec * 1000; /* 秒到纳秒 */
    /* 添加偏移量以获取超时值 */
    tsp->tv_sec += minutes * 60;
}
