// Author: Jareth Franco
// Date: October 12, 2026
// Purpose: Apply checking fees while reusing the base account's balance checks.

#include "CheckingAccount.h"
#include <cmath>
#include <stdexcept>

CheckingAccount::CheckingAccount(const std::string& number, const std::string& name,
                                 double initialBalance, double fee)
    : BankAccount(number, name, initialBalance), transactionFee(fee) {
    if (!std::isfinite(fee) || fee < 0.0) {
        throw std::invalid_argument("Transaction fee must be finite and nonnegative.");
    }
}

bool CheckingAccount::withdraw(double amount) {
    if (!std::isfinite(amount) || amount <= 0.0) return false;
    // One transaction: if the amount plus fee is too large, charge neither.
    return BankAccount::withdraw(amount + transactionFee);
}

double CheckingAccount::getTransactionFee() const {
    return transactionFee;
}
