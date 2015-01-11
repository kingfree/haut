/* User.h
 * ����: �û���
 * ����: �ﾢ��
 * ����ʱ��: 2015��1��11��
 * �޸�ʱ��: 2015��1��11��
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
    int id; // Ψһ��ʶԪ�ص� ID
    string username;
    string password;
    double money; // �û����ϵ�Ǯ
    bool admin; // �Ƿ�Ϊ����Ա
    vector<Cash> cashes; // �û����еĹ�Ʊ
    double amount; // �û����й�Ʊ���ܽ��
};
