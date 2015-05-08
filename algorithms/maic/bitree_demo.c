/* 
 * 文件: bitree_demo.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月11日
 * 修改时间: 2014年5月11日
 * 描述: 二叉树操作的演示
 */
#include <stdio.h>
#include <stdlib.h>

#include "bitree.h"
#include "traverse.h"

/* 打印链表 */
static void print_list(const List *list) {
  ListElmt *element;
  int *data, size, i;
  size = list_size(list);
  element = list_head(list);
  for (i = 0; i < size; i++) {
    data = list_data(element);
    printf("%d ", *data);
    element = list_next (element);
  }
  printf("\n");
}

/* 打印二叉树 */
void bitree_print(const BiTreeNode *node, int order) {
  List list;
  list_init(&list, free);
  if (order == 1)
    preorder(node, &list);
  else if (order == 0)
    inorder(node, &list);
  else
    postorder(node, &list);
  print_list(&list);
}

#define print_preorder(node) bitree_print(node, 1)

#define print_inorder(node) bitree_print(node, 0)

#define print_postorder(node) bitree_print(node, 2)

/* 插入整数 */
static int insert_int(BiTree * tree, int i) {
  BiTreeNode *node, *prev;
  int direction, *data;
  /* 作为二叉查找树来插入结点 */
  node = tree->root;
  direction = 0;
  while (!bitree_is_eob(node)) {
    prev = node;
    if (i == *(int *) bitree_data(node)) {
      return -1;
    } else if (i < *(int *) bitree_data(node)) {
      node = bitree_left(node);
      direction = 1;
    } else {
      node = bitree_right(node);
      direction = 2;
    }
  }
  if ((data = (int *) malloc(sizeof (int))) == NULL)
    return -1;
  *data = i;
  if (direction == 0)
    return bitree_ins_left(tree, NULL, data);
  if (direction == 1)
    return bitree_ins_left(tree, prev, data);
  if (direction == 2)
    return bitree_ins_right(tree, prev, data);
  return -1;
}

/* 查找整数 */
static BiTreeNode *search_int (BiTree * tree, int i) {
  BiTreeNode *node;
  /* 作为二叉查找树来查找结点 */
  node = bitree_root(tree);
  while (!bitree_is_eob(node)) {
    if (i == *(int *) bitree_data(node)) {
      return node;
    } else if (i < *(int *) bitree_data(node)) {
      node = bitree_left(node);
    } else {
      node = bitree_right(node);
    }
  }
  return NULL;
}

/* 主函数 */
int main (int argc, char **argv) {
  BiTree tree;
  BiTreeNode *node;
  int i;

  printf("初始化链表\n");
  bitree_init(&tree, free);
  printf("插入一些结点\n");
  int a[10] = {20, 10, 30, 15, 25, 70, 80, 23, 26, 5};
  for (i = 0; i < 10; i++) {
    if (insert_int(&tree, a[i]) != 0)
      return 1;
  }
  printf("树大小: %d\n", bitree_size(&tree));
  printf("(先序遍历)\n");
  print_preorder(bitree_root(&tree));

  i = 30;
  if ((node = search_int(&tree, i)) == NULL) {
    printf("找不到 %d\n", i);
  } else {
    printf("找到 %d...删除其左子树\n", i);
    bitree_rem_left(&tree, node);
    printf("树大小: %d\n", bitree_size(&tree));
    printf("(先序遍历)\n");
    print_preorder(bitree_root(&tree));
  }

  i = 99;
  if ((node = search_int(&tree, i)) == NULL) {
    printf("找不到 %d\n", i);
  } else {
    printf("找到 %d...删除其右子树\n", i);
    bitree_rem_right(&tree, node);
    printf("树大小: %d\n", bitree_size(&tree));
    printf("(先序遍历)\n");
    print_preorder(bitree_root(&tree));
  }

  i = 20;
  if ((node = search_int(&tree, i)) == NULL) {
    printf("找不到 %d\n", i);
  } else {
    printf("找到 %d...删除其右子树\n", i);
    bitree_rem_right(&tree, node);
    printf("树大小: %d\n", bitree_size(&tree));
    printf("(先序遍历)\n");
    print_preorder (bitree_root(&tree));
  }

  i = bitree_is_leaf(bitree_root(&tree));
  printf("测试 bitree_is_leaf...值=%d (应为0)\n", i);
  i = bitree_is_leaf(bitree_left((bitree_root(&tree))));
  printf("测试 bitree_is_leaf...值=%d (应为0)\n", i);
  i = bitree_is_leaf(bitree_left(bitree_left((bitree_root(&tree)))));
  printf("测试 bitree_is_leaf...值=%d (应为1)\n", i);
  i = bitree_is_leaf(bitree_right(bitree_left((bitree_root(&tree)))));
  printf("测试 bitree_is_leaf...值=%d (应为1)\n", i);

  printf("插入一些结点\n");
  int b[4] = {55, 44, 77, 11};
  for (i = 0; i < 4; i++) {
    if (insert_int(&tree, b[i]) != 0)
      return 1;
  }
  printf("树大小: %d\n", bitree_size(&tree));
  printf("(先序遍历)\n");
  print_preorder(bitree_root(&tree));
  printf("(中序遍历)\n");
  print_inorder(bitree_root(&tree));
  printf("(后序遍历)\n");
  print_postorder(bitree_root(&tree));

  printf("销毁二叉树\n");
  bitree_destroy(&tree);
  return 0;
}
