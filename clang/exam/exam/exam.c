/* exam.c
 * 入口主程序
 */

#include <stdio.h>
#include <stdlib.h>

#include "ui.h"

int main(int argc, char *argv[])
{
    if (argc > 1) {
        // 处理命令行参数
        // 这里暂时不需要
    }
    ui_index();
    return 0;
}
