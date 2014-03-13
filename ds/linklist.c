/* 文件: linklist.c
 * 作者: 田劲锋
 * 创建: 2014年3月13日
 * 修改: 2014年3月13日
 * 描述: 线性表的链表实现
 */

#include "status.h"

typedef struct {
  ElemType data;
  struct LNode *next;
} LNode, *LinkList;

Status Init(LinkList *L) {
  *L = (LinkList) malloc(sizeof(LNode));
  if (!*L) {
    return Overflow;
  }
  (*L)->next = NULL;
  return Ok;
}

Status Create(LinkList *L, int n) {
  LNode *p;
  int i;
  for (i = 0; i < n; i++) {
    p = (LNode *) malloc(sizeof(LNode));
    p->data = i + 1;
    p->next = (*L)->next;
    (*L)->next = p;
  }
  return Ok;
}

Status Length(LinkList *L) {
  LNode *p;
  int i = 0;
  for (p = (*L)->next; p; p = p->next) {
    i += 1;
  }
  return i;
}

void Traverse(LinkList *L, char* (*visit)(ElemType *)) {
  printf("length: %d\n", Length(L));
  LNode *p;
  for (p = (*L)->next; p; p = p->next) {
    printf("|%s", visit(&p->data));
  }
  printf("|\n");
}

int main() {
  LinkList *l, list;
  puts("初始化");
  Init(l = &list);
  Traverse(l, ev);
  puts("创建");
  Create(l, 10);
  Traverse(l, ev);
  return 0;
}
