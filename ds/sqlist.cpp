#include "status.h"

const int LIST_INIT_SIZE = 100;
const int LISTINCREMENT = 10;

typedef int ElemType;

int cmp(const ElemType a, const ElemType b) {
  return a - b;
}

struct SqList {
  ElemType *elem;
  size_t length;
  size_t listsize;

  // 无参数初始化
  SqList() {
    elem = (ElemType *) malloc(LIST_INIT_SIZE * sizeof(ElemType));
    if (!elem) {
      exit(Overflow);
    }
    length = 0;
    listsize = LIST_INIT_SIZE;
    //return Ok;
  }

  // 打印当前表中信息
  void print() {
    printf("length: %d, listsize: %d\n", length, listsize);
    for (ElemType *i = elem; i < elem + length; i++) {
      printf("|%d", *i);
    }
    printf("|\n");
  }

  Status insert(int i, ElemType e) {
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
      *(p + 1) = *p; // 插入位置及之后的元素右移
    }
    *q = e; // 插入 e
    length++; // 表长增加
    return Ok;
  }

  ElemType del(int i) {
    if (i < 1 || i > length + 1) {
      return Error;
    }
    ElemType *p = elem + i - 1;
    ElemType e = *p;
    ElemType *q = elem + length - 1;
    for (p++; p <= q; p++) {
      *(p - 1) = *p;
    }
    length--; // 表长减少
    return e;
  }

  int locate(ElemType e, int (*compare)(ElemType, ElemType) = cmp) {
    int i = 1;
    ElemType *p = elem;
    for (; i <= length && (*compare)(*p++, e) != 0; i++)
      ;
    return i <= length ? i : 0;
  }
};

int main() {
  SqList l;
  l.print(); // => |
  l.insert(1, 1);
  l.insert(1, 2);
  l.insert(1, 3);
  l.insert(3, 4);
  l.insert(6, 1); // => Error
  l.print(); // => |3|2|4|1|
  l.del(2);
  l.del(l.locate(3));
  l.print(); // => |3|4|
  printf("Find: %d at %d\n", 1, l.locate(1)); // => 2
  printf("Find: %d at %d\n", 3, l.locate(3)); // => 0, Not found
  return 0;
}
