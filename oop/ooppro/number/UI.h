/* UI.h
* 描述: 用户界面类，用于用户交互
* 作者: 田劲锋
* 创建时间: 2015年1月11日
* 修改时间: 2015年1月12日
*/
#pragma once

#include "Score.h"

class UI
{
public:
    static void Main();

    static int MainMenu();

    static void NewGame();

    static bool GuessNumber();

    static void ViewDetail();

    static void ViewAnswer();

    static void ShowScore();

    static bool InputPassword();
};
