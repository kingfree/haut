#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid1, pid2;

    if ((pid1 = fork()) < 0) {
        fprintf(stderr, "创建子进程1失败\n");
        exit(1);
    } else if (pid1 == 0) {     /* 子进程1 */
        printf("This is child1 (pid1=%ld) process: B\n",
                (long) getpid());
    } else {                    /* 父进程 */
        printf("Parent Process: A\n");

        if ((pid2 = fork()) < 0) {
            fprintf(stderr, "创建子进程2失败\n");
            exit(1);
        } else if (pid2 == 0) { /* 子进程2 */
            printf("This is child2 (pid2=%ld) process: C\n",
                    (long) getpid());
        }
    }

    exit(0);
}

