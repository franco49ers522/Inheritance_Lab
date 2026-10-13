// Author: Jareth Franco
// Date: October 12, 2026
// Purpose: Declare a savings account with an interest rate.

#ifndef SAVINGS_ACCOUNT_H
#define SAVINGS_ACCOUNT_H

#include "BankAccount.h"

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& name,
                   double initialBalance, double rate);
    void calculateInterest();
    double getInterestRate() const;
    const char* getAccountType() const override { return "Savings"; }
};

#endif
