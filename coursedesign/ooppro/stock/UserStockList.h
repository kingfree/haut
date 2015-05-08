/* UserStockList.h
* 描述: 用户股票表类
* 作者: 田劲锋
* 创建时间: 2015年1月11日
* 修改时间: 2015年1月11日
*/
#pragma once

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "List.h"
#include "UserStock.h"

class UserStockList : public List
{
public:
    UserStockList(string tablename);
    virtual ~UserStockList();

    int getTotal();

    UserStock& getUserStock(int index) const;

    UserStock& findUserStock(string keyword, int startIndex = 0) const;
    int findUserStockIndex(string keyword, int startIndex = 0) const;

    int insert(const UserStock&);
    void remove(int index);
    void remove(int startIndex, int number);

    void sort(bool asc = true);

protected:
    vector<UserStock&> us;
};
