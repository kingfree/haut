/* cli.cpp
* 描述: 猜数字游戏的命令行界面
* 作者: 田劲锋
* 创建时间: 2015年1月11日
* 修改时间: 2015年1月11日
*/
#pragma once

#include "Number.h"
#include "UI.h"
#include "TestCase.h"

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "-test") == 0) {
        TestCase::All();
    }

    UI::MainMenu();

    return 0;
}
