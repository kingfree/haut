/* error_functions.c

   在程序中用到的一些标准错误处理。
*/
#include <stdarg.h>
#include "error_functions.h"
#include "header.h"
#include "ename.c.inc"          /* 定义 ename 和 MAX_ENAME */

static void
terminate(bool useExit3)
{
    char *s;

    /* 若定义了 EF_DUMPCORE 环境变量且为非空字串则转储核心，
       否则按照 'useExit3' 声明调用 exit(3) 或 _exit(2) */

    s = getenv("EF_DUMPCORE");

    if (s != NULL && *s != '\0')
        abort();
    else if (useExit3)
        exit(EXIT_FAILURE);
    else
        _exit(EXIT_FAILURE);
}

/* 按以下方式诊断 'errno' :
      * 输出对错误号为 'err' 的错误名（在数组 'ename' 中），并输出 strerror()
      * 输出以 'format' 格式化的 'ap' */

static void
outputError(bool useErr, int err, bool flushStdout,
        const char *format, va_list ap)
{
#define BUF_SIZE 500
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s %s]",
                (err > 0 && err <= MAX_ENAME) ?
                ename[err] : "?未知?", strerror(err));
    else
        snprintf(errText, BUF_SIZE, ":");

    snprintf(buf, BUF_SIZE, "错误%s %s\n", errText, userMsg);

    if (flushStdout)
        fflush(stdout);       /* 刷新 stdout */
    fputs(buf, stderr);
    fflush(stderr);           /* 假设 stderr 非行缓冲 */
}

/* 显示包含 'errno' 的错误诊断，并直接返回 */

void
errMsg(const char *format, ...)
{
    va_list argList;
    int savedErrno;

    savedErrno = errno;       /* 有可能发生改变 */

    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);

    errno = savedErrno;
}

/* 显示包含 'errno' 的错误诊断，并终止进程 */

void
errExit(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);

    terminate(true);
}

/* 显示包含 'errno' 的错误诊断，并调用 _exit() 终止进程 */

void
err_exit(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(true, errno, false, format, argList);
    va_end(argList);

    terminate(false);
}

/* 根据错误号 'errnum' 报错 ，并终止进程 */

void
errExitEN(int errnum, const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(true, errnum, true, format, argList);
    va_end(argList);

    terminate(true);
}

/* 打印一个不含 'errno' 的错误信息，并终止进程 */

void
fatal(const char *format, ...)
{
    va_list argList;

    va_start(argList, format);
    outputError(false, 0, true, format, argList);
    va_end(argList);

    terminate(true);
}

/* 打印命令行用法，并终止进程 */

void
usageErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);           /* 刷新 stdout */

    fprintf(stderr, "用法：");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);           /* 假设 stderr 非行缓冲 */
    exit(EXIT_FAILURE);
}

/* 提示命令行用法错误，并终止进程 */

void
cmdLineErr(const char *format, ...)
{
    va_list argList;

    fflush(stdout);           /* 刷新 stdout */

    fprintf(stderr, "命令行用法错误：");
    va_start(argList, format);
    vfprintf(stderr, format, argList);
    va_end(argList);

    fflush(stderr);           /* 假设 stderr 非行缓冲 */
    exit(EXIT_FAILURE);
}
