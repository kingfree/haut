#include "apue.h"
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

#define NTHR 8               /* 线程数 */
#define NUMNUM 8000000L      /* 待排序数量 */
#define TNUM (NUMNUM / NTHR) /* 每个线程排序数量 */

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;

#ifdef SOLARIS
#define heapsort qsort
#else
extern int heapsort(void *, size_t, size_t,
        int (*)(const void *, const void *));
#endif

/*
 * 比较两个长整型数
 */
int complong(const void *arg1, const void *arg2)
{
    long l1 = *(long *)arg1;
    long l2 = *(long *)arg2;

    if (l1 == l2)
        return 0;
    else if (l1 < l2)
        return -1;
    else
        return 1;
}

/*
 * 部分排序线程
 */
void *thr_fn(void *arg)
{
    long idx = (long)arg;

    heapsort(&nums[idx], TNUM, sizeof(long), complong);
    pthread_barrier_wait(&b);

    /*
     * 执行其他操作
     */
    return ((void *)0);
}

/*
 * 合并排序结果
 */
void merge()
{
    long idx[NTHR];
    long i, minidx, sidx, num;

    for (i = 0; i < NTHR; i++) idx[i] = i * TNUM;
    for (sidx = 0; sidx < NUMNUM; sidx++) {
        num = LONG_MAX;
        for (i = 0; i < NTHR; i++) {
            if ((idx[i] < (i + 1) * TNUM) && (nums[idx[i]] < num)) {
                num = nums[idx[i]];
                minidx = i;
            }
        }
        snums[sidx] = nums[idx[minidx]];
        idx[minidx]++;
    }
}

int main()
{
    unsigned long i;
    struct timeval start, end;
    long long startusec, endusec;
    double elapsed;
    int err;
    pthread_t tid;

    /*
     * 初始化待排序数组
     */
    srandom(1);
    for (i = 0; i < NUMNUM; i++) nums[i] = random();

    /*
     * 创建 8 个用于排序的线程
     */
    gettimeofday(&start, NULL);
    pthread_barrier_init(&b, NULL, NTHR + 1);
    for (i = 0; i < NTHR; i++) {
        err = pthread_create(&tid, NULL, thr_fn, (void *)(i * TNUM));
        if (err != 0) err_exit(err, "无法创建线程");
    }
    pthread_barrier_wait(&b);
    merge();
    gettimeofday(&end, NULL);

    /*
     * 打印排序结果
     */
    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec = end.tv_sec * 1000000 + end.tv_usec;
    elapsed = (double)(endusec - startusec) / 1000000.0;
    printf("排序用时 %.4f 秒\n", elapsed);
    for (i = 0; i < NUMNUM; i++) printf("%ld\n", snums[i]);
    exit(0);
}
