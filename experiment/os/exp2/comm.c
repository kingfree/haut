#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 512

int main(int argc, char *argv[])
{
    int n;
    int fd[2];
    pid_t pid;
    char *s = "Child process 1 is sending a message!\n";
    char line[MAXLINE];

    if (pipe(fd) < 0) {
        fprintf(stderr, "创建管道失败\n");
        exit(1);
    }
    
    if ((pid = fork()) < 0) {
        fprintf(stderr, "创建子进程失败\n");
        exit(1);
    } else if (pid == 0) {      /* 子进程 */
        close(fd[0]);
        write(fd[1], s, strlen(s));
    } else {                    /* 父进程 */
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }

    exit(0);
}

