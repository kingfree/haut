/* bad_exclusive_open.c

   演示为何需要 open() O_EXCL 标志.

   程序尝试以独占方式创建一个文件，方法是调用不带 O_CREAT 标志的 open() 。
*/
#include <sys/stat.h>
#include <fcntl.h>
#include "header.h"

int main(int argc, char *argv[])
{
    int fd;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s 文件名\n", argv[0]);

    fd = open(argv[1], O_WRONLY);   /* Open 1: 检查文件是否存在 */
    if (fd != -1) {             /* 打开成功 */
        printf("[PID %ld] 文件 \"%s\" 已存在\n",
               (long) getpid(), argv[1]);
        close(fd);
    } else {
        if (errno != ENOENT) {  /* 因意外原因失败 */
            errExit("open");
        } else {
            printf("[PID %ld] 文件 \"%s\" 不存在\n",
                   (long) getpid(), argv[1]);
            if (argc > 2) {     /* 在检查和创建之间等待 */
                sleep(5);       /* 睡眠 5 秒 */
                printf("[PID %ld] 沉睡中\n", (long) getpid());
            }
            fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd == -1)
                errExit("open");

            printf("[PID %ld] 创建文件 \"%s\" 成功\n", (long) getpid(), argv[1]); /* 可能不为真！ */
        }
    }

    exit(EXIT_SUCCESS);
}
