/* 
 * 文件: stack_demo.c
 * 作者: 田劲锋 <anytjf@gmail.com>
 * 创建时间: 2014年5月10日
 * 修改时间: 2014年5月10日
 * 描述: 栈操作的演示
 */

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "stack.h"

/* 打印栈 */
static void print_stack(const Stack * stack) {
  ListElmt *element;
  int *data, size, i;
  printf("  栈大小: %d\n  ", size = list_size(stack));
  element = list_head(stack);
  for (i = 0; i < size; i++) {
    data = list_data(element);
    printf("%d ", *data);
    element = list_next(element);
  }
  printf("\n");
}

/* 主函数 */
int main(int argc, char **argv) {
  Stack stack;
  int *data, i;

  printf("初始化栈\n");
  stack_init(&stack, free);
  print_stack(&stack);

  printf("压入 10 个元素\n");
  for (i = 0; i < 10; i++) {
    if ((data =(int *) malloc(sizeof(int))) == NULL)
      return 1;
    *data = i + 1;
    if (stack_push(&stack, data) != 0)
      return 1;
  }
  print_stack(&stack);

  printf("弹出 5 个元素\n");
  for (i = 0; i < 5; i++) {
    if (stack_pop(&stack, (void **) &data) == 0)
      free(data);
    else
      return 1;
  }
  print_stack(&stack);

  printf("压入 100 和 200\n");
  if ((data =(int *) malloc(sizeof(int))) == NULL)
    return 1;
  *data = 100;
  if (stack_push(&stack, data) != 0)
    return 1;
  if ((data =(int *) malloc(sizeof(int))) == NULL)
    return 1;
  *data = 200;
  if (stack_push(&stack, data) != 0)
    return 1;
  print_stack(&stack);

  if ((data = stack_peek(&stack)) != NULL)
    printf("检查栈顶...值=%d\n", *data);
  else
    printf("检查栈顶...值=NULL\n");
  print_stack(&stack);

  printf("弹出所有元素\n");
  while (stack_size(&stack) > 0) {
    if (stack_pop(&stack, (void **) &data) == 0)
      free(data);
  }
  if ((data = stack_peek(&stack)) != NULL)
    printf("检查空栈...值=%d\n", *data);
  else
    printf("检查空栈...值=NULL\n");
  print_stack(&stack);

  printf("销毁栈\n");
  stack_destroy(&stack);

  return 0;

}
