#include <iostream>
using namespace std;

#include "Account.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"

int main()
{
    double tmp;

    SavingsAccount haruka(1000, 0.02);

    cout << "はるか的初始金额: " << haruka.getBalance() << endl;

    tmp = 200;
    cout << "はるか存入了: " << tmp << endl;
    haruka.credit(tmp);
    cout << "はるか的余额: " << haruka.getBalance() << endl;

    tmp = haruka.calculateInterest();
    cout << "はるか产生了利息: " << tmp << endl;
    haruka.credit(tmp);
    cout << "はるか的余额: " << haruka.getBalance() << endl;

    tmp = 200;
    cout << "はるか取出了: " << tmp << endl;
    haruka.debit(tmp);
    cout << "はるか的余额: " << haruka.getBalance() << endl;

    cout << endl;

    CheckingAccount honoka(1200, 2);

    cout << "ほのか的初始金额: " << honoka.getBalance() << endl;

    tmp = 2000;
    cout << "ほのか取出了: " << tmp << endl;
    honoka.debit(tmp);
    cout << "ほのか的余额: " << honoka.getBalance() << endl;

    tmp = 200;
    cout << "ほのか取出了: " << tmp << endl;
    honoka.debit(tmp);
    cout << "ほのか的余额: " << honoka.getBalance() << endl;

    cout << endl;

    CheckingAccount nico(-1000, 2);

    cout << "にこ的初始金额: " << nico.getBalance() << endl;
    cout << "にこ是个穷光蛋！" << endl;

    return 0;
}