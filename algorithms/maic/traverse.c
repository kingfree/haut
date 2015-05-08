/* 
 * 文件: traverse.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月11日
 * 修改时间: 2014年5月11日
 * 描述: 二叉树遍历的实现
 */

#include "list.h"
#include "traverse.h"

/* 先序遍历 */
int preorder(const BiTreeNode *node, List *list) {
  if (!bitree_is_eob(node)) {
    /* 根 */
    if (list_ins_next(list, list_tail(list), bitree_data(node)) != 0)
      return -1;
    /* 左 */
    if (!bitree_is_eob(bitree_left(node)))
      if (preorder(bitree_left(node), list) != 0)
        return -1;
    /* 右 */
    if (!bitree_is_eob(bitree_right(node)))
      if (preorder(bitree_right(node), list) != 0)
        return -1;
  }
  return 0;
}

/* 中序遍历 */
int inorder(const BiTreeNode *node, List *list) {
  if (!bitree_is_eob(node)) {
    /* 左 */
    if (!bitree_is_eob(bitree_left(node)))
      if (inorder(bitree_left(node), list) != 0)
        return -1;
    /* 根 */
    if (list_ins_next(list, list_tail(list), bitree_data(node)) != 0)
      return -1;
    /* 右 */
    if (!bitree_is_eob(bitree_right(node)))
      if (inorder(bitree_right(node), list) != 0)
        return -1;
  }
  return 0;
}

/* 后序遍历 */
int postorder(const BiTreeNode *node, List *list) {
  if (!bitree_is_eob(node)) {
    /* 左 */
    if (!bitree_is_eob(bitree_left(node)))
      if (postorder(bitree_left(node), list) != 0)
        return -1;
    /* 右 */
    if (!bitree_is_eob(bitree_right(node)))
      if (postorder(bitree_right(node), list) != 0)
        return -1;
    /* 根 */
    if (list_ins_next(list, list_tail(list), bitree_data(node)) != 0)
      return -1;
  }
  return 0;
}
