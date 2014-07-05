/* addon.h
* 附加函数库
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "addon.h"

bool gotn()
/* 清理遗留的回车换行符 */
{
    char c;
    c = getchar();
    if (c == '\n' || c == '\r') {
        return true;
    }
    ungetc(c, stdin);
    return false;
}

bool select_cond_number(sel_num cond, int data)
/* 查询条件 */
{
    //fprintf(stderr, "%d - [%d %s]\n", data, cond.num, cond.mark);
    if (strcmp(cond.mark, "<") == 0) {
        return data < cond.num;
    } else if (strcmp(cond.mark, ">") == 0) {
        return data > cond.num;
    } else if (strcmp(cond.mark, "<=") == 0) {
        return data <= cond.num;
    } else if (strcmp(cond.mark, ">=") == 0) {
        return data >= cond.num;
    } else if (strcmp(cond.mark, "!=") == 0 || strcmp(cond.mark, "<>") == 0) {
        return data != cond.num;
    } else { // "==" "=" ""
        return data == cond.num;
    }
}

bool zero(double d)
/* 浮点数是否为零 */
{
    return fabs(d - 0) <= 1e-6;
}

bool psame(int n, double d)
/* 浮点数 d 的小数位是否等于整数 n */
{
    int i = 0;
    double m = n;
    for (i = 0; i < 6; i++) {
        if (zero((m /= 10) - d)) {
            return true;
        }
    }
    return false;
}
