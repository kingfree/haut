#include "unp.h"

void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    pid = wait(&stat);
    printf("子进程 %d 结束\n", pid);
    return;
}
