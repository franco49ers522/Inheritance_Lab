// Author: Jareth Franco
// Date: October 12, 2026
// Purpose: Check inheritance, fees, interest, copies, and the existing operators.

#include "../CheckingAccount.h"
#include "../SavingsAccount.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

void check(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

bool closeTo(double actual, double expected) {
    return std::abs(actual - expected) < 0.000001;
}

template <typename Action>
void rejects(Action action) {
    bool threw = false;
    try { action(); }
    catch (const std::invalid_argument&) { threw = true; }
    check(threw, "Expected invalid_argument");
}

int main() {
    static_assert(std::has_virtual_destructor<BankAccount>::value,
                  "Deleting through BankAccount must be safe");
    std::vector<std::unique_ptr<BankAccount>> accounts;
    accounts.push_back(std::make_unique<CheckingAccount>("C1", "Jareth Franco", 100.0, 2.0));
    accounts.push_back(std::make_unique<SavingsAccount>("S1", "Jareth Franco", 100.0, 0.03));
    for (const auto& account : accounts) check(account->withdraw(20.0), "Polymorphic withdrawal");
    check(closeTo(accounts[0]->getBalance(), 78.0), "Checking fee missing");
    check(closeTo(accounts[1]->getBalance(), 80.0), "Savings should not charge a fee");

    BankAccount& checking = *accounts[0];
    check(!checking.withdraw(77.0), "Must have enough for amount plus fee");
    check(closeTo(checking.getBalance(), 78.0), "Failed withdrawal charged a fee");
    check(!checking.withdraw(0.0) && !checking.withdraw(-1.0), "Invalid withdrawal accepted");
    check(!checking.withdraw(std::numeric_limits<double>::infinity()), "Infinity accepted");
    check(!checking.withdraw(std::numeric_limits<double>::quiet_NaN()), "NaN accepted");
    checking += 22.0;
    checking -= 10.0;
    check(closeTo(checking.getBalance(), 88.0), "Operator -= bypassed the fee");
    rejects([&] { checking -= 88.0; });
    check(checking.withdraw(86.0) && closeTo(checking.getBalance(), 0.0), "Exact amount plus fee");

    auto& savings = dynamic_cast<SavingsAccount&>(*accounts[1]);
    savings.calculateInterest();
    check(closeTo(savings.getBalance(), 82.4), "Savings interest");
    savings.calculateInterest();
    check(closeTo(savings.getBalance(), 84.872), "Repeated interest");
    SavingsAccount empty("S2", "Empty", 0.0, 0.03);
    empty.calculateInterest();
    check(empty.getBalance() == 0.0, "Interest on zero balance");
    SavingsAccount zeroRate("S3", "No interest", 100.0, 0.0);
    zeroRate.calculateInterest();
    check(zeroRate.getBalance() == 100.0, "Zero interest rate");
    CheckingAccount free("C2", "No fee", 100.0, 0.0);
    check(free.withdraw(100.0) && free.getBalance() == 0.0, "Zero fee withdrawal");

    CheckingAccount original("C3", "Original", 100.0, 2.0);
    CheckingAccount copied(original);
    CheckingAccount assigned("C4", "Old", 10.0, 9.0);
    assigned = original;
    assigned = assigned;
    copied.setAccountHolderName("Copy");
    assigned.setAccountHolderName("Assigned");
    copied -= 20.0;
    check(original.getAccountHolderName() == "Original" && original.getBalance() == 100.0,
          "Copy changed original");
    check(copied.getBalance() == 78.0 && assigned.getTransactionFee() == 2.0,
          "Derived copies lost fee");
    check(original == copied && original > copied && copied < original, "Comparisons");
    SavingsAccount savedCopy(savings);
    savedCopy.calculateInterest();
    check(closeTo(savings.getBalance(), 84.872), "Savings copy changed original");

    const double maximum = std::numeric_limits<double>::max();
    SavingsAccount huge("S4", "Overflow", maximum, 2.0);
    rejects([&] { huge.calculateInterest(); });
    check(huge.getBalance() == maximum, "Interest overflow changed balance");
    CheckingAccount hugeFee("C5", "Overflow", maximum, maximum);
    check(!hugeFee.withdraw(maximum) && hugeFee.getBalance() == maximum, "Fee overflow");
    rejects([] { CheckingAccount bad("C", "Name", 0.0, -1.0); });
    rejects([] { SavingsAccount bad("S", "Name", 0.0, -0.1); });
    rejects([] { SavingsAccount bad("S", "Name", -1.0, 0.03); });
    rejects([] { CheckingAccount bad("", "Name", 0.0, 1.0); });
    std::cout << "All BankAccount tests passed.\n";
}
