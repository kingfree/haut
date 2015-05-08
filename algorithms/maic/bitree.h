/* 
 * 文件: bitree.h
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月11日
 * 修改时间: 2014年5月11日
 * 描述: 二叉树头文件
 */

#ifndef BITREE_H
#define BITREE_H

#include <stdlib.h>

/* 定义二叉树结点 */
typedef struct BiTreeNode_ {
  void *data;
  struct BiTreeNode_ *left;
  struct BiTreeNode_ *right;
} BiTreeNode;

/* 定义二叉树 */
typedef struct BiTree_ {
  int size;
  int (*compare)(const void *key1, const void *key2);
  void (*destroy)(void *data);
  BiTreeNode *root;
} BiTree;

/* 公共接口 */

void bitree_init(BiTree *tree, void (*destroy)(void *data));

void bitree_destroy(BiTree *tree);

int bitree_ins(BiTree *tree, BiTreeNode *node, const void *data, int left);

#define bitree_ins_left(tree, node, data) bitree_ins(tree, node, data, 1)

#define bitree_ins_right(tree, node, data) bitree_ins(tree, node, data, 0)

void bitree_rem(BiTree *tree, BiTreeNode *node, int left);

#define bitree_rem_left(tree, node) bitree_rem(tree, node, 1)

#define bitree_rem_right(tree, node) bitree_rem(tree, node, 0)

int bitree_merge(BiTree *merge, BiTree *left, BiTree *right, const void *data);

#define bitree_size(tree) ((tree)->size)

#define bitree_root(tree) ((tree)->root)

#define bitree_is_eob(node) ((node) == NULL)

#define bitree_is_leaf(node) ((node)->left == NULL && (node)->right == NULL)

#define bitree_data(node) ((node)->data)

#define bitree_left(node) ((node)->left)

#define bitree_right(node) ((node)->right)

#endif
