#include <stdio.h>
#include <math.h>

#define PI M_PI

#include "error10_1_cal.c"
#include "error10_1_vol.c"

int main()
{
    int sel;
    while (1) {
        printf("    1 - 计算球体体积\n");
        printf("    2 - 计算圆柱体体积\n");
        printf("    3 - 计算圆锥体体积\n");
        printf(" 其他 - 退出程序运行\n");
        printf("请输入对应指令：");
        scanf("%d", &sel);
        if (sel < 1 || sel > 3) {
            break;
        } else {
            cal(sel);
        }
    }
    return 0;
}
