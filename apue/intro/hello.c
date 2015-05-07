#include "apue.h"

int main()
{
    printf("你好，世界！\n来自进程 %ld\n", (long) getpid());

    exit(0);
}

