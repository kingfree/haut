/* 
 * 文件: stack.h
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月10日
 * 修改时间: 2014年5月10日
 * 描述: 栈头文件
 */

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#include "list.h"

/* 栈其实是链表 */

typedef List Stack;

/* 公共接口 */

#define stack_init list_init

#define stack_destroy list_destroy

int stack_push(Stack * stack, const void *data);

int stack_pop(Stack * stack, void **data);

#define stack_peek(stack) ((stack)->head == NULL ? NULL : (stack)->head->data)

#define stack_size list_size

#endif
