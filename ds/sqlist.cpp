/* �ļ�: sqlist.cpp
 * ����: �ﾢ��
 * ����ʱ��: 2014��3��3��
 * �޸�ʱ��: 2014��3��13��
 * ����: ���Ա��˳���ʵ��
 */

#include "status.h"

const int LIST_INIT_SIZE = 100;
const int LISTINCREMENT = 10;

struct SqList {
  ElemType *elem;
  size_t length;
  size_t listsize;

  // ��ʼ������
  SqList() {
    elem = (ElemType *) malloc(LIST_INIT_SIZE * sizeof(ElemType));
    if (!elem) {
      exit(Overflow);
    }
    length = 0;
    listsize = LIST_INIT_SIZE;
  }

  Status Destory() {
    free(elem);
    if (elem = NULL) {
      return Ok;
    } else {
      return Error;
    }
  }

  Status Clear() {
    ElemType * newbase = (ElemType *) realloc(elem, LIST_INIT_SIZE * sizeof(ElemType));
    if (!newbase) {
      exit(Overflow);
    }
    elem = newbase;
    length = 0;
    listsize = LIST_INIT_SIZE;
    if (elem != NULL) {
      return Ok;
    } else {
      return Error;
    }
  }

  bool Empty() {
    return length == 0;
  }

  size_t Length() {
    return length;
  }

  ElemType Get(int i) {
    return elem[i];
  }

  int Locate(ElemType e, int (*compare)(ElemType, ElemType) = cmp) {
    int i = 1;
    ElemType *p = elem;
    for (; i <= length && (*compare)(*p++, e) != 0; i++)
      ;
    return i <= length ? i : 0;
  }

  Status Prev(ElemType cur_e, ElemType &pre_e) {
    int i = Locate(cur_e) - 1;
    if (i > 1 && i < length) {
      pre_e = elem[i - 1];
      return Ok;
    } else {
      return NotFound;
    }
  }

  Status Next(ElemType cur_e, ElemType &next_e) {
    int i = Locate(cur_e) - 1;
    if (i > 0 && i < length - 1) {
      next_e = elem[i + 1];
      return Ok;
    } else {
      return NotFound;
    }
  }

  Status Insert(int i, ElemType e) {
    if (i < 1 || i > length + 1) {
      return Error;
    }
    if (length >= listsize) {
      ElemType * newbase = (ElemType *) realloc(elem, (listsize + LISTINCREMENT) * sizeof(ElemType));
      if (!newbase) {
        exit(Overflow);
      }
      elem = newbase;
      listsize += LISTINCREMENT;
    }
    ElemType *q = elem + i - 1;
    for (ElemType *p = elem + length - 1; p >= q; p--) {
      *(p + 1) = *p; // ����λ�ü�֮���Ԫ������
    }
    *q = e; // ���� e
    length++; // ������
    return Ok;
  }

  ElemType Delete(int i) {
    if (i < 1 || i > length + 1) {
      return Error;
    }
    ElemType *p = elem + i - 1;
    ElemType e = *p;
    ElemType *q = elem + length - 1;
    for (p++; p <= q; p++) {
      *(p - 1) = *p;
    }
    length--; // ������
    return e;
  }

  void Traverse(char* (*visit)(ElemType *) = elemvisit) {
    printf("length: %d, listsize: %d\n", length, listsize);
    for (ElemType *i = elem; i < elem + length; i++) {
      printf("|%s", visit(i));
    }
    printf("|\n");
  }

};

int main() {
  SqList l; // �Զ����ó�ʼ������
  puts("��ʼ��");
  l.Traverse(); // => |
  puts("����");
  l.Insert(1, 1);
  l.Insert(1, 2);
  l.Insert(1, 3);
  l.Insert(3, 4);
  l.Insert(6, 1); // => Error
  l.Traverse(); // => |3|2|4|1|
  puts("ɾ��");
  l.Delete(2);
  l.Delete(l.Locate(3));
  l.Traverse(); // => |3|4|
  puts("����");
  printf("Find: %d at %d\n", 1, l.Locate(1)); // => 2
  printf("Find: %d at %d\n", 3, l.Locate(3)); // => 0, Not found
  puts("���");
  l.Clear();
  l.Traverse();
  puts("����¼������");
  for (int i = 1; i <= 101; i++) {
    l.Insert(l.Length() + 1, i);
  }
  l.Traverse();
  puts("ǰ�����");
  ElemType x;
  l.Prev(5, x);
  printf("Prev(%d): %d\n", 5, x);
  l.Next(5, x);
  printf("Next(%d): %d\n", 5, x);
  return 0;
}
