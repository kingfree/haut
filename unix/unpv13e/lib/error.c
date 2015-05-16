#include	"unp.h"

#include	<stdarg.h>		/* ANSI C 头文件 */
#include	<syslog.h>		/* 用于 syslog() */

int		daemon_proc;		/* 用 daemon_init() 置为非零 */

static void	err_doit(int, int, const char *, va_list);

/* 系统调用的非致命错误
 * 打印信息后返回 */

void
err_ret(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}

/* 系统调用的致命错误
 * 打印信息后终止 */

void
err_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

/* 系统调用的致命错误
 * 打印信息后转储内核并终止 */

void
err_dump(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	abort();		/* 转储内核并终止 */
	exit(1);		/* 不会执行到这里 */
}

/* 非系统调用的非致命错误
 * 打印信息后返回 */

void
err_msg(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, LOG_INFO, fmt, ap);
	va_end(ap);
	return;
}

/* 非系统调用的致命错误
 * 打印信息后终止 */

void
err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

/* 打印信息并返回调用者
 * 调用者指定 "errnoflag" 和 "level" */

static void
err_doit(int errnoflag, int level, const char *fmt, va_list ap)
{
	int		errno_save, n;
	char	buf[MAXLINE + 1];

	errno_save = errno;		/* 调用者想要打印的值 */
#ifdef	HAVE_VSNPRINTF
	vsnprintf(buf, MAXLINE, fmt, ap);		/* 安全 */
#else
	vsprintf(buf, fmt, ap);					/* 不安全 */
#endif
	n = strlen(buf);
	if (errnoflag)
		snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));
	strcat(buf, "\n");

	if (daemon_proc) {
		syslog(level, buf);
	} else {
		fflush(stdout);		/* 当标准输出和标准错误一样时 */
		fputs(buf, stderr);
		fflush(stderr);
	}
	return;
}
