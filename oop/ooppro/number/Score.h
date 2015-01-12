/* Score.h
* 描述: 得分类
* 作者: 田劲锋
* 创建时间: 2015年1月11日
* 修改时间: 2015年1月12日
*/
#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "Number.h"

class Score
{
    friend class Number;
public:
    virtual ~Score();

    static Score& getInstance();

    void newGame();
    void newGame(int);

    std::pair<bool, std::string> guess(int);

    int read();
    void write();

    int getScore();

    bool checkPassword(std::string password);

protected:
    int score;
    int lastNumber; // 用户上一次猜的数
    std::vector<Number> numbers;

    const static int PLUS = 20;
    const static int MINUS = 40;

    void plus();
    void minus();

private:
    Score();
    std::string password;
};
