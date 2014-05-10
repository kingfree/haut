/* 文件: linkqueue.c
 * 作者: 田劲锋
 * 创建: 2014年3月26日
 * 修改: 2014年3月27日
 * 描述: 队列的链表实现
 */

#include "status.h"

typedef struct QNode {
  ElemType data;
  struct QNode *next;
} QNode;

typedef struct {
  QNode *front;
  QNode *rear;
} LinkQueue;

Status Init(LinkQueue *Q) {
  Q->front = Q->rear = (QNode *) malloc(sizeof(QNode));
  if (!Q->front) return Overflow;
  Q->rear->next = NULL;
  return Ok;
}

Status Destroy(LinkQueue *Q) {
  while (Q->front) {
    Q->rear = Q->front->next;
    free(Q->front);
    Q->front = Q->rear;
  }
  return Ok;
}

int Empty(LinkQueue *Q) {
  return Q->front == Q->rear;
}

Status EnQueue(LinkQueue *Q, ElemType e) {
  QNode *p = (QNode *) malloc(sizeof(QNode));
  if (!p) return Overflow;
  p->data = e;
  p->next = NULL;
  Q->rear->next = p;
  Q->rear = p;
  return Ok;
}

ElemType DeQueue(LinkQueue *Q) {
  if (Empty(Q)) return -1;
  QNode *p = Q->front->next;
  ElemType e = p->data;
  Q->front->next = p->next;
  if (Q->rear == p) Q->rear = Q->front;
  free(p);
  return e;
}

void Traverse(LinkQueue *Q, char* (*visit)(ElemType *)) {
  if (Empty(Q)) printf("_");
  QNode *p;
  for (p = Q->front; p != Q->rear; p = p->next) {
    printf("|%s", visit(&p->next->data));
  }
  printf("|\n");
}

#define STRLEN 256

int main() {
  LinkQueue *q, qq;
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
