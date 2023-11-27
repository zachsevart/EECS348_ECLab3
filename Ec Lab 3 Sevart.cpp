#include <iostream>
#include <iomanip>
#include <algorithm>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual ~Account() {}

    virtual void displayDetails() const {
        std::cout << "\nAccount Details for " << getType() << " (ID: " << getAccountNumber() << "):" << std::endl;
        std::cout << "   Holder: " << getAccountHolder() << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << getBalance() << std::endl;
    }

    virtual void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
        } else {
            std::cout << "Insufficient balance. Adding overdraft fee." << std::endl;
            double overdraftFee = amount - balance;
            balance = 0;  
           
            deposit(overdraftFee);
        }
    }

    virtual std::string getType() const {
        return "Account";
    }

   
    std::string getAccountNumber() const { return accountNumber; }
    std::string getAccountHolder() const { return accountHolder; }
    double getBalance() const { return balance; }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%" << std::endl;
    }

    void withdraw(double amount) override {
        if (getBalance() - amount >= 1000) {
            balance -= amount;
        } 
    }

    std::string getType() const override {
        return "Savings Account";
    }

    double getInterestRate() const {
        return interestRate;
    }
};

class CurrentAccount : public Account {
protected:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double overdraft)
        : Account(number, holder, initialBalance), overdraftLimit(overdraft) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }

    void withdraw(double amount) override {
        if (getBalance() + overdraftLimit >= amount - 1000) {
            balance -= amount;
        }
    }

    std::string getType() const override {
        return "Current Account";
    }

    double getOverdraftLimit() const {
        return overdraftLimit;
    }
};

CurrentAccount operator+(CurrentAccount& current, SavingsAccount& savings) {
    double transferAmount = std::min(savings.getBalance(), current.getBalance() + current.getOverdraftLimit());
    transferAmount = std::min(transferAmount, current.getOverdraftLimit() - (current.getBalance() - 1000));

    if (transferAmount > 0) {
        double interestEarned = transferAmount * savings.getInterestRate();
        transferAmount += interestEarned;

        current.deposit(transferAmount);
        savings.withdraw(transferAmount);
    }

    return current;
}

std::ostream& operator<<(std::ostream& os, const Account& account) {
    account.displayDetails();
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings;
    std::cout << current;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << "Account Details after deposit and withdrawal:" << std::endl;
    std::cout << savings;
    std::cout << current;

    // Transfer 300 from savings to current
    current = current + savings;

    std::cout << "Account Details after transfer:" << std::endl;
    std::cout << savings;
    std::cout << current;

    return 0;
}
