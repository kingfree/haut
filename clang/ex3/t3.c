#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student {
    int id;
    char name[16];
    int score[3];
    int total, avg;
} student;

int main(int argc, char *argv[])
{
    char *filename = "stu.db";
    if (argc > 1)
        filename = argv[1];
    FILE *file = fopen(filename, "wb+");   /* 以二进制方式打开文件 */
    if (file == NULL) {
        perror("打开文件失败");
        exit(errno);
    }

    student a[10];
    int i = 0;
    while (scanf("%d%s%d%d%d", &a[i].id, a[i].name,
        &a[i].score[0], &a[i].score[1], &a[i].score[2]) > 0) {
        a[i].total = a[i].score[0] + a[i].score[1] + a[i].score[2];
        a[i].avg = a[i].total / 3;
        i++;                               /* 边读入，边计算 */
    }
    int n = i;
    fwrite(a, sizeof(student), n, file);   /* 直接写出二进制数据 */

    rewind(file);
    student b[10];
    fread(b, sizeof(student), n, file);    /* 直接读入二进制数据 */
    fclose(file);

    printf("学号\t 姓名\t 数学\t 语文\t 英语\t 总成绩\t 平均分\n");
    for (i = 0; i < n; i++) {
        printf("%d\t %s\t %d\t %d\t %d\t %d\t %d\n", b[i].id, b[i].name,
            b[i].score[0], b[i].score[1], b[i].score[2],
            b[i].total, b[i].avg);
    }

    return 0;
}
