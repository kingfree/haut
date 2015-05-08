#include "CheckingAccount.h"

CheckingAccount::CheckingAccount(double kane, double fee)
    : Account(kane)
{
    feechargepertransaction = fee;
}

CheckingAccount::~CheckingAccount()
{}

void CheckingAccount::credit(double kane)
{
    Account::credit(kane);
    balance -= feechargepertransaction;
}

bool CheckingAccount::debit(double kane)
{
    if (Account::debit(kane)) {
    	balance -= feechargepertransaction;
    	return true;
    }
    return false;
}
