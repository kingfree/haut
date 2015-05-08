/* UserStock.h
* 描述: 用户股票类，存储
* 作者: 田劲锋
* 创建时间: 2015年1月11日
* 修改时间: 2015年1月11日
*/
#pragma once

#include <vector>
#include <string>
using namespace std;

#include "User.h"
#include "Stock.h"

class UserStock
{
    friend class User;
    friend class Stock;
public:
    UserStock(const User&, const Stock&, int num = 0);
    virtual ~UserStock();

    void set(const UserStock&);
    UserStock& get() const;

    bool operator==(const UserStock&) const;
    bool operator<(const UserStock&) const;

    UserStock& operator=(const UserStock&);

    friend ostream& operator<<(ostream&, const UserStock&);
    friend istream& operator>>(istream&, const UserStock&);

protected:
    int id; // 唯一标识元素的 ID
    int userId;
    int StockId;
    int num; // 股票数量
    double price; // 购入价 / 卖出价（负）
    time_t timestamp; // 交易时间
private:
    User& user;
    Stock& stock;
};
