/* 
 * 文件: stack.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月10日
 * 修改时间: 2014年5月10日
 * 描述: 栈实现
 */

#include <stdlib.h>

#include "list.h"
#include "stack.h"

/* 压栈 */
int stack_push(Stack * stack, const void *data) {
  /* 插入尾部 */
  return list_ins_next(stack, NULL, data);
}

/* 出栈 */
int stack_pop(Stack * stack, void **data) {
  /* 删除首部 */
  return list_rem_next(stack, NULL, data);
}

