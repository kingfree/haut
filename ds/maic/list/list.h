/* 
 * 文件: list.h
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月10日
 * 修改时间: 2014年5月10日
 * 描述: 链表头文件
 */

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/* 定义链表元素 */
typedef struct ListElmt_ {
  void *data;
  struct ListElmt_ *next;
} ListElmt;

/* 定义链表 */
typedef struct List_ {
  int size;
  int (*match)(const void *key1, const void *key2);
  void (*destroy)(void *data);
  ListElmt *head;
  ListElmt *tail;
} List;

/* 公共接口 */

void list_init(List *list, void (*destroy)(void *data));

void list_destroy(List *list);

int list_ins_next(List *list, ListElmt *element, const void *data);

int list_rem_next(List *list, ListElmt *element, void **data);

#define list_size(list) ((list)->size)

#define list_head(list) ((list)->head)

#define list_tail(list) ((list)->tail)

#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)

#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)

#define list_data(element) ((element)->data)

#define list_next(element) ((element)->next)

#endif
