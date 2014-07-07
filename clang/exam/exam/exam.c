/* exam.c
 * 入口主程序
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

int main(int argc, char *argv[])
{
    if (argc > 1) {
        // 调试模式
        if (strcmp(argv[1], "teacher") == 0) {
            ui_teacher();
            return 0;
        }
    }
    ui_index();
    return 0;
}
