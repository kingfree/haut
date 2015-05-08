#include <iostream>
using std::cerr;
using std::endl;

#include "Account.h"

Account::Account(double kane)
{
    if (kane < 0) {
        cerr << "初始金额 " << kane << " 不能为负数！" << endl;
        kane = 0;
    }
    balance = kane;
}

Account::~Account()
{}

void Account::credit(double kane)
{
    balance += kane;
}

bool Account::debit(double kane)
{
    if (kane > getBalance()) {
        cerr << "取钱金额超过账户余额！" << endl;
        return false;
    }
    balance -= kane;
    return true;
}

double Account::getBalance() const
{
    return balance;
}
