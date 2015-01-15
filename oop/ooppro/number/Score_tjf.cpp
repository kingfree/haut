/* Score_tjf.cpp
* 描述: 得分类的实现
* 作者: 田劲锋
* 创建时间: 2015-1-15
* 修改时间: 2015-1-15
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "Score.h"

int Score::getScore() const
{
    return this->score;
}

Score::Score()
{
    srand((unsigned int) time(NULL));
    this->score = 0;
    this->password = "root";
    this->lastNumber = -1;
    this->read();
}

Score& Score::getInstance()
{
    static Score instance;
    return instance;
}

Score::~Score()
{
    this->write();
}

void Score::newGame()
{
    Number n;
    this->numbers.push_back(n);
}

pair<bool, string> Score::guess(int number)
{
    Number &n = this->numbers.back();
    if (number == 8888) {
        if (this->lastNumber == -1) {
            return make_pair(false, "还没有猜过任何数");
        }
        vector<int> a = n.detail(this->lastNumber);
        ostringstream s;
        s << "第 ";
        for (size_t i = 0; i < a.size() - 1; i++) {
            s << (i + 1) << ",";
        }
        s << a.back() << "位数字正确";
        return make_pair(false, s.str());
    } else if (number == 7777) {
        int a = n.answer();
        ostringstream s;
        s << "正确答案是 " << a;
        return make_pair(false, s.str());
    } else {
        pair<int, int> a = n.guess(this->lastNumber = number);
        int x = a.first, y = a.second;
        ostringstream s;
        if (x == 4 && y == 0) {
            s << "猜中了，答案就是 " << number;
            plus();
            return make_pair(true, s.str());
        } else {
            s << "数位匹配 " << x << " 个，数匹配位不符 " << y << " 个";
            minus();
            return make_pair(false, s.str());
        }
    }
}

void Score::plus()
{
    this->score += Score::PLUS;
}

void Score::minus()
{
    this->score -= Score::MINUS;
}

const char* score_filename = "score.dat";

int Score::read()
{
    ifstream fin(score_filename);
    if (fin.good()) {
        fin >> this->password;
        fin >> this->score;
    } else {
        this->write();
    }
    return this->score;
}

void Score::write()
{
    ofstream fout(score_filename);
    if (fout.good()) {
        fout << this->password << endl;
        fout << this->score << endl;
    }
}

bool Score::checkPassword(string password)
{
    return password == this->password;
}
