#include "apue.h"
#include <pthread.h>

struct foo {
    int a, b, c, d;
};

void printfoo(const char *s, const struct foo *fp)
{
    printf("%s", s);
    printf("  结构体 0x%lx\n", (unsigned long)fp);
    printf("  foo.a = %d\n", fp->a);
    printf("  foo.b = %d\n", fp->b);
    printf("  foo.c = %d\n", fp->c);
    printf("  foo.d = %d\n", fp->d);
}

void *thr_fn1(void *arg)
{
    struct foo foo = {1, 2, 3, 4};

    printfoo("线程 1:\n", &foo);
    pthread_exit((void *)&foo);
}

void *thr_fn2(void *arg)
{
    printf("线程 2: ID %lu\n", (unsigned long)pthread_self());
    pthread_exit((void *)0);
}

int main(void)
{
    int err;
    pthread_t tid1, tid2;
    struct foo *fp;

    err = pthread_create(&tid1, NULL, thr_fn1, NULL);
    if (err != 0) err_exit(err, "无法创建线程1");
    err = pthread_join(tid1, (void *)&fp);
    if (err != 0) err_exit(err, "无法同步线程1");
    sleep(1);
    printf("父线程启动第二个线程\n");
    err = pthread_create(&tid2, NULL, thr_fn2, NULL);
    if (err != 0) err_exit(err, "无法创建线程2");
    sleep(1);
    printfoo("父线程:\n", fp);
    exit(0);
}
