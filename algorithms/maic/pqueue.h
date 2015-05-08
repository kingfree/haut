/* 
 * 文件: pqueue.h
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月12日
 * 修改时间: 2014年5月12日
 * 描述: 优先队列的头文件
 */

#ifndef PQUEUE_H
#define PQUEUE_H

#include "heap.h"

/* 优先队列可用堆实现 */
typedef Heap PQueue;

/* 公共接口 */

#define pqueue_init heap_init

#define pqueue_destroy heap_destroy

#define pqueue_insert heap_insert

#define pqueue_extract heap_extract

#define pqueue_peek(pqueue) ((pqueue)->tree == NULL ? NULL : (pqueue)->tree[0])

#define pqueue_size heap_size

#endif
