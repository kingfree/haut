#include "apue.h"
#include <sys/wait.h>

int main()
{
    char buf[MAXLINE];
    pid_t pid;
    int status;

    printf("%% ");
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = 0;

        if ((pid = fork()) < 0) {
            err_sys("fork 错误");
        } else if (pid == 0) {      /* 子进程 */
            execlp(buf, buf, (char *) 0);
            err_ret("无法执行: %s", buf);
            exit(127);
        }

        /* 父进程 */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid 错误");
        printf("%% ");
    }
    exit(0);
}

