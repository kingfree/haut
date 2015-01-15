/* Number.h
* 描述: 猜数字类
* 作者: 田劲锋
* 创建时间: 2015-1-11
* 修改时间: 2015-1-15
*/
#pragma once

#include <vector>
#include <algorithm>

class Number
{
public:
    Number();
    virtual ~Number();

    void setNumber(int number);

    std::pair<int, int> guess(int);
    std::vector<int> detail(int);
    int answer();

protected:
    int number;
    int count; // 猜数次数
private:
    int numbers[4];
    int genRand();
};
