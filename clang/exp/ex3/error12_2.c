#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp;                                /* fp 是指针类型 */
    int n, sum = 0;                          /* sum 应初始化 */

    if ((fp = fopen("Int_Data.dat", "r+")) == NULL) {
        perror("打开文件失败");              /* 打开方式为读并更新 */
        exit(0);
    }

    while (fscanf(fp, "%d", &n) != EOF)      /* 读到文件尾 */
        sum = sum + n;

    fprintf(fp, "%d\n", sum);
    fclose(fp);

    return 0;
}
