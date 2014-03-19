/* �ļ�: linklist.cpp
 * ����: �ﾢ��
 * ����: 2014��3��19��
 * �޸�: 2014��3��19��
 * ����: ���Ա������ʵ��
 */

#include "status.h"

struct LNode {
  ElemType data;
  LNode *next;
};
typedef LNode *LinkList;

Status Init(LinkList &L) {
  L = (LinkList) malloc(sizeof(LNode));
  if (!L) {
    return Overflow;
  }
  L->next = NULL;
  return Ok;
}

Status Create(LinkList &L, int n) {
  LNode *p;
  for (int i = 1; i <= n; i++) {
    p = (LNode *) malloc(sizeof(LNode));
    p->data = i; // ����Ԫ�� i
    p->next = L->next;
    L->next = p;
  }
  return Ok;
}

Status Clear(LinkList &L) {
  // free(L); //....
  LinkList p, q = NULL;
  for (p = L; p; p = p->next) {
    free(q);
    q = p;
    fprintf(stderr, "%p: %8d, %p\n", q, q->data, q->next);
  }
  Init(L);
  return Ok;
}

int Length(LinkList &L) {
  int i = 0;
  for (LNode *p = L->next; p; p = p->next) {
    i += 1;
  }
  return i;
}

ElemType Get(LinkList &L, int i) {
  LNode *p = L->next;
  int j = 1;
  for (; p && j < i; j++) {
    p = p->next;
  }
  if (!p && j > i) {
    return L->data; // Not found
  }
  return p->data;
}

int Locate(LinkList &L, ElemType e, int (*compare)(ElemType, ElemType)) {
  LNode *p = L->next;
  int j = 1;
  for (; p && (*compare)(p->data, e) != 0; j++) {
    p = p->next;
  }
  return p ? j : 0;
}

Status Insert(LinkList &L, int i, ElemType e) {
  LNode *p = L;
  int j = 0;
  for (; p && j < i - 1; j++) {
    p = p->next;
  }
  if (!p || j > i - 1) {
    return Error;
  }
  LinkList s = (LinkList) malloc(sizeof(LNode));
  s->data = e;
  s->next = p->next;
  p->next = s;
  return Ok;
}

Status Delete(LinkList &L, int i, ElemType &e) {
  LNode *p = L;
  int j = 0;
  for (; p && j < i - 1; j++) {
    p = p->next;
  }
  if (!p || j > i - 1) {
    return Error;
  }
  LinkList q = p->next;
  p->next = q->next;
  e = q->data;
  free(q);
  return Ok;
}

void Traverse(LinkList &L, char* (*visit)(ElemType *)) {
  printf("length: %d\n", Length(L));
  LNode *p;
  for (p = L->next; p; p = p->next) {
    printf("|%s", visit(&p->data));
  }
  printf("|\n");
}

int main() {
  LinkList l;
  puts("��ʼ��");
  Init(l);
  Traverse(l, ev);
  puts("����");
  Create(l, 10);
  Traverse(l, ev);
  puts("ȡԪ��");
  printf("%d: %d\n", 3, Get(l, 3));
  printf("%d: %d\n", 6, Get(l, 6));
  puts("���");
  Clear(l);
  Traverse(l, ev);
  puts("����");
  Insert(l, 1, 1);
  Insert(l, 1, 2);
  Insert(l, 1, 3);
  Insert(l, 3, 4);
  Insert(l, 6, 1); // => Error
  Traverse(l, ev); // => |3|2|4|1|
  puts("ɾ��");
  ElemType x;
  Delete(l, 2, x);
  Delete(l, Locate(l, 3, cmp), x);
  Traverse(l, ev); // => |3|4|
  puts("����");
  printf("Find: %d at %d\n", 1, Locate(l, 1, cmp)); // => 2
  printf("Find: %d at %d\n", 3, Locate(l, 3, cmp)); // => 0, Not found
  return 0;
}
