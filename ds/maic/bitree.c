/* 
 * 文件: bitree.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月11日
 * 修改时间: 2014年5月11日
 * 描述: 二叉树的实现
 */

#include <stdlib.h>
#include <string.h>

#include "bitree.h"

/* 初始化二叉树 */
void bitree_init(BiTree *tree, void (*destroy)(void *data)) {
  tree->size = 0;
  tree->destroy = destroy;
  tree->root = NULL;
}

/* 销毁二叉树 */
void bitree_destroy(BiTree *tree) {
  /* 从根删除树上所有结点 */
  bitree_rem_left(tree, NULL);
  /* 保险起见，清空结构体 */
  memset(tree, 0, sizeof(BiTree));
}

/* 插入结点 */
int bitree_ins(BiTree *tree, BiTreeNode *node, const void *data, int left) {
  BiTreeNode *new_node, **position;
  /* 定位 */
  if (node == NULL) {
    /* 只有空树才能插入在根上 */
    if (bitree_size(tree) > 0)
      return -1;
    position = &tree->root;
  } else {
    /* 必须插在末端分支 */
    if (left) {
      /* 插入在左端 */
      if (bitree_left(node) != NULL)
        return -1;
      position = &node->left;
    } else {
      /* 插入在右端 */
      if (bitree_right(node) != NULL)
        return -1;
      position = &node->right;
    }
  }
  /* 分配存储空间 */
  if ((new_node = (BiTreeNode *)malloc(sizeof(BiTreeNode))) == NULL)
    return -1;
  /* 插入结点 */
  new_node->data = (void *)data;
  new_node->left = NULL;
  new_node->right = NULL;
  *position = new_node;
  /* 树大小自增 */
  tree->size++;
  return 0;
}

/* 删除结点 */
void bitree_rem(BiTree *tree, BiTreeNode *node, int left) {
  BiTreeNode **position;
  /* 不允许删除空树上的结点 */
  if (bitree_size(tree) == 0)
    return;
  /* 定位 */
  if (node == NULL)
    position = &tree->root;
  else if (left)
    position = &node->left;
  else
    position = &node->right;
  /* 删除结点 */
  if (*position != NULL) {
    bitree_rem_left(tree, *position);
    bitree_rem_right(tree, *position);
    if (tree->destroy != NULL) {
      /* 调用用户函数销毁结点数据空间 */
      tree->destroy((*position)->data);
    }
    free(*position);
    *position = NULL;
    /* 树大小自减 */
    tree->size--;
  }
}

/* 合并二叉树 */
int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void
    *data) {
  /* 初始化目标树 */
  bitree_init(merge, left->destroy);
  /* 把数据插入到根节点 */
  if (bitree_ins_left(merge, NULL, data) != 0) {
    bitree_destroy(merge);
    return -1;
  }
  /* 合并两棵二叉树 */
  bitree_root(merge)->left = bitree_root(left);
  bitree_root(merge)->right = bitree_root(right);
  /* 树大小改变 */
  merge->size = merge->size + bitree_size(left) + bitree_size(right);
  /* 原来的树不再指向已合并的结点 */
  left->root = NULL;
  left->size = 0;
  right->root = NULL;
  right->size = 0;
  return 0;
}
