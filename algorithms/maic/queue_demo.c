/* 
 * 文件: queue_demo.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月11日
 * 修改时间: 2014年5月11日
 * 描述: 队列操作的演示
 */

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "queue.h"

/* 打印队列 */
static void print_queue (const Queue * queue) {
  ListElmt *element;
  int *data, size, i;
  printf("  队列大小: %d\n  ", size = list_size(queue));
  element = list_head(queue);
  for (i = 0; i < size; i++) {
    data = list_data(element);
    printf("%d ", *data);
    element = list_next(element);
  }
  printf("\n");
}

/* 主函数 */
int main (int argc, char **argv) {
  Queue queue;
  int *data, i;

  printf("初始化队列\n");
  queue_init(&queue, free);
  print_queue(&queue);

  printf("入队 10 个元素\n");
  for (i = 0; i < 10; i++) {
    if ((data =(int *) malloc(sizeof(int))) == NULL)
      return 1;
    *data = i + 1;
    if (queue_enqueue(&queue, data) != 0)
      return 1;
  }
  print_queue(&queue);

  printf("出队 5 个元素\n");
  for (i = 0; i < 5; i++) {
    if (queue_dequeue(&queue, (void **) &data) == 0)
      free(data);
    else
      return 1;
  }
  print_queue(&queue);

  printf("入队 100 和 200\n");
  if ((data =(int *) malloc(sizeof(int))) == NULL)
    return 1;
  *data = 100;
  if (queue_enqueue(&queue, data) != 0)
    return 1;
  if ((data =(int *) malloc(sizeof(int))) == NULL)
    return 1;
  *data = 200;
  if (queue_enqueue(&queue, data) != 0)
    return 1;
  print_queue(&queue);

  if ((data = queue_peek(&queue)) != NULL)
    printf("检查队列顶...值=%d\n", *data);
  else
    printf("检查队列顶...值=NULL\n");
  print_queue(&queue);

  printf("出队所有元素\n");
  while (queue_size(&queue) > 0) {
    if (queue_dequeue(&queue, (void **) &data) == 0)
      free(data);
  }
  if ((data = queue_peek(&queue)) != NULL)
    printf("检查空队列...值=%d\n", *data);
  else
    printf("检查空队列...值=NULL\n");
  print_queue(&queue);

  printf("销毁队列\n");
  queue_destroy(&queue);

  return 0;
}
