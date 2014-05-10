/* 
 * 文件: list.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月10日
 * 修改时间: 2014年5月10日
 * 描述: 链表实现
 */

#include <stdlib.h>
#include <string.h>
#include "list.h"

/* 初始化链表 */
void list_init(List *list, void (*destroy)(void *data)) {
  list->size = 0;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;
}

/* 销毁链表 */
void list_destroy(List *list) {
  void *data;
  while (list_size(list) > 0) {
    if (list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy != NULL) {
      /* 调用数据销毁函数 */
      list->destroy(data);
    }
  }
  /* 保险起见，清空链表 */
  memset(list, 0, sizeof(List));
}

/* 插入元素 */
int list_ins_next(List *list, ListElmt *element, const void *data) {
  ListElmt *new_element;
  /* 分配存储空间 */
  if ((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL)
    return -1;
  /* 插入链表中 */
  new_element->data = (void *)data;
  if (element == NULL) {
    /* 插入链表头部 */
    if (list_size(list) == 0)
      list->tail = new_element;
    new_element->next = list->head;
    list->head = new_element;
  } else {
    /* 插入指定位置 */
    if (element->next == NULL)
      list->tail = new_element;
    new_element->next = element->next;
    element->next = new_element;
  }
  /* 插入后表长增加 */
  list->size++;
  return 0;
}

/* 删除元素 */
int list_rem_next(List *list, ListElmt *element, void **data) {
  ListElmt *old_element;
  /* 不允许删除空表中的元素 */
  if (list_size(list) == 0)
    return -1;
  /* 从链表中删除 */
  if (element == NULL) {
    /* 删除头部的元素 */
    *data = list->head->data;
    old_element = list->head;
    list->head = list->head->next;
    if (list_size(list) == 0)
      list->tail = NULL;
  } else {
    /* 删除指定位置的元素 */
    if (element->next == NULL)
      return -1;
    *data = element->next->data;
    old_element = element->next;
    element->next = element->next->next;
    if (element->next == NULL)
      list->tail = element;
  }
  /* 释放已删除元素的空间 */
  free(old_element);
  /* 删除后表长自减 */
  list->size--;
  return 0;
}
