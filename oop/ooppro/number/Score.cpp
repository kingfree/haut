/* Score.cpp
* 描述: 得分类
* 作者: 邢志鹏
* 创建时间: 2015年1月11日
* 修改时间: 2015年1月11日
*/

#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<utility>
#include<vector>
using namespace std;

#include"Score.h"

Score::Score()
{
	this->score = 0;
	this->password = "root";
	this->lastNumber = -1;
	this->read();
}

int Score::getScore() const
{
	return this->score;
}

Score& Score::getInstance()
{
	static Score instance;
	return instance;  //返回的直接就是对象的引用？？
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
	Number &reNumber = this->numbers.back();

	ostringstream s;
	if (number == 8888){
		if (this->lastNumber == -1){
			s << "                      您还没有猜数，查看详细帮助属于作弊行为\n";
			s << "                          返回上一级";
			return make_pair(false, s.str());
		}
		else
		{
			vector<int> v = reNumber.detail(this->lastNumber);

			if (v.empty() == true)
			{
				s << "                详细帮助：您的数字所有数字的位置都是错误的\n";

				return make_pair(false, s.str());
			}
			s << "                     详细帮助：您的第 ";
			size_t i;
			for (i = 0; i < v.size(); i++){//要强制类型转换为int类型
				if (i != v.size() - 1)
					s << v[i] << ",";
				else
					s << v[i];
			}
			s << "位数字正确\n";
			return make_pair(false, s.str());
		}
	}

	if (number == 7777){
		s << "                        本游戏正确答案是:";
		s << reNumber.answer();
		return make_pair(false, s.str());
	}
	this->lastNumber = number;
	pair<int, int> temp = reNumber.guess(number);

	if (temp.first == 4){
		s << "                          恭喜您~,竟然猜对了！\n";
		Score::plus();
		return make_pair(true, s.str());
	}
	else{
		Score::minus();
		s << "                      数位匹配";
		s << temp.first;
		s << "个,";
		s << "数匹配位不符";
		s << temp.second;
		s << "个\n";
		return make_pair(false, s.str());
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

const char* filename = "SAP.txt";


int Score::read()
{
	
	ifstream fin;
	fin.open("SAP.txt");
	if(fin.good()){				
		fin >> this->score;
		fin >> this->password;
	}
	else{
		this->write();
	}

	return this->score;
}

void Score::write()
{
	ofstream fout("SAP.txt");

	if (fout.good()){
		fout << this->score;
		fout << this->password;
	}
	else
		cout << "文件写入失败";
}

bool Score::checkPassword( string word)
{
	if(word == this->password)
		return true;
	else
		return false;
}