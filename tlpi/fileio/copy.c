/* copy.c

   复制文件 argv[1] 到 argv[2]。
*/
#include <sys/stat.h>
#include <fcntl.h>
#include "header.h"

#ifndef BUF_SIZE                /* 允许 "cc -D" 重定义 */
#define BUF_SIZE 1024
#endif                          /*  */
int main(int argc, char *argv[])
{
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s 源文件 目标文件\n", argv[0]);

    /* 打开输入输出文件 */
    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
        errExit("打开文件 %s", argv[1]);
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;  /* rw-rw-rw- */
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1)
        errExit("打开文件 %s", argv[2]);

    /* 转写数据直到文件末尾或出错 */
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead)
            fatal("无法写出整个缓冲区");
    if (numRead == -1)
        errExit("读取");
    if (close(inputFd) == -1)
        errExit("关闭输入");
    if (close(outputFd) == -1)
        errExit("关闭输出");
    exit(EXIT_SUCCESS);
}
