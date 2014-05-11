/* 
 * 文件: traverse.h
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月11日
 * 修改时间: 2014年5月11日
 * 描述: 二叉树遍历头文件
 */


#ifndef TRAVERSE_H
#define TRAVERSE_H

#include "bitree.h"
#include "list.h"

/* 公共接口 */

int preorder(const BiTreeNode *node, List *list);

int inorder(const BiTreeNode *node, List *list);

int postorder(const BiTreeNode *node, List *list);

#endif
