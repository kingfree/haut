/* 文件: linklist.cpp
 * 作者: 田劲锋
 * 创建: 2014年3月13日
 * 修改: 2014年3月13日
 * 描述: 线性表的链表实现
 */

#include "status.h"

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

typedef struct {
  ElemType data;
  struct LNode *next;
} LNode, *LinkList;

Status Init(LinkList this) {
  this = (LinkList) malloc(sizeof(LNode));
  this->next = NULL;
  return Ok;
}

int main() {
  LinkList l;
  puts("初始化");
  Init(l);
  return 0;
}
