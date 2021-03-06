# C

1. 如何求 3.14 除以 2.1 的余数？

    double remainder = fmod(3.14, 2.1);

2. 声明语句 `extern int fun();` 和 `int fun();` 的区别在于？

    没有区别，只是前者可能在另一个文件中。

3. 在函数 `int execv(const char *path, char *const argv[]);` 中对参数合法和非法的操作都有哪些？举一些例子。

    path[2] = "e";    // 非法
    path = "/bin/sh"; // 合法
    argv[1] = NULL;   // 非法
    argv[0][0] = 'a'; // 合法

4. 编写一个宏，交换两个相同类型的变量值。

    #define swap(a, b, c) {c t = a; a = b; b = t;}

5. `NULL` 在哪个头文件中？被定义为什么？

    <stddef.h> <stdio.h> <stdlib.h> <string.h> <time.h> 之一
    #define NULL ((void*)0)

6. 用星号和括号展开 `weekdays[now->tm_wday][1]`。

    *(*(weekdays + (*now).tm_wday) + 1)

7. 如何避免头文件重复包含？如何避免变量重复定义？

----

    #ifndef _HEADER_
    #define _HEADER_
    #endif

8. `<string.h>` 中，哪些函数是不安全的？如何解决？

----

    strcat strcmp strcpy
    分别使用安全的 strncat strncmp strncpy

9. 编写一个宏，向标准错误输出中打印变量名和其地址。如果要打印其值怎么办？

----

    #define p(v, format) fprintf(stderr, "%s[%p] = " format "\n", #v, &v, v)

10. 如何判断系统平台是用大端法还是小端法在内存中存储数据的？写出实现的代码。

----

    union {
        short s;
        char c[sizeof(short)];
    } un;

    un.s = 0x0102;
    if (sizeof(short) == 2) {
        if (un.c[0] == 1 && un.c[1] == 2)
            printf("big-endian\n");
        else if (un.c[0] == 2 && un.c[1] == 1)
            printf("little-endian\n");
        else
            printf("unknown\n");
    }

11. 用几种不同的方法申请一个 M*N 大小的 time_t 类型的数组。

----

    time_t ** a = (time_t ** ) malloc(M * N * sizeof(time_t));
    if (a == NULL) exit(EXIT_FAILURE);
    free(a);
    time_t ** b = (time_t ** ) calloc(M * N, sizeof(time_t));
    if (b == NULL) exit(EXIT_FAILURE);
    free(b);

12. 生成 [a, b) 的左闭右开区间上均匀分布的伪随机数。

----

    return (double) rand() / RAND_MAX * (b - a) + a;

13. 实现计算参数和的函数`int sum(int n, ...)`。

----

    #include <stdarg.h>
    int sum(int n, ...) {
        int s = 0;
        va_list ptr;
        va_start(ptr, n);
        for (int i = 0; i < n; i++) {
          s += va_arg(ptr, int);
        }
        va_end(ptr);
        return s;
    }

14. 实现`SList`类的`slist_foreach`方法，该方法遍历链表，对每个元素应用回调函数。

    typedef struct slist {
      struct slist *next;
      const void *userdata;
    } SList;
    typedef void *SListCallback(SList *item, void *userdata);
    void *slist_foreach(SList *slist, SListCallback *foreach, void *userdata);

----

    void *slist_foreach(SList *slist, SListCallback *foreach, void *userdata)
    {
        void *result = 0;
        assert(foreach);
        while (slist) {
            SList *next = slist->next;
            result = (*foreach) (slist, userdata);
            if (result) break;
            slist = next;
        }
        return result;
    }

15. 指出`"unp.h"`中包含哪些头文件和函数定义，试实现其中一个函数。

    #include "unp.h"

    int main(int argc, char** argv)
    {
        int listenfd, connfd;
        pid_t childpid;
        socklen_t clilen;
        struct sockaddr_in cliaddr, servaddr;
        void sig_chld(int);

        listenfd = Socket(AF_INET, SOCK_STREAM, 0);

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(SERV_PORT);

        Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

        Listen(listenfd, LISTENQ);

        Signal(SIGCHLD, sig_chld);

        for (;;) {
            clilen = sizeof(cliaddr);
            if ((connfd = accept(listenfd, (SA*)&cliaddr, &clilen)) < 0) {
                if (errno == EINTR)
                    continue; /* back to for() */
                else
                    err_sys("accept error");
            }

            if ((childpid = Fork()) == 0) { /* child process */
                Close(listenfd); /* close listening socket */
                str_echo(connfd); /* process the request */
                exit(0);
            }
            Close(connfd); /* parent closes connected socket */
        }
    }

----

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <netdb.h>

    int Socket(int family, int type, int protocol)
    {
        int n;

        if ((n = socket(family, type, protocol)) < 0)
            err_sys("socket error");
        return (n);
    }

16. 写一个程序，通过命令行传递两个整数，返回两数之和。处理出现的异常情况。

----

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <errno.h>
  #include <limits.h>

  int parse_int(char const *s) {
      char *p = NULL;
      long int a = strtol(s, &p, 10);
      if (a == 0 && p == s) {
          fprintf(stderr, "无效字符\n");
          exit(EXIT_FAILURE);
      }
      if (errno == ERANGE || a > INT_MAX || a < INT_MIN) {
          fprintf(stderr, "范围溢出\n");
          exit(EXIT_FAILURE);
      }
      if (errno) {
          fprintf(stderr, "转换失败\n");
          exit(EXIT_FAILURE);
      }
      return (int) a;
  }

  int main(int argc, char const *argv[]) {
      if (argc < 3) {
          fprintf(stderr, "用法: %s <加数> <加数>\n", argv[0]);
          exit(EXIT_FAILURE);
      }
      int a = parse_int(argv[1]);
      int b = parse_int(argv[2]);
      if (a > INT_MAX - b) {
          fprintf(stderr, "加法溢出\n");
          exit(EXIT_FAILURE);
      }
      int c = a + b;
      return c;
  }

17. 一颗二叉查找树T中，把一棵子树u归并到另一棵子树v中，u的父亲变为v的父亲。

----

    void Transplant(Tree *T, Tree *u, Tree *v) {
        if (u->father == NULL) {
            T = v;
        } else if (u == u->father->left) {
            u->father->left = v;
        } else {
            u->father->right = v;
        }
        if (v != NULL) {
            v->father = u->father;
        }
    }

18. 以下代码输出什么？试分解第3行代码。

    int arr[3] = {10, 20, 30};
    int x = 0;
    x = ++arr[++x] + ++x + arr[--x];
    printf("%d\n", x);

----

    44

    ++x;
    int a = ++arr[x];
    int b = ++x;
    --x;
    int c = arr[x];
    x = a + b + c;

















结尾
