/* UI_tjf.cpp
* 描述: 用户界面类的实现
* 作者: 田劲锋
* 创建时间: 2015-1-15
* 修改时间: 2015-1-15
*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
using namespace std;

#include "UI.h"
#include "mylib.h"

void UI::Main()
{
    int n = 1;
    while (n) {
        n = UI::MainMenu();
        if (n == 1) {
            UI::NewGame();
        } else if (n == 2) {
            UI::ShowScore();
        } else if (n == 3) {
            UI::ReadHelp();
        } else {
            break;
        }
        pause();
    }
}

int geti()
{
    cin.clear();
    string str;
    getline(cin, str);
    int n = atoi(str.c_str());
    return n;
}

int UI::MainMenu()
{
    cls();

    cout << endl;
    cout << "  猜数字游戏" << endl;
    cout << "--------------" << endl;
    cout << "1) 新游戏 " << endl;
    cout << "2) 查成绩" << endl;
    cout << "3) 看帮助" << endl;
    cout << "0) 退出" << endl;
    cout << endl;
    cout << "---> ";

    int n = geti();
    return n;
}

void UI::ReadHelp()
{
    cout << endl
        << "  猜数字游戏 帮助" << endl
        << "-------------------" << endl
        << "1. 游戏目的是猜一个四位数，且这个四位数每位都不相同" << endl
        << "2. 每次猜数都会有提示" << endl
        << "   分别表示数字、位置都匹配的个数，数字匹配但位置不匹配的个数" << endl
        << "3. 猜对了加 20 分，猜错了减 40 分" << endl
        << "4. 猜 8888 可以得到详细的提示" << endl
        << "5. 猜 7777 可以直接看答案，但需要密码" << endl
        << "6. 猜 0 或负数会退出该轮游戏，但仍会计分哦" << endl
        << endl
        << "**** 享受你的游戏！****" << endl;
}

void UI::NewGame()
{
    Score& s = Score::getInstance();
    s.newGame();
    bool flag = false;
    while (!flag) {
        cout << "请猜一个四位数: " << endl;
        cout << "==> ";
        int n = geti();
        if (n <= 0) {
            cout << "结束本轮游戏！" << endl;
            return;
        }
        flag = UI::GuessNumber(n);
    }
    cout << "恭喜你猜对了！" << endl;
}

bool UI::GuessNumber(int n)
{
    if (n == 8888) {
        UI::ViewDetail();
        return false;
    } else if (n == 7777) {
        UI::ViewAnswer();
        return false;
    }
    cout << "你猜的是: " << n << endl;
    Score& s = Score::getInstance();
    pair<bool, string> a = s.guess(n);
    cout << a.second << endl;
    UI::ShowScore();
    return a.first;
}

void UI::ViewDetail()
{
    Score& s = Score::getInstance();
    cout << s.guess(8888).second << endl;
}

void UI::ViewAnswer()
{
    for (int i = 0; i < 3; i++) {
        if (UI::InputPassword()) {
            Score& s = Score::getInstance();
            cout << s.guess(7777).second << endl;
            return;
        } else {
            cout << "密码错误！" << endl;
        }
    }
}

bool UI::InputPassword()
{
    Score& s = Score::getInstance();
    string t("请输入密码: ");
    string password(getpass((char *) t.c_str()));
    cout << endl;
    return s.checkPassword(password);
}

void UI::ShowScore()
{
    Score& s = Score::getInstance();
    cout << "当前得分: " << s.getScore() << endl;
}
