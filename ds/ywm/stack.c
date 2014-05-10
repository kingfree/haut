/* 文件: stack.c
 * 作者: 田劲锋
 * 创建: 2014年3月26日
 * 修改: 2014年3月26日
 * 描述: 栈的实现
 */

#include "status.h"

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef struct Stack {
  ElemType *base;
  ElemType *top;
  size_t size;
} Stack;

Status Init(Stack *S) {
  S->base = (ElemType *) malloc(STACK_INIT_SIZE * sizeof(ElemType));
  if (!S->base) return Overflow;
  S->top = S->base;
  S->size = STACK_INIT_SIZE;
  return Ok;
}

int Empty(Stack *S) {
  return S->top == S->base;
}

ElemType* Top(Stack *S) {
  return Empty(S) ? NULL : S->top - 1;
}

Status Push(Stack *S, ElemType e) {
  if (S->top - S->base >= S->size) {
    S->base = (ElemType *) realloc(S->base, (S->size + STACKINCREMENT) * sizeof(ElemType));
    if (!S->base) return Overflow;
    S->top = S->base + S->size;
    S->size += STACKINCREMENT;
  }
  *S->top++ = e;
  return Ok;
}

ElemType* Pop(Stack *S) {
  return Empty(S) ? NULL : --S->top;
}

void Traverse(Stack *S, char* (*visit)(ElemType *)) {
  if (Empty(S)) printf("_");
  ElemType *i;
  for (i = S->base; i < S->top; i++) {
    printf("|%s", visit(i));
  }
  printf("|\n");
}

#define STRLEN 256

int main() {
  Stack *s, ss;
  Init(s = &ss);
  Traverse(s, ev);
  printf("CMD push x/top/pop/quit:\n$ ");
  char c[STRLEN];
  int x;
  while (fgets(c, STRLEN, stdin) != NULL) {
    if (c[0] == 'q') {                  // quit
      break;
    } else if (c[0] == 't') {           // top
      printf("top: %s\n", ev(Top(s)));
    } else if (c[1] == 'o') {           // pop
      printf("pop: %s\n", ev(Pop(s)));
      Traverse(s, ev);
    } else if (c[1] == 'u') {           // push x
      sscanf(c, "push %d", &x);
      Push(s, x);
      printf("push: %s\n", ev(&x));
      Traverse(s, ev);
    }
    printf("$ ");
  }
  return 0;
}
