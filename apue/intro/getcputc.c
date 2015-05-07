#include "apue.h"

int main()
{
    int c;

    while ((c = getc(stdin)) != EOF)
        if (putc(c, stdout) == EOF)
            err_sys("输出错误");

    if (ferror(stdin))
        err_sys("输入错误");

    exit(0);
}

