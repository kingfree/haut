#include "SavingsAccount.h"

SavingsAccount::SavingsAccount(double kane, double ritz)
    : Account(kane)
{
    interestrate = ritz;
}

SavingsAccount::~SavingsAccount()
{}

double SavingsAccount::calculateInterest() const
{
    return balance * interestrate;
}
