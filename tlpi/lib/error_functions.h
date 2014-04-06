/* error_functions.h

   error_functions.c 的头文件。
*/
#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

/* 通常错误处理 */
void errMsg(const char *format, ...);

#define NORETURN
void errExit(const char *format, ...) NORETURN;
void err_exit(const char *format, ...) NORETURN;
void errExitEN(int errnum, const char *format, ...) NORETURN;
void fatal(const char *format, ...) NORETURN;
void usageErr(const char *format, ...) NORETURN;
void cmdLineErr(const char *format, ...) NORETURN;

#endif                          /*  */
