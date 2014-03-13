/* 文件: status.h
 * 作者: 田劲锋
 * 创建: 2014年3月3日
 * 修改: 2014年3月13日
 * 描述: 《数据结构》头文件
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {Ok, Error, Infeasible, Overflow, NotFound} Status; // 状态的枚举类型

// 元素类型和相关操作
typedef int ElemType;

int cmp(const ElemType a, const ElemType b) {
  return a - b;
}

char *ev(ElemType *x) {
  static char s[1 << 20];
  sprintf(s, "%d", *x);
  return s;
}
