/* Cash.h
 * 描述: 现金类，描述有几股该股票
 * 作者: 田劲锋
 * 创建时间: 2015年1月11日
 * 修改时间: 2015年1月11日
 */
#pragma once

#include "Stock.h"

class Cash
{
public:
    Cash(const Stock&, int = 0);
    virtual ~Cash();

protected:
    Stock& stock;
    int num; // 股数
    double amount; // 总价值
};
