/* UserStock.h
* ����: �û���Ʊ�࣬�洢
* ����: �ﾢ��
* ����ʱ��: 2015��1��11��
* �޸�ʱ��: 2015��1��11��
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
    int id; // Ψһ��ʶԪ�ص� ID
    int userId;
    int StockId;
    int num; // ��Ʊ����
    double price; // ����� / �����ۣ�����
    time_t timestamp; // ����ʱ��
private:
    User& user;
    Stock& stock;
};
