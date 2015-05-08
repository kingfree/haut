/* get_num.c

   命令行参数数字化函数。
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "get_num.h"

/* 打印函数名('fname')、参数值('arg')、参数名('name')和错误信息('msg') */
static void
gnFail(const char *fname, const char *msg, const char *arg,
       const char *name)
{
    fprintf(stderr, "%s 错误", fname);
    if (name != NULL)
        fprintf(stderr, " (在 %s)", name);
    fprintf(stderr, ": %s\n", msg);
    if (arg != NULL && *arg != '\0')
        fprintf(stderr, "        错误参数: %s\n", arg);
    exit(EXIT_FAILURE);
}

/* 把命令行参数 ('arg') 转成长整数。
   'flags' 使用 get_num.h 中的常量表示进制等。

   'fname' 是调用者的名字，'name' 是参数 'arg' 的名字。
   'fname' 和 'name' 用于 'arg' 出错时打印错误信息。 */
static long
getNum(const char *fname, const char *arg, int flags, const char *name)
{
    long res;
    char *endptr;
    int base;
    if (arg == NULL || *arg == '\0')
        gnFail(fname, "空字串", arg, name);
    base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 :
        (flags & GN_BASE_16) ? 16 : 10;
    errno = 0;
    res = strtol(arg, &endptr, base);
    if (errno != 0)
        gnFail(fname, "strtol() 失败", arg, name);
    if (*endptr != '\0')
        gnFail(fname, "非数字字符", arg, name);
    if ((flags & GN_NONNEG) && res < 0)
        gnFail(fname, "不允许负数", arg, name);
    if ((flags & GN_GT_0) && res <= 0)
        gnFail(fname, "值必须 > 0", arg, name);
    return res;
}

/* 调用 getNum() 转换成长整数 */
long getLong(const char *arg, int flags, const char *name)
{
    return getNum("getLong", arg, flags, name);
}

/* 调用 getNum() 转换成整数 */
int getInt(const char *arg, int flags, const char *name)
{
    long res;
    res = getNum("getInt", arg, flags, name);
    if (res > INT_MAX || res < INT_MIN)
        gnFail("getInt", "整数超出范围", arg, name);
    return (int) res;
}
