/* 
 * 文件: list_demo.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月10日
 * 修改时间: 2014年5月10日
 * 描述: 链表操作的演示
 */

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

/* 打印链表 */
static void print_list(const List *list) {
  ListElmt *element;
  int *data, i;
  printf("表长: %d\n", list_size(list));
  /* 空表 */
  if (list_size(list) < 1)
    return;
  /* 从头循环到尾 */
  for (element = list_head(list); !list_is_tail(element); element = list_next(element)) {
    printf("%d ", *(data = list_data(element)));
  }
  printf("\n");
}

/* 主函数 */
int main(int argc, char **argv) {
  List list;
  ListElmt *element;
  int *data, i;

  printf("初始化链表\n");
  list_init(&list, free);
  print_list(&list);

  printf("插入 10 个元素\n");
  element = list_head(&list);
  for (i = 10; i > 0; i--) {
    if ((data = (int *)malloc(sizeof(int))) == NULL)
      return 1;
    *data = i;
    if (list_ins_next(&list, NULL, data) != 0)
      return 1;
  }
  print_list(&list);

  element = list_head(&list);
  for (i = 0; i < 7; i++)
    element = list_next(element);
  data = list_data(element);
  printf("删除 %d 之后的元素\n", *data);
  if (list_rem_next(&list, element, (void **)&data) != 0)
    return 1;
  print_list(&list);

  printf("在尾部插入 11\n");
  *data = 11;
  if (list_ins_next(&list, list_tail(&list), data) != 0)
    return 1;
  print_list(&list);

  printf("删除首元素之后的元素\n");
  element = list_head(&list);
  if (list_rem_next(&list, element, (void **)&data) != 0)
    return 1;
  print_list(&list);

  printf("在头部插入 12\n");
  *data = 12;
  if (list_ins_next(&list, NULL, data) != 0)
    return 1;
  print_list(&list);

  printf("定位并删除第四个元素\n");
  element = list_head(&list);
  element = list_next(element);
  element = list_next(element);
  if (list_rem_next(&list, element, (void **)&data) != 0)
    return 1;
  print_list(&list);

  printf("在首元素之后插入 13\n");
  *data = 13;
  if (list_ins_next(&list, list_head(&list), data) != 0)
    return 1;
  print_list(&list);

  i = list_is_head(&list, list_head(&list));
  printf("测试 list_is_head...值=%d (应为1)\n", i);
  i = list_is_head(&list, list_tail(&list));
  printf("测试 list_is_head...值=%d (应为0)\n", i);
  i = list_is_tail(list_tail(&list));
  printf("测试 list_is_tail...值=%d (应为1)\n", i);
  i = list_is_tail(list_head(&list));
  printf("测试 list_is_tail...值=%d (应为0)\n", i);

  printf("销毁链表\n");
  list_destroy(&list);

  return 0;
}
