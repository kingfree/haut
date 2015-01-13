/* Number.h
 * 描述: 猜数字类
 * 作者: 田劲锋
 * 创建时间: 2015年1月11日
 * 修改时间: 2015年1月13日
 */
#pragma once

#include <vector>
#include <algorithm>

class Number
{
public:
    Number();
    virtual ~Number();

    std::pair<int, int> guess(int);
    std::vector<int> detail(int);
    int answer();

protected:
    int number;
    int count; // 猜数次数
private:
    short numbers[4];
    int genRand();
};
