/* 
 * 文件: queue.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月11日
 * 修改时间: 2014年5月11日
 * 描述: 队列实现
 */

#include <stdlib.h>

#include "list.h"
#include "queue.h"

/* 入队 */
int queue_enqueue(Queue *queue, const void *data) {
  /* 插入尾部 */
  return list_ins_next(queue, list_tail(queue), data);
}

/* 出队 */
int queue_dequeue(Queue *queue, void **data) {
  /* 删除首部 */
  return list_rem_next(queue, NULL, data);
}

