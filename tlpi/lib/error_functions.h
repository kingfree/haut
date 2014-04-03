/* error_functions.h

   error_functions.c 的头文件。
*/
#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

/* 通常错误处理 */

void errMsg(const char *format, ...);

#ifdef __GNUC__

    /* 这个宏防止以下函数在终止 main() 或其他非空函数时
       'gcc -Wall' 报错 "control reaches end of non-void function" */

#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif
#define NORETURN

void errExit(const char *format, ...) NORETURN ;

void err_exit(const char *format, ...) NORETURN ;

void errExitEN(int errnum, const char *format, ...) NORETURN ;

void fatal(const char *format, ...) NORETURN ;

void usageErr(const char *format, ...) NORETURN ;

void cmdLineErr(const char *format, ...) NORETURN ;

#endif
