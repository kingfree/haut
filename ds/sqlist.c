/* �ļ�: sqlist.c
 * ����: �ﾢ��
 * ����: 2014��3��3��
 * �޸�: 2014��3��13��
 * ����: ���Ա��˳���ʵ��
 */

#include "status.h"

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

typedef struct {
  ElemType *elem;
  size_t length;
  size_t listsize;
} SqList;

/* ��ʼ��������ָ�룬����һ���յ����Ա� */
Status Init(SqList *L) {
  L->elem = (ElemType *) malloc(LIST_INIT_SIZE * sizeof(ElemType)); // ΪԪ������һ�����ȵĿռ�
  if (!L->elem) { // �ڴ����ʧ��
    exit(Overflow);
  }
  L->length = 0; // ��ʼ����Ϊ 0
  L->listsize = LIST_INIT_SIZE; // ��ʼ�洢����
  return Ok;
}

Status Destory(SqList *L) {
  free(L->elem);
  free(L);
  return Ok;
}

Status Clear(SqList *L) {
  ElemType * newbase = (ElemType *) realloc(L->elem, LIST_INIT_SIZE * sizeof(ElemType));
  if (!newbase) {
    exit(Overflow);
  }
  L->elem = newbase;
  L->length = 0;
  L->listsize = LIST_INIT_SIZE;
  if (L->elem != NULL) {
    return Ok;
  } else {
    return Error;
  }
}

int Empty(SqList *L) {
  return L->length == 0;
}

size_t Length(SqList *L) {
  return L->length;
}

ElemType Get(SqList *L, int i) {
  return L->elem[i];
}

int Locate(SqList *L, ElemType e, int (*compare)(ElemType, ElemType)) {
  int i = 1;
  ElemType *p = L->elem;
  for (; i <= L->length && (*compare)(*p++, e) != 0; i++)
    ;
  return i <= L->length ? i : 0;
}

Status Prev(SqList *L, ElemType cur_e, ElemType *pre_e, int (*compare)(ElemType, ElemType)) {
  int i = Locate(L, cur_e, compare) - 1;
  if (i > 1 && i < L->length) {
    *pre_e = L->elem[i - 1];
    return Ok;
  } else {
    return NotFound;
  }
}

Status Next(SqList *L, ElemType cur_e, ElemType *next_e, int (*compare)(ElemType, ElemType)) {
  int i = Locate(L, cur_e, compare) - 1;
  if (i > 0 && i < L->length - 1) {
    *next_e = L->elem[i + 1];
    return Ok;
  } else {
    return NotFound;
  }
}

Status Insert(SqList *L, int i, ElemType e) {
  if (i < 1 || i > L->length + 1) {
    return Error;
  }
  if (L->length >= L->listsize) {
    ElemType * newbase = (ElemType *) realloc(L->elem, (L->listsize + LISTINCREMENT) * sizeof(ElemType));
    if (!newbase) {
      exit(Overflow);
    }
    L->elem = newbase;
    L->listsize += LISTINCREMENT;
  }
  ElemType *p, *q;
  q = L->elem + i - 1;
  for (p = L->elem + L->length - 1; p >= q; p--) {
    *(p + 1) = *p; // ����λ�ü�֮���Ԫ������
  }
  *q = e; // ���� e
  L->length++; // ������
  return Ok;
}

ElemType Delete(SqList *L, int i) {
  if (i < 1 || i > L->length + 1) {
    return Error;
  }
  ElemType *p = L->elem + i - 1;
  ElemType e = *p;
  ElemType *q = L->elem + L->length - 1;
  for (p++; p <= q; p++) {
    *(p - 1) = *p;
  }
  L->length--; // ������
  return e;
}

void Traverse(SqList *L, char* (*visit)(ElemType *)) {
  printf("length: %d, listsize: %d\n", L->length, L->listsize);
  ElemType *i;
  for (i = L->elem; i < L->elem + L->length; i++) {
    printf("|%s", visit(i));
  }
  printf("|\n");
}

int main() {
  SqList *l, list;
  puts("��ʼ��");
  Init(l = &list); // l �� list ��ָ��
  Traverse(l, ev); // => |
  puts("����");
  Insert(l, 1, 1);
  Insert(l, 1, 2);
  Insert(l, 1, 3);
  Insert(l, 3, 4);
  Insert(l, 6, 1); // => Error
  Traverse(l, ev); // => |3|2|4|1|
  puts("ɾ��");
  Delete(l, 2);
  Delete(l, Locate(l, 3, cmp));
  Traverse(l, ev); // => |3|4|
  puts("����");
  printf("Find: %d at %d\n", 1, Locate(l, 1, cmp)); // => 2
  printf("Find: %d at %d\n", 3, Locate(l, 3, cmp)); // => 0, Not found
  puts("���");
  Clear(l);
  Traverse(l, ev);
  puts("����¼������");
  int i;
  for (i = 1; i <= 101; i++) {
    Insert(l, Length(l) + 1, i);
  }
  Traverse(l, ev);
  puts("ǰ�����");
  ElemType x;
  Prev(l, 5, &x, cmp);
  printf("Prev(%d): %d\n", 5, x);
  Next(l, 5, &x, cmp);
  printf("Next(%d): %d\n", 5, x);
  return 0;
}
