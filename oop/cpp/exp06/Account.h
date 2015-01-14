#pragma once

class Account
{
public:
    Account(double kane);
    virtual ~Account();

    virtual void credit(double kane);
    virtual bool debit(double kane);

    double getBalance() const;
protected:
    double balance;
};
