/* UserList.h
* 描述: 用户表类
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
#include "User.h"

class UserList : public List
{
public:
    UserList(string tablename);
    virtual ~UserList();

    int getTotal();

    User& getUser(int index) const;

    User& findUser(string keyword, int startIndex = 0) const;
    int findUserIndex(string keyword, int startIndex = 0) const;

    int insert(const User&);
    void remove(int index);
    void remove(int startIndex, int number);

    void sort(bool asc = true);

protected:
    vector<User&> users;
};
