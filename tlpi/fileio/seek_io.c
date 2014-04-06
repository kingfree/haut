/* seek_io.c

   演示 lseek() 和文件 I/O 系统调用。

   用法： seek_io 文件名 {r<长度>|R<长度>|w<字串>|s<偏移>}...

   程序打开指定的文件，按参数指定演示以下文件 I/O 操作：
           r<长度>    在当前文件偏移处读取 '长度' 字节并显示文本。
           R<长度>    在当前文件偏移处读取 '长度' 字节并显示十六进制值。
           w<字串>    在当前文件偏移处写出 '字串' 。
           s<偏移>    把当前文件偏移设置为 '偏移' 。

   样例：
        seek_io myfile wxyz s1 r2
*/
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "header.h"
int main(int argc, char *argv[])
{
    size_t len;
    off_t offset;
    int fd, ap, j;
    char *buf;
    ssize_t numRead, numWritten;
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr
            ("%s file {r<长度>|R<长度>|w<字串>|s<偏移>}...\n",
             argv[0]);
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);    /* rw-rw-rw- */
    if (fd == -1)
        errExit("open");
    for (ap = 2; ap < argc; ap++) {
        switch (argv[ap][0]) {
        case 'r':              /* 显示偏移处文本 */
        case 'R':              /* 显示偏移处十六进制 */
            len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
            buf = malloc(len);
            if (buf == NULL)
                errExit("malloc");
            numRead = read(fd, buf, len);
            if (numRead == -1)
                errExit("read");
            if (numRead == 0) {
                printf("%s: 文件结束\n", argv[ap]);
            } else {
                printf("%s: ", argv[ap]);
                for (j = 0; j < numRead; j++) {
                    if (argv[ap][0] == 'r')
                        printf("%c", (unsigned char) buf[j]);

                    /* 考虑汉字的情况还是直接输出为好，不判断是否可打印 */
                    else
                        printf("%02x ", (unsigned int) buf[j]);
                } printf("\n");
            } free(buf);
            break;
        case 'w':              /* 在当前偏移处写字串 */
            numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
            if (numWritten == -1)
                errExit("write");
            printf("%s: 已写出 %zd 字节\n", argv[ap], numWritten);

            /* %zd 是自适应字节数长度的整数格式 */
            break;
        case 's':              /* 设置偏移 */
            offset = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
            if (lseek(fd, offset, SEEK_SET) == -1)
                errExit("lseek");
            printf("%s: 设置偏移成功\n", argv[ap]);
            break;
        default:
            cmdLineErr("参数必须以 [rRws] 开始：%s\n", argv[ap]);
        }
    }
    exit(EXIT_SUCCESS);
}
