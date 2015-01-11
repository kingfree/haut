/* UserStockList.h
* ����: �û���Ʊ����
* ����: �ﾢ��
* ����ʱ��: 2015��1��11��
* �޸�ʱ��: 2015��1��11��
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
