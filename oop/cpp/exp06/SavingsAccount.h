#pragma once

#include "Account.h"

class SavingsAccount : public Account
{
public:
    SavingsAccount(double kane, double ritz);
    virtual ~SavingsAccount();

    double calculateInterest() const;
protected:
    double interestrate;
};
