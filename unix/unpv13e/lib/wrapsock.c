/*
 * 套接字包装函数。
 * These could all go into separate files, so only the ones needed cause
 * the corresponding function to be added to the executable.  If sockets
 * are a library (SVR4) this might make a difference (?), but if sockets
 * are in the kernel (BSD) it doesn't matter.
 *
 * These wrapper functions also use the same prototypes as POSIX.1g,
 * which might differ from many implementations (i.e., POSIX.1g specifies
 * the fourth argument to getsockopt() as "void *", not "char *").
 *
 * If your system's headers are not correct [i.e., the Solaris 2.5
 * <sys/socket.h> omits the "const" from the second argument to both
 * bind() and connect()], you'll get warnings of the form:
 *warning: passing arg 2 of `bind' discards `const' from pointer target type
 *warning: passing arg 2 of `connect' discards `const' from pointer target type
 */

#include "unp.h"

int Accept(int fd, struct sockaddr* sa, socklen_t* salenptr)
{
    int n;

again:
    if ((n = accept(fd, sa, salenptr)) < 0) {
#ifdef EPROTO
        if (errno == EPROTO || errno == ECONNABORTED)
#else
        if (errno == ECONNABORTED)
#endif
            goto again;
        else
            err_sys("accept 错误");
    }
    return (n);
}

void Bind(int fd, const struct sockaddr* sa, socklen_t salen)
{
    if (bind(fd, sa, salen) < 0)
        err_sys("bind 错误");
}

void Connect(int fd, const struct sockaddr* sa, socklen_t salen)
{
    if (connect(fd, sa, salen) < 0)
        err_sys("connect 错误");
}

void Getpeername(int fd, struct sockaddr* sa, socklen_t* salenptr)
{
    if (getpeername(fd, sa, salenptr) < 0)
        err_sys("getpeername 错误");
}

void Getsockname(int fd, struct sockaddr* sa, socklen_t* salenptr)
{
    if (getsockname(fd, sa, salenptr) < 0)
        err_sys("getsockname 错误");
}

void Getsockopt(int fd, int level, int optname, void* optval, socklen_t* optlenptr)
{
    if (getsockopt(fd, level, optname, optval, optlenptr) < 0)
        err_sys("getsockopt 错误");
}

#ifdef HAVE_INET6_RTH_INIT
int Inet6_rth_space(int type, int segments)
{
    int ret;

    ret = inet6_rth_space(type, segments);
    if (ret < 0)
        err_quit("inet6_rth_space 错误");

    return ret;
}

void* Inet6_rth_init(void* rthbuf, socklen_t rthlen, int type, int segments)
{
    void* ret;

    ret = inet6_rth_init(rthbuf, rthlen, type, segments);
    if (ret == NULL)
        err_quit("inet6_rth_init 错误");

    return ret;
}

void Inet6_rth_add(void* rthbuf, const struct in6_addr* addr)
{
    if (inet6_rth_add(rthbuf, addr) < 0)
        err_quit("inet6_rth_add 错误");
}

void Inet6_rth_reverse(const void* in, void* out)
{
    if (inet6_rth_reverse(in, out) < 0)
        err_quit("inet6_rth_reverse 错误");
}

int Inet6_rth_segments(const void* rthbuf)
{
    int ret;

    ret = inet6_rth_segments(rthbuf);
    if (ret < 0)
        err_quit("inet6_rth_segments 错误");

    return ret;
}

struct in6_addr*
Inet6_rth_getaddr(const void* rthbuf, int idx)
{
    struct in6_addr* ret;

    ret = inet6_rth_getaddr(rthbuf, idx);
    if (ret == NULL)
        err_quit("inet6_rth_getaddr 错误");

    return ret;
}
#endif

#ifdef HAVE_KQUEUE
int Kqueue(void)
{
    int ret;

    if ((ret = kqueue()) < 0)
        err_sys("kqueue 错误");
    return ret;
}

int Kevent(int kq, const struct kevent* changelist, int nchanges,
    struct kevent* eventlist, int nevents, const struct timespec* timeout)
{
    int ret;

    if ((ret = kevent(kq, changelist, nchanges,
             eventlist, nevents, timeout)) < 0)
        err_sys("kevent 错误");
    return ret;
}
#endif

/* include Listen */
void Listen(int fd, int backlog)
{
    char* ptr;

    /* 如果可能的话用环境变量替换第二个参数 */
    if ((ptr = getenv("LISTENQ")) != NULL)
        backlog = atoi(ptr);

    if (listen(fd, backlog) < 0)
        err_sys("listen 错误");
}
/* end Listen */

#ifdef HAVE_POLL
int Poll(struct pollfd* fdarray, unsigned long nfds, int timeout)
{
    int n;

    if ((n = poll(fdarray, nfds, timeout)) < 0)
        err_sys("poll 错误");

    return (n);
}
#endif

ssize_t
Recv(int fd, void* ptr, size_t nbytes, int flags)
{
    ssize_t n;

    if ((n = recv(fd, ptr, nbytes, flags)) < 0)
        err_sys("recv 错误");
    return (n);
}

ssize_t
Recvfrom(int fd, void* ptr, size_t nbytes, int flags,
    struct sockaddr* sa, socklen_t* salenptr)
{
    ssize_t n;

    if ((n = recvfrom(fd, ptr, nbytes, flags, sa, salenptr)) < 0)
        err_sys("recvfrom 错误");
    return (n);
}

ssize_t
Recvmsg(int fd, struct msghdr* msg, int flags)
{
    ssize_t n;

    if ((n = recvmsg(fd, msg, flags)) < 0)
        err_sys("recvmsg 错误");
    return (n);
}

int Select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
    struct timeval* timeout)
{
    int n;

    if ((n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
        err_sys("select 错误");
    return (n); /* can return 0 on timeout */
}

void Send(int fd, const void* ptr, size_t nbytes, int flags)
{
    if (send(fd, ptr, nbytes, flags) != (ssize_t)nbytes)
        err_sys("send 错误");
}

void Sendto(int fd, const void* ptr, size_t nbytes, int flags,
    const struct sockaddr* sa, socklen_t salen)
{
    if (sendto(fd, ptr, nbytes, flags, sa, salen) != (ssize_t)nbytes)
        err_sys("sendto 错误");
}

void Sendmsg(int fd, const struct msghdr* msg, int flags)
{
    unsigned int i;
    ssize_t nbytes;

    nbytes = 0; /* must first figure out what return value should be */
    for (i = 0; i < msg->msg_iovlen; i++)
        nbytes += msg->msg_iov[i].iov_len;

    if (sendmsg(fd, msg, flags) != nbytes)
        err_sys("sendmsg 错误");
}

void Setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen)
{
    if (setsockopt(fd, level, optname, optval, optlen) < 0)
        err_sys("setsockopt 错误");
}

void Shutdown(int fd, int how)
{
    if (shutdown(fd, how) < 0)
        err_sys("shutdown 错误");
}

int Sockatmark(int fd)
{
    int n;

    if ((n = sockatmark(fd)) < 0)
        err_sys("sockatmark 错误");
    return (n);
}

/* include Socket */
int Socket(int family, int type, int protocol)
{
    int n;

    if ((n = socket(family, type, protocol)) < 0)
        err_sys("socket 错误");
    return (n);
}
/* end Socket */

void Socketpair(int family, int type, int protocol, int* fd)
{
    int n;

    if ((n = socketpair(family, type, protocol, fd)) < 0)
        err_sys("socketpair 错误");
}
