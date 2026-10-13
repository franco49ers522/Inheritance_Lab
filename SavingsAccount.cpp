// Author: Jareth Franco
// Date: October 12, 2026
// Purpose: Add savings interest using the existing deposit function.

#include "SavingsAccount.h"
#include <cmath>
#include <stdexcept>

SavingsAccount::SavingsAccount(const std::string& number, const std::string& name,
                               double initialBalance, double rate)
    : BankAccount(number, name, initialBalance), interestRate(rate) {
    if (!std::isfinite(rate) || rate < 0.0) {
        throw std::invalid_argument("Interest rate must be finite and nonnegative.");
    }
}

void SavingsAccount::calculateInterest() {
    // The rate is a decimal: 0.03 means 3% per application.
    const double interest = getBalance() * interestRate;
    if (interest == 0.0) return;
    if (!deposit(interest)) {
        throw std::invalid_argument("Interest would overflow the balance.");
    }
}

double SavingsAccount::getInterestRate() const {
    return interestRate;
}
