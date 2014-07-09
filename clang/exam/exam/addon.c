/* addon.h
 * ���Ӻ�����
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "addon.h"

#ifdef WIN32
#include <conio.h>
#else
#include <termios.h>
#endif

void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

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

int random(int a, int b)
/* ���� [a, b) ֮�������� */
{
    return rand() % (b - a) + a;
}

#define PWD_MAX 256

char *getpass(char *prompt)
{
    static char passwd[PWD_MAX] = "";
    printf("%s", prompt);
#ifdef WIN32
    int i = 0;
    char c;
    while ((c = _getch()) != '\n' && c != '\r' && i < PWD_MAX) {
        if (c == '\b' && i > 0) {
            passwd[i--] = '\0';
            printf("\b \b");
        } else {
            passwd[i++] = c;
            printf("*");
        }
    }
    passwd[i] = '\0';
#else
    struct termios oldflags, newflags;
    tcgetattr(fileno(stdin), &oldflags);
    newflags = oldflags;
    newflags.c_lflag &= ~ECHO;
    newflags.c_lflag |= ECHONL;
    if (tcsetattr(fileno(stdin), TCSANOW, &newflags) != 0) {
        perror("tcsetattr");
        return NULL;
    }
    fgets(passwd, PWD_MAX, stdin);
    if (tcsetattr(fileno(stdin), TCSANOW, &oldflags) != 0) {
        perror("tcsetattr");
        return NULL;
    }
#endif
    return passwd;
}
