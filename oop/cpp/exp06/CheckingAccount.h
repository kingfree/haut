#pragma once

#include "Account.h"

class CheckingAccount : public Account
{
public:
    CheckingAccount(double kane, double fee);
    virtual ~CheckingAccount();

    void credit(double kane);
    bool debit(double kane);

protected:
    double feechargepertransaction;
};
