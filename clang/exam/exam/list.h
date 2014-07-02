/* list.h
 * 链表
 */

#ifndef _LIST_H_
#define _LIST_H_

typedef struct list {
  void *data;
  struct list *next;
} list;

#endif
