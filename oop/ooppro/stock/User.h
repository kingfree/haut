/* User.h
 * 描述: 用户类
 * 作者: 田劲锋
 * 创建时间: 2015年1月11日
 * 修改时间: 2015年1月11日
 */
#pragma once

#include <vector>
#include <string>
using namespace std;

#include "Cash.h"
#include "Stock.h"
#include "UserStock.h"

class User
{
    friend class Stock;
    friend class UserStock;
public:
    User();
    virtual ~User();

    void setUsername(string);
    string getUsername() const;

    void setPassword(string);
    bool checkPassword(string) const;

    void setAdmin(bool);
    bool isAdmin() const;

    bool operator==(const User&) const;
    bool operator<(const User&) const;

    User(const User&);
    User& operator=(const User&);

    friend ostream& operator<<(ostream&, const User&);
    friend istream& operator>>(istream&, const User&);

protected:
    int id; // 唯一标识元素的 ID
    string username;
    string password;
    double money; // 用户手上的钱
    bool admin; // 是否为管理员
    vector<Cash> cashes; // 用户持有的股票
    double amount; // 用户持有股票的总金额
};
