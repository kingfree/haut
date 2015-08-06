#include "apue.h"
#include <pthread.h>

int main(void)
{
    int err;
    struct timespec tout;
    struct tm *tmp;
    char buf[64];
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&lock);
    printf("mutex 已锁定\n");
    clock_gettime(CLOCK_REALTIME, &tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf, sizeof(buf), "%r", tmp);
    printf("当前时间 %s\n", buf);
    tout.tv_sec += 10; /* 从现在开始 10 秒 */
    /* 注意: 这会导致死锁 */
    err = pthread_mutex_timedlock(&lock, &tout);
    clock_gettime(CLOCK_REALTIME, &tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf, sizeof(buf), "%r", tmp);
    printf("现在时间 %s\n", buf);
    if (err == 0)
        printf("mutex 再次锁定!\n");
    else
        printf("无法再次锁定 mutex: %s\n", strerror(err));
    exit(0);
}
