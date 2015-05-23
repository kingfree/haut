/* FIFO队列 */

#include "bootpack.h"

#define FLAGS_OVERRUN 0x0001

/* 初始化FIFO缓冲区 */
void fifo32_init(fifo32* q, int size, int* buf, struct TASK* task)
{
    q->size = size;
    q->buf = buf;
    q->free = size; /* 空 */
    q->flags = 0;
    q->p = 0; /* 队尾 */
    q->q = 0; /* 队首 */
    q->task = task; /* 有数据写入时需要唤醒的任务 */
    return;
}

/* 压入FIFO堆里 */
int fifo32_put(fifo32* q, int data)
{
    if (q->free == 0) {
        /* 溢出 */
        q->flags |= FLAGS_OVERRUN;
        return -1;
    }
    q->buf[q->p] = data;
    q->p++;
    if (q->p == q->size) {
        q->p = 0;
    }
    q->free--;
    if (q->task != 0) {
        if (q->task->flags != 2) { /* 任务在休眠 */
            task_run(q->task, -1, 0); /* 唤醒 */
        }
    }
    return 0;
}

/* 弹出FIFO队列 */
int fifo32_get(fifo32* q)
{
    int data;
    if (q->free == q->size) {
        /* 已空，返回-1 */
        return -1;
    }
    data = q->buf[q->q];
    q->q++;
    if (q->q == q->size) {
        q->q = 0;
    }
    q->free++;
    return data;
}

/* 队列长度 */
int fifo32_status(fifo32* q)
{
    return q->size - q->free;
}
