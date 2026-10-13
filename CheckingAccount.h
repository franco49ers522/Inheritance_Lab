// Author: Jareth Franco
// Date: October 12, 2026
// Purpose: Declare a checking account that charges a fee for each withdrawal.

#ifndef CHECKING_ACCOUNT_H
#define CHECKING_ACCOUNT_H

#include "BankAccount.h"

class CheckingAccount : public BankAccount {
private:
    double transactionFee;

public:
    CheckingAccount(const std::string& number, const std::string& name,
                    double initialBalance, double fee);
    bool withdraw(double amount) override;
    double getTransactionFee() const;
    const char* getAccountType() const override { return "Checking"; }
};

#endif
