/* addon.h
* ���Ӻ�����
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "addon.h"

bool gotn()
/* ���������Ļس����з� */
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
/* ��ѯ���� */
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
/* �������Ƿ�Ϊ�� */
{
    return fabs(d - 0) <= 1e-6;
}

bool psame(int n, double d)
/* ������ d ��С��λ�Ƿ�������� n */
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
