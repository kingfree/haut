/* 文件: sqqueue.c
 * 作者: 田劲锋
 * 创建: 2014年3月27日
 * 修改: 2014年3月27日
 * 描述: 循环队列的顺序表实现
 */

#include "status.h"

#define MAXQSIZE 100

typedef struct {
  ElemType *base;
  int front;
  int rear;
} SqQueue;

Status Init(SqQueue *Q) {
  Q->base = (ElemType *) malloc(MAXQSIZE * sizeof(ElemType));
  if (!Q->base) return Overflow;
  Q->front = Q->rear = 0;
  return Ok;
}

int Length(SqQueue *Q) {
  return (Q->rear - Q->front + MAXQSIZE) % MAXQSIZE;
}

int Empty(SqQueue *Q) {
  return Q->rear == Q->front;
}

Status EnQueue(SqQueue *Q, ElemType e) {
  if ((Q->rear + 1) % MAXQSIZE == Q->front) return Overflow;
  Q->base[Q->rear] = e;
  Q->rear = (Q->rear + 1) % MAXQSIZE;
  return Ok;
}

ElemType DeQueue(SqQueue *Q) {
  if (Empty(Q)) return -1;
  ElemType e = Q->base[Q->front];
  Q->front = (Q->front + 1) % MAXQSIZE;
  return e;
}

void Traverse(SqQueue *Q, char* (*visit)(ElemType *)) {
  if (Empty(Q)) printf("_");
  int i;
  for (i = Q->front; i != Q->rear; i = (i + 1) % MAXQSIZE) {
    printf("|%s", visit(&Q->base[i]));
  }
  printf("|\n");
}

#define STRLEN 256

int main() {
  SqQueue *q, qq;
  Init(q = &qq);
  Traverse(q, ev);
  printf("CMD e/d/q:\n$ ");
  char c[STRLEN];
  int x;
  while (fgets(c, STRLEN, stdin) != NULL) {
    if (c[0] == 'q') {                  // quit
      break;
    } else if (c[0] == 'e') {           // enqueue
      sscanf(c, "e %d", &x);
      printf("enqueue: %d\n", x);
      EnQueue(q, x);
      Traverse(q, ev);
    } else if (c[0] == 'd') {           // dequeue
      printf("dequeue: %d\n", DeQueue(q));
      Traverse(q, ev);
    }
    printf("$ ");
  }
  return 0;
}
