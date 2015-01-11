/* Stock.h
 * ����: ��Ʊ��
 * ����: �ﾢ��
 * ����ʱ��: 2015��1��11��
 * �޸�ʱ��: 2015��1��11��
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
    int id; // Ψһ��ʶԪ�ص� ID
    string code; // ���״���
    string name;
    bool valid; // �Ƿ�ɽ��� / ����
    double price; // �ɼ�
    int total; // ��Ʊ����
    vector<User&> users; // ���иù�Ʊ���û�
};

