/* �ļ�: linklist.cpp
 * ����: �ﾢ��
 * ����: 2014��3��13��
 * �޸�: 2014��3��13��
 * ����: ���Ա������ʵ��
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
  puts("��ʼ��");
  Init(l);
  return 0;
}
