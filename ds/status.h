/* 文件: status.h
 * 作者: 田劲锋
 * 创建: 2014年3月3日
 * 修改: 2014年3月13日
 * 描述: 《数据结构》课程预定义常量和状态头文件
 *      为了简便起见，使用 C/C++ 混合而非 ANSI C
 */

#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>

enum Status {Ok, Error, Infeasible, Overflow, NotFound}; // 状态的枚举类型

typedef int ElemType;

int cmp(const ElemType a, const ElemType b) {
  return a - b;
}

char *elemvisit(ElemType *x) {
  static char s[1 << 20];
  sprintf(s, "%d", *x);
  return s;
}

