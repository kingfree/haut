/* t_readv.c

   演示 readv() 系统调用实现的 "分散 I/O" 功能。
*/
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "header.h"

int main(int argc, char *argv[])
{
    int fd;
    struct iovec iov[3];
    struct stat myStruct;       /* 第一个缓冲区 */
    int x;                      /* 第二个缓冲区 */
#define STR_SIZE 100
    char str[STR_SIZE];         /* 第三个缓冲区 */
    ssize_t numRead, totRequired;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s 文件名\n", argv[0]);

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        errExit("open");

    totRequired = 0;

    iov[0].iov_base = &myStruct;
    iov[0].iov_len = sizeof(struct stat);
    totRequired += iov[0].iov_len;

    iov[1].iov_base = &x;
    iov[1].iov_len = sizeof(x);
    totRequired += iov[1].iov_len;

    iov[2].iov_base = str;
    iov[2].iov_len = STR_SIZE;
    totRequired += iov[2].iov_len;

    numRead = readv(fd, iov, 3);
    if (numRead == -1)
        errExit("readv");

    if (numRead < totRequired)
        printf("读入字节小于请求\n");

    int i;
    for (i = 0; i < 3; i++)
        printf("缓冲区 %d：\n%s\n", i + 1, (char *) iov[i].iov_base);

    printf("总请求字节：%ld，读入字节：%ld\n",
           (long) totRequired, (long) numRead);
    exit(EXIT_SUCCESS);
}
