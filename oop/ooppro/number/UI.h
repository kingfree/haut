/* UI.h
* 描述: 用户界面类，用于用户交互
* 作者: 田劲锋
* 创建时间: 2015-1-11
* 修改时间: 2015-1-15
*/
#pragma once

#include "Score.h"

class UI
{
public:
    static void Main();

    static int MainMenu();

    static void NewGame();

    static bool GuessNumber(int n);

    static void ViewDetail();

    static void ViewAnswer();

    static void ShowScore();

    static bool InputPassword();

    static void ReadHelp();
};
