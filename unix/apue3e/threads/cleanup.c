#include "apue.h"
#include <pthread.h>

void cleanup(void *arg) { printf("清理: %s\n", (char *)arg); }

void *thr_fn1(void *arg)
{
    printf("线程1 启动\n");
    pthread_cleanup_push(cleanup, "线程1 首个 handler");
    pthread_cleanup_push(cleanup, "线程1 次个 handler");
    printf("线程1 push 完成\n");
    if (arg) return ((void *)1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return ((void *)1);
}

void *thr_fn2(void *arg)
{
    printf("线程2 启动\n");
    pthread_cleanup_push(cleanup, "线程2 首个 handler");
    pthread_cleanup_push(cleanup, "线程2 次个 handler");
    printf("线程2 push 完成\n");
    if (arg) pthread_exit((void *)2);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void *)2);
}

int main(void)
{
    int err;
    pthread_t tid1, tid2;
    void *tret;

    err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);
    if (err != 0) err_exit(err, "无法创建线程1");
    err = pthread_create(&tid2, NULL, thr_fn2, (void *)1);
    if (err != 0) err_exit(err, "无法创建线程2");
    err = pthread_join(tid1, &tret);
    if (err != 0) err_exit(err, "无法同步线程1");
    printf("线程1返回: %ld\n", (long)tret);
    err = pthread_join(tid2, &tret);
    if (err != 0) err_exit(err, "无法同步线程2");
    printf("线程2返回: %ld\n", (long)tret);
    exit(0);
}
