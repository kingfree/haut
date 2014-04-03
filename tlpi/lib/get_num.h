/* get_num.h

   get_num.c 的头文件
*/
#ifndef GET_NUM_H
#define GET_NUM_H

#define GN_NONNEG       01      /* 值必须 >= 0 */
#define GN_GT_0         02      /* 值必须 > 0 */

                                /* 默认整数是十进制的 */
#define GN_ANY_BASE   0100      /* 可以使用任何进制，比如 strtol(3) */
#define GN_BASE_8     0200      /* 值以八进制表示 */
#define GN_BASE_16    0400      /* 值以十六进制表示 */

long getLong(const char *arg, int flags, const char *name);

int getInt(const char *arg, int flags, const char *name);

#endif
