/* UI.cpp
* 描述: 用户界面类的实现，用于用户交互
* 作者: 邢志鹏
* 创建时间: 2015年1月12日
* 修改时间: 2015年1月12日
*/

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
using namespace std;

#include"UI.h"
#include"mylib.h" //提供密码输入转换成*的函数


void UI::Main()
{ 
	int n = 1; 
	 while (n) 
	 {  
		 n = UI::MainMenu();
		 if (n == 1)
		 {  
			 UI::NewGame();
		 }
		 else if (n == 2)
		 { 
			 UI::ShowScore(); 
		 } 
		 else if (n == 3)
		 { 
			 UI::ReadHelp(); 
		 } else 
		 { 
			 break; 
		 } 
		 cout << "                        ";
		  pause();   
	 } 
}

int UI::MainMenu()
{
	cls();
	int selection;
	cout << endl;
	cout << endl;
	cout << "			    猜数字游戏					" << endl;
	cout << "\n			  =================" << endl;
	cout << "			  ★1) 新游戏    ★" << endl;
	cout << "			  ★2) 看成绩    ★" << endl;
	cout << "			  ★3) 游戏讲解  ★" << endl;
	cout << "			  ★0) 退出      ★" << endl;
	cout << "			  =================";
	cout << endl;
	cout <<	"				---> ";

	cin >> selection;
	return selection;
}

void UI::NewGame()
{
	Score& s = Score::getInstance();
	s.newGame();
	int judge = 0;
	while (judge == 0){
		cout << "\n                      请输入四位不重复的正整数:" << endl;
		int number;
		cout << "                                ";
		cin >> number;
		if (number <= 0)
			{
				cout << "                          结束本轮游戏" << endl;
				return;
			}
		if (UI::GuessNumber(number) == true){
			judge = 1;
			cout << "                          恭喜你猜对了" << endl;
		}
	}
}

bool UI::GuessNumber(int number)
{
	if (number == 8888){
		UI::ViewDetail();
		return false;
	}
	else if (number == 7777)
	{
		UI::ViewAnswer();
		return false;
	}	
	
	Score& s = Score::getInstance();
	pair<bool, string> s1 = s.guess(number);
	cout << s1.second;
	UI::ShowScore();
	return s1.first;
}

void UI::ViewDetail()
{
	Score& s = Score::getInstance();
	pair<bool, string> sOther = s.guess(8888);
	cout << sOther.second;
	cout << endl;
}

void UI::ViewAnswer()
{
	Score& s = Score::getInstance();
	cout << "     (注：您有三次输入密码的机会，三次均不对则验证失败，然后返回上一层)" << endl;
	for (int i = 1; i <= 3; i++){
		if (UI::InputPassword() == true){
			cout << "\n                            密码输入正确\n";
			cout << s.guess(7777).second << endl;
			return;
		}
		else
			cout << "\n                            密码输入错误" << endl;
	}
	cout << "                        验证失败，返回上一层" << endl;
}

bool UI::InputPassword()
{
	Score& s = Score::getInstance();
	char *words = getpass("                  请输入查看答案所需的密码：");
    string word(words);
	return s.checkPassword(word);
}

void UI::ShowScore()
{
	Score& s = Score::getInstance();
	cout << "                          您的得分是";
	cout << s.getScore() << endl;
}

void UI::ReadHelp()
{
	cls();
	cout << "\n\n\n\n═══════════════════════════════════════"<<endl;
	cout << "★1.游戏目的是猜一个四位数，且这个四位数每位都不相同                        ★\n" 
		<< "★2.每次猜数的提示分别表示数字、位置都匹配的个数，数字匹配但位置不匹配的个数★\n"
		<< "★3.猜对了加 20 分，猜错了减 40 分                                          ★\n"
		<< "★4.猜 8888 可以得到详细的提示                                              ★\n"
		<< "★5.猜 7777 可以直接看答案，但需要密码                                      ★\n"
		<< "★6.猜 0 或负数会退出该轮游戏，但仍会计分哦                                 ★\n"
		<< "═══════════════════════════════════════" << endl;
	
}