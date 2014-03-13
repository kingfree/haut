/* 文件: sqlist.c
 * 作者: 田劲锋
 * 创建: 2014年3月3日
 * 修改: 2014年3月13日
 * 描述: 线性表的顺序表实现
 */

#include "status.h"

#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10

typedef struct {
  ElemType *elem;
  size_t length;
  size_t listsize;
} SqList;

/* 初始化，传入指针，构造一个空的线性表 */
Status Init(SqList *this) {
  this->elem = (ElemType *) malloc(LIST_INIT_SIZE * sizeof(ElemType)); // 为元素申请一定长度的空间
  if (!this->elem) { // 内存分配失败
    exit(Overflow);
  }
  this->length = 0; // 初始长度为 0
  this->listsize = LIST_INIT_SIZE; // 初始存储容量
  return Ok;
}

Status Destory(SqList *this) {
  free(this->elem);
  free(this);
  return 0;
}

Status Clear(SqList *this) {
  ElemType * newbase = (ElemType *) realloc(this->elem, LIST_INIT_SIZE * sizeof(ElemType));
  if (!newbase) {
    exit(Overflow);
  }
  this->elem = newbase;
  this->length = 0;
  this->listsize = LIST_INIT_SIZE;
  if (this->elem != NULL) {
    return Ok;
  } else {
    return Error;
  }
}

int Empty(SqList *this) {
  return this->length == 0;
}

size_t Length(SqList *this) {
  return this->length;
}

ElemType Get(SqList *this, int i) {
  return this->elem[i];
}

int Locate(SqList *this, ElemType e, int (*compare)(ElemType, ElemType)) {
  int i = 1;
  ElemType *p = this->elem;
  for (; i <= this->length && (*compare)(*p++, e) != 0; i++)
    ;
  return i <= this->length ? i : 0;
}

Status Prev(SqList *this, ElemType cur_e, ElemType *pre_e, int (*compare)(ElemType, ElemType)) {
  int i = Locate(this, cur_e, compare) - 1;
  if (i > 1 && i < this->length) {
    *pre_e = this->elem[i - 1];
    return Ok;
  } else {
    return NotFound;
  }
}

Status Next(SqList *this, ElemType cur_e, ElemType *next_e, int (*compare)(ElemType, ElemType)) {
  int i = Locate(this, cur_e, compare) - 1;
  if (i > 0 && i < this->length - 1) {
    *next_e = this->elem[i + 1];
    return Ok;
  } else {
    return NotFound;
  }
}

Status Insert(SqList *this, int i, ElemType e) {
  if (i < 1 || i > this->length + 1) {
    return Error;
  }
  if (this->length >= this->listsize) {
    ElemType * newbase = (ElemType *) realloc(this->elem, (this->listsize + LISTINCREMENT) * sizeof(ElemType));
    if (!newbase) {
      exit(Overflow);
    }
    this->elem = newbase;
    this->listsize += LISTINCREMENT;
  }
  ElemType *p, *q;
  q = this->elem + i - 1;
  for (p = this->elem + this->length - 1; p >= q; p--) {
    *(p + 1) = *p; // 插入位置及之后的元素右移
  }
  *q = e; // 插入 e
  this->length++; // 表长增加
  return Ok;
}

ElemType Delete(SqList *this, int i) {
  if (i < 1 || i > this->length + 1) {
    return Error;
  }
  ElemType *p = this->elem + i - 1;
  ElemType e = *p;
  ElemType *q = this->elem + this->length - 1;
  for (p++; p <= q; p++) {
    *(p - 1) = *p;
  }
  this->length--; // 表长减少
  return e;
}

void Traverse(SqList *this, char* (*visit)(ElemType *)) {
  printf("length: %d, listsize: %d\n", this->length, this->listsize);
  ElemType *i;
  for (i = this->elem; i < this->elem + this->length; i++) {
    printf("|%s", visit(i));
  }
  printf("|\n");
}

int main() {
  SqList *l, list;
  puts("初始化");
  Init(l = &list); // l 是 list 的指针
  Traverse(l, elemvisit); // => |
  puts("插入");
  Insert(l, 1, 1);
  Insert(l, 1, 2);
  Insert(l, 1, 3);
  Insert(l, 3, 4);
  Insert(l, 6, 1); // => Error
  Traverse(l, elemvisit); // => |3|2|4|1|
  puts("删除");
  Delete(l, 2);
  Delete(l, Locate(l, 3, cmp));
  Traverse(l, elemvisit); // => |3|4|
  puts("查找");
  printf("Find: %d at %d\n", 1, Locate(l, 1, cmp)); // => 2
  printf("Find: %d at %d\n", 3, Locate(l, 3, cmp)); // => 0, Not found
  puts("清除");
  Clear(l);
  Traverse(l, elemvisit);
  puts("重新录入数据");
  int i;
  for (i = 1; i <= 101; i++) {
    Insert(l, Length(l) + 1, i);
  }
  Traverse(l, elemvisit);
  puts("前驱后继");
  ElemType x;
  Prev(l, 5, &x, cmp);
  printf("Prev(%d): %d\n", 5, x);
  Next(l, 5, &x, cmp);
  printf("Next(%d): %d\n", 5, x);
  return 0;
}
