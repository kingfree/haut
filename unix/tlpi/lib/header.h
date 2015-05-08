/* header.h

   用于几乎所有例程的标准头文件。
*/
#ifndef HEADER_H
#define HEADER_H                /* 防止多重包含 */

#include <sys/types.h>          /* 类型定义 */
#include <stdio.h>              /* 标准 I/O 函数 */
#include <stdlib.h>             /* 通用库函数原型，
                                   包括 EXIT_SUCCESS 和 EXIT_FAILURE 常量 */
#include <unistd.h>             /* 系统调用函数原型 */
#include <errno.h>              /* 声明 errno 以及定义错误常量 */
#include <string.h>             /* 通用字符串操作函数 */

#include "get_num.h"            /* 声明数字处理函数 (getInt(), getLong()) */

#include "error_functions.h"    /* 声明错误处理函数 */

typedef enum { false, true } bool;  /* C++ 式的布尔类型 */

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif
