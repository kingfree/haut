/* Number_tjf.cpp
* 描述: 数字类的实现
* 作者: 田劲锋
* 创建时间: 2015-1-15
* 修改时间: 2015-1-15
*/
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include "Number.h"

Number::Number()
{
    this->setNumber(Number::genRand());
    this->count = 0;
}

Number::~Number()
{}

int Number::genRand()
{
    int s[4];
    vector<int> t;
    for (int i = 1; i <= 9; i++)
        t.push_back(i);
    int k = rand() % t.size();
    s[0] = t[k];
    t.erase(t.begin() + k);
    t.push_back(0);
    for (int i = 1; i <= 3; i++) {
        k = rand() % t.size();
        s[i] = t[k];
        t.erase(t.begin() + k);
    }
    int n = s[0] * 1000 + s[1] * 100 + s[2] * 10 + s[3];
    return n;
}

vector<int> itov(int n)
{
    vector<int> v(4);
    v[0] = n / 1000;
    v[1] = n % 1000 / 100;
    v[2] = n % 100 / 10;
    v[3] = n % 10;
    return v;
}

void Number::setNumber(int number)
{
    this->number = number;
    vector<int> v = itov(number);
    for (int i = 0; i < 4; i++) {
        this->numbers[i] = v[i];
    }
}

pair<int, int> Number::guess(int number)
{
    int x = 0, y = 0;
    vector<int> s = itov(this->number);
    vector<int> a = itov(number);
    int v[10] = { 0 };
    for (int i = 0; i < 4; i++) {
        x += (a[i] == s[i]);
        v[a[i]]++;
        v[s[i]]++;
    }
    for (int i = 0; i < 10; i++) {
        y += (v[i] == 2);
    }
    y = y - x;
    this->count++;
    return make_pair(x, y);
}

vector<int> Number::detail(int number)
{
    vector<int> v;
    int *s = this->numbers;
    vector<int> a = itov(number);
    for (int i = 0; i < 4; i++) {
        if (a[i] == s[i]) {
            v.push_back(i + 1);
        }
    }
    this->count++;
    return v;
}

int Number::answer()
{
    this->count++;
    return this->number;
}
