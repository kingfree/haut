/* Stock.h
 * 描述: 股票类
 * 作者: 田劲锋
 * 创建时间: 2015年1月11日
 * 修改时间: 2015年1月11日
 */
#pragma once

#include <vector>
#include <string>
using namespace std;

#include "User.h"
#include "UserStock.h"

class Stock
{
    friend class User;
    friend class UserStock;
public:
    Stock();
    Stock(int code, double price, string name = "");
    virtual ~Stock();

    void setCode(int);
    void setCode(string);
    string getCode() const;

    void setName(string);
    string getName() const;

    void setPrice(double);
    double getPrice() const;

    void addTotal(int);
    int getTotal() const;

    friend ostream& operator<<(ostream&, const Stock&);
    friend istream& operator>>(istream&, const Stock&);

protected:
    int id; // 唯一标识元素的 ID
    string code; // 交易代码
    string name;
    bool valid; // 是否可交易 / 挂起
    double price; // 股价
    int total; // 股票数量
    vector<User&> users; // 持有该股票的用户
};

