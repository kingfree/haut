
void Bind(int fd, const struct sockaddr* sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0)
        err_sys("bind 错误");
}

int Socket(int family, int type, int protocol)
{
    int n;

    if ((n = socket(family, type, protocol)) < 0)
        err_sys("socket 错误");
    return (n);
}

void Listen(int fd, int backlog)
{
    char* ptr;

    /* 如果可能的话用环境变量替换第二个参数 */
    if ((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);

    if (listen(fd, backlog) < 0)
        err_sys("listen 错误");
}

void Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close error");
}

Sigfunc*
signal(int signo, Sigfunc* func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT; /* SunOS 4.x */
#endif
    }
    else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART; /* SVR4, 44BSD */
#endif
    }
    if (sigaction(signo, &act, &oact) < 0)
        return (SIG_ERR);
    return (oact.sa_handler);
}
/* end signal */

Sigfunc*
Signal(int signo, Sigfunc* func) /* 包装我们的 signal() 函数 */
{
    Sigfunc* sigfunc;

    if ((sigfunc = signal(signo, func)) == SIG_ERR)
        err_sys("signal 错误");
    return (sigfunc);
}
