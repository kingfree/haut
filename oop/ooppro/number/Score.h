/* Score.h
* 描述: 得分类
* 作者: 田劲锋
* 创建时间: 2015年1月11日
* 修改时间: 2015年1月11日
*/
#pragma once

#include <vector>
#include <algorithm>

class Score
{
public:
    Score();
    Score(int);
    virtual ~Score();

    Score& operator++();
    Score& operator--();
    Score operator++(int);
    Score operator--(int);
    void plus();
    void minus();

    int read();
    void write();

protected:
    int score;

    const static int PLUS = 20;
    const static int MINUS = 40;
};
