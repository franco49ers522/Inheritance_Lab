// Author: Jareth Franco
// Date: October 12, 2026
// Purpose: Manage checking and savings accounts through base class pointers.

#include "BankAccount.h"
#include "Input.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include <memory>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int findAccount(const std::vector<std::unique_ptr<BankAccount>>& accounts, const std::string& number) {
    for (std::size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i]->getAccountNumber() == number) return static_cast<int>(i);
    }
    return -1;
}

// Local copies deliberately retain the account number; they are not added to the list.
template <typename Account>
void demonstrateCopies(const Account& original) {
    Account copied(original);       // Copy constructor.
    Account assigned(original);     // Existing destination object.
    assigned = original;            // Copy assignment operator.
    copied.setAccountHolderName("Copy constructor demonstration");
    assigned.setAccountHolderName("Copy assignment demonstration");
    // Change only copied names so even an empty checking account can be copied.
    assigned = assigned;            // Self-assignment must be safe.
    std::cout << "Original (unchanged):\n";
    BankAccount::printAccount(original);
    std::cout << "Independent copy-constructed object:\n";
    BankAccount::printAccount(copied);
    std::cout << "Independent copy-assigned object:\n";
    BankAccount::printAccount(assigned);
    std::cout << "Copies keep the same account number: "
              << ((original == copied && original == assigned) ? "true" : "false")
              << "\nLocal copies are destroyed when this demonstration ends.\n";
}

int main() {
    std::vector<std::unique_ptr<BankAccount>> accounts;
    std::string choice;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Inheritance Lab - Jareth Franco\n";
    while (true) {
        std::cout << "\nBank Account Management\n"
                  << "1. Create account\n2. List accounts\n3. Deposit (+=)\n"
                  << "4. Withdraw (-=)\n5. Change account holder name\n"
                  << "6. Compare accounts (==, <, >)\n"
                  << "7. Demonstrate deep copies\n8. Add savings interest\n"
                  << "9. Demonstrate polymorphic withdrawals\n0. Exit\n";
        if (!Input::readText("Choose an option: ", choice) || choice == "0") break;
        try {
            if (choice == "9") {
                // Fresh examples keep the user's own accounts unchanged.
                std::vector<std::unique_ptr<BankAccount>> demo;
                demo.push_back(std::make_unique<CheckingAccount>("DEMO-C", "Demo", 100.0, 2.0));
                demo.push_back(std::make_unique<SavingsAccount>("DEMO-S", "Demo", 100.0, 0.03));
                std::cout << "Both accounts start at $100.00. Withdraw $20.00 from each.\n";
                for (const auto& item : demo) {
                    BankAccount* account = item.get();
                    account->withdraw(20.0); // Same call, different withdrawal rules.
                    BankAccount::printAccount(*account);
                }
                std::cout << "Checking includes a $2.00 fee; savings has no fee.\n";
            } else if (choice == "1") {
                std::string type;
                if (!Input::readText("Account type (1 = checking, 2 = savings): ", type)) break;
                if (type != "1" && type != "2") {
                    std::cout << "Choose 1 or 2.\n";
                    continue;
                }
                BankAccount account = BankAccount::createAccountFromInput();
                if (findAccount(accounts, account.getAccountNumber()) != -1) {
                    std::cout << "That account number already exists.\n";
                } else {
                    double setting;
                    if (type == "1") {
                        if (!Input::readAmount("Withdrawal fee: $", setting, true)) break;
                        accounts.push_back(std::make_unique<CheckingAccount>(
                            account.getAccountNumber(), account.getAccountHolderName(),
                            account.getBalance(), setting));
                    } else {
                        if (!Input::readAmount("Interest rate as a decimal (0.03 = 3%): ", setting, true)) break;
                        accounts.push_back(std::make_unique<SavingsAccount>(
                            account.getAccountNumber(), account.getAccountHolderName(),
                            account.getBalance(), setting));
                    }
                    std::cout << "Account created.\n";
                }
            } else if (choice == "2") {
                if (accounts.empty()) std::cout << "No accounts available.\n";
                for (const auto& account : accounts) {
                    BankAccount::printAccount(*account);
                    if (const auto* checking = dynamic_cast<const CheckingAccount*>(account.get()))
                        std::cout << "  Withdrawal fee: $" << checking->getTransactionFee() << '\n';
                    if (const auto* savings = dynamic_cast<const SavingsAccount*>(account.get()))
                        std::cout << "  Interest per application: " << savings->getInterestRate() * 100.0 << "%\n";
                }
            } else if (choice == "3" || choice == "4" || choice == "5" ||
                       choice == "6" || choice == "7" || choice == "8") {
                if (accounts.empty()) {
                    std::cout << "No accounts available.\n";
                    continue;
                }
                std::string number;
                if (!Input::readText("Account number: ", number)) break;
                const int index = findAccount(accounts, number);
                if (index == -1) {
                    std::cout << "Account not found.\n";
                    continue;
                }
                BankAccount& account = *accounts[index];
                if (choice == "5") {
                    std::string name;
                    if (!Input::readText("New account holder name: ", name)) break;
                    account.setAccountHolderName(name);
                    std::cout << "Name updated.\n";
                } else if (choice == "6") {
                    if (!Input::readText("Second account number: ", number)) break;
                    const int second = findAccount(accounts, number);
                    if (second == -1) {
                        std::cout << "Account not found.\n";
                        continue;
                    }
                    const BankAccount& other = *accounts[second];
                    std::cout << "Same account number (==): " << (account == other ? "true" : "false")
                              << "\nFirst balance is less (<): " << (account < other ? "true" : "false")
                              << "\nFirst balance is greater (>): " << (account > other ? "true" : "false") << '\n';
                    BankAccount::printAccount(other);
                } else if (choice == "7") {
                    // Copy the actual derived type to preserve its fee or interest rate.
                    if (auto* checking = dynamic_cast<CheckingAccount*>(&account))
                        demonstrateCopies(*checking);
                    else if (auto* savings = dynamic_cast<SavingsAccount*>(&account))
                        demonstrateCopies(*savings);
                } else if (choice == "8") {
                    if (auto* savings = dynamic_cast<SavingsAccount*>(&account)) {
                        const double before = savings->getBalance();
                        savings->calculateInterest();
                        std::cout << "Interest added: $" << savings->getBalance() - before << '\n';
                    } else {
                        std::cout << "Interest is only available for savings accounts.\n";
                    }
                } else {
                    double amount;
                    if (!Input::readAmount("Amount: $", amount, false)) break;
                    if (choice == "3") {
                        account += amount;
                        std::cout << "Deposit successful.\n";
                    } else {
                        account -= amount;
                        std::cout << "Withdrawal successful.\n";
                    }
                }
                BankAccount::printAccount(account);
            } else {
                std::cout << "Invalid option. Choose 0 through 9.\n";
            }
        } catch (const std::invalid_argument& error) {
            std::cout << "Transaction rejected: " << error.what() << '\n';
        } catch (const std::runtime_error& error) {
            std::cout << error.what() << '\n';
            break;
        }
    }
    std::cout << "Goodbye!\n";
    return 0;
}
