/* 
 * 文件: queue.h
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月11日
 * 修改时间: 2014年5月11日
 * 描述: 队列头文件
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#include "list.h"

/* 队列其实是链表 */

typedef List Queue;

/* 公共接口 */

#define queue_init list_init

#define queue_destroy list_destroy

int queue_enqueue(Queue *queue, const void *data);

int queue_dequeue(Queue *queue, void **data);

#define queue_peek(queue) ((queue)->head == NULL ? NULL : (queue)->head->data)

#define queue_size list_size

#endif
