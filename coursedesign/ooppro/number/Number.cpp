/* Number.cpp
* 描述: 猜数字类
* 作者: 邢志鹏
* 创建时间: 2015-1-12
* 修改时间: 2015-1-16
*/
#include<iostream>
#include<vector>
#include<cstdlib>
#include<utility>
#include<ctime>
using namespace std;

#include"Number.h"

Number::Number()
{
	srand(time(0)); //用于生成随机的正确答案，否则每次游戏答案都是一样的
	int n = Number::genRand();
	this->setNumber( n );
	this->count = 0;
}

Number::~Number()
{
}

int Number::genRand()
{
	vector<int> s(4);
	vector<int> t;

	for (int i = 1; i < 10; i++)
		t.push_back(i);

	int randIndex = rand() % t.size();
	s[0] = t[randIndex];

	t.erase( t.begin() + randIndex);
	t.push_back(0);

	for (int i = 1; i<= 3; i++)
	{
		int randIndex = rand() % t.size();
		s[i] = t[randIndex];

		t.erase(t.begin() + randIndex);
	} 

	int nrand = 1000 * s[0] + 100*s[1] + 10*s[2] + s[3];

	return  nrand;
}

void Number::setNumber( int n )
{
	this->number = n;
	this->numbers[3] = n % 10;
	this->numbers[2] = (n / 10) % 10;
	this->numbers[1] = (n / 100) % 10;
	this->numbers[0] = n / 1000;//初始化数据成员numbers[];
}

pair<int, int> Number::guess(int n)
{
	int v[4] = {0};
	
	v[3] = n % 10;
	v[2] = (n / 10) % 10;
	v[1] = (n / 100) % 10;
	v[0] = n / 1000;

	int a[10] = {0};
	int s[10] = {0};

	int x = 0; //表示数字，位置都匹配的个数
	int y = 0; //表示数字匹配但是位置不匹配的个数
	for (int i = 0; i < 4; i++){
		if (v[i] == numbers[i]){
			x++;
		}	
		a[v[i]]++; 
		s[numbers[i]]++;
	}
	//求两个集合中元素的交集
	for (int i = 0; i < 10; i++){
		if (a[i] && s[i])
			y++;
	}
	y = y - x;
	this->count ++;
	return make_pair( x , y);
}

vector<int> Number::detail(int n)
{
	std::vector<int> v(4);
	std::vector<int> s;  //表示要返回的数组
	
	v[3] = n % 10;
	v[2] = (n / 10) % 10;
	v[1] = (n / 100) % 10;
	v[0] = (n / 1000) % 10;

	for(int i = 0; i < 4; i++){
		if( v[i] == numbers[i]) //这里numbers数组可以使用this直接使用this指针吗？
			s.push_back(i+1);
	}
	this->count ++;
	return s;
}

int Number::answer()
{
	this->count ++;
	return this->number;
}
