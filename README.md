# Inheritance Lab

**Jareth Franco**  
October 12, 2026

This project adds checking and savings accounts to my Operator Overload Lab. The menu lets you create either type, deposit or withdraw money, compare accounts, and add interest to savings. Accounts are stored only while the program is running.

## How it works

- `BankAccount` keeps the shared account data, input checks, deep copies, and overloaded operators from the previous lab. Its withdrawal function and destructor are now virtual.
- `CheckingAccount` publicly inherits from `BankAccount`. Each successful withdrawal removes the amount plus its transaction fee. If there isn't enough for both, the balance stays the same.
- `SavingsAccount` publicly inherits from `BankAccount` and uses the normal withdrawal rules. `calculateInterest()` adds `balance * interestRate` to the account.
- The menu stores both types in a `std::vector<std::unique_ptr<BankAccount>>`. The pointers preserve each object's actual type and release the accounts automatically when the program ends.

Interest rates use decimals, so `0.03` means 3%. Each time you choose the interest option, it applies the rate once to the current balance. A zero fee or rate is allowed. Negative values, invalid input, overdrafts, and numeric overflow are rejected.

The existing `+=`, `-=`, `==`, `<`, and `>` operators still work. Since `-=` calls virtual `withdraw()`, it also charges the checking fee. Equality compares account numbers; `<` and `>` compare balances. Copy demonstrations preserve the derived type and give each copy its own holder name.

## Build and run

Open this folder as a CMake project in CLion and run the `bank_accounts` target, or use a C++14 compiler:

```sh
g++ -std=c++14 -Wall -Wextra -Wpedantic main.cpp BankAccount.cpp CheckingAccount.cpp SavingsAccount.cpp -o bank_accounts
```

Run `./bank_accounts` on Linux/macOS or `.\bank_accounts.exe` in Windows PowerShell.

## Menu

| Option | Action |
| --- | --- |
| 1 | Create a checking or savings account |
| 2 | List accounts, fees, and rates |
| 3 / 4 | Deposit / withdraw |
| 5 | Change the account holder's name |
| 6 | Compare two accounts |
| 7 | Demonstrate independent copies |
| 8 | Add interest to a savings account |
| 9 | Demonstrate withdrawals through base class pointers |
| 0 | Exit |

Option 9 uses separate demo accounts starting at $100.00. It calls `withdraw(20.0)` through a `BankAccount*` for each account. Checking ends at $78.00 with a $2.00 fee, while savings ends at $80.00. This shows how the same call uses different behavior depending on the object's type.

## Tests

```sh
g++ -std=c++14 -Wall -Wextra -Wpedantic tests/BankAccountTests.cpp BankAccount.cpp CheckingAccount.cpp SavingsAccount.cpp -o account_tests
```

Run `./account_tests` or `.\account_tests.exe`. You can also run the `account_tests` target in CLion. Expected output: `All BankAccount tests passed.`

Tests cover polymorphic withdrawals, fees, rejected transactions, zero balances and rates, repeated interest, overflow, derived copies, and the existing operators. Both targets compiled with warnings treated as errors. A menu run also checked creation, withdrawals, interest, comparisons, copies, and exit.
