#include "apue.h"

#define BUFFSIZE 4096

int main()
{
    int n;
    char buf[BUFFSIZE];

    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("写出错误");

    if (n < 0)
        err_sys("读取错误");

    exit(0);
}

