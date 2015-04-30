/* FIFO队列 */

#include "bootpack.h"

#define FLAGS_OVERRUN 0x0001

void fifo32_init(fifo32 *q, int size, int *buf)
/* 初始化FIFO缓冲区 */
{
    q->size = size;
    q->buf = buf;
    q->free = size; /* 空 */
    q->flags = 0;
    q->p = 0; /* 队尾 */
    q->q = 0; /* 队首 */
    return;
}

int fifo32_put(fifo32 *q, int data)
/* 压入FIFO堆里 */
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
    return 0;
}

int fifo32_get(fifo32 *q)
/* 弹出FIFO队列 */
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

int fifo32_status(fifo32 *q)
/* 队列长度 */
{
    return q->size - q->free;
}