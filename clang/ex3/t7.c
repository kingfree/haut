#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct account {
    int id;
    char name[16];
    double bala;
} account;

account getbyid(int i, FILE *file) {
    account t;
    fseek(file, sizeof(account) * (i - 1), SEEK_SET);
    fread(&t, sizeof(account), 1, file);
    return t;
}

int main(int argc, char *argv[])
{
    int i = 0, n = 0;
    account a[10], t;
    char c;
    double v;

    char *filename = "acct.db";
    if (argc > 1)
        filename = argv[1];
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        perror("没有数据记录");
        file = fopen(filename, "wb+");
        goto input;
    }

    fseek(file, 0, SEEK_END);
    n = ftell(file) / sizeof(account);
    rewind(file);
    fread(a, sizeof(account), n, file);

    printf("目前文件中读出来 %d 个数据。 \n", n);
    for (i = 0; i < n; i++) {
        printf(" %d\t%8s\t%.2lf\n", a[i].id, a[i].name, a[i].bala);
    }

input:
    i = 0, v = 0;
    do {
        printf("输入账户编号(0退出): ");
        scanf("%d", &i);
        if (i > n) {
            printf("没有编号为 %d 的用户\n", i);
            printf("输入姓名新建(%d): ", t.id = i = ++n);
            scanf("%s", t.name);
            t.bala = 0;
        } else if (i <= 0) {
            break;
        } else {
            t = getbyid(i, file);
            printf("查询到记录 %d:\n %d\t%8s\t%.2lf\n", i, t.id, t.name, t.bala);
        }
        printf("输入收支金额: ");
        scanf("%lf", &v);
        t.bala += v;
        fseek(file, sizeof(account) * (i - 1), SEEK_SET);
        fwrite(&t, sizeof(account), 1, file);
        fflush(file);
        printf("已写出记录 %d:\n %d\t%8s\t%.2lf\n", i, t.id, t.name, t.bala);
    } while (i > 0);

    fclose(file);
    return 0;
}
