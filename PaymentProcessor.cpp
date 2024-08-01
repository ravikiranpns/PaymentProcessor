#include <iostream>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

class Account {
public:
    std::string accountId;
    std::string ownerName;
    double balance;

    // Default constructor
    Account() : accountId(""), ownerName(""), balance(0.0) {}

    // Parameterized constructor
    Account(const std::string& accountId, const std::string& ownerName, double balance)
        : accountId(accountId), ownerName(ownerName), balance(balance) {}
};

class Transaction {
public:
    std::string transactionId;
    std::string fromAccountId;
    std::string toAccountId;
    double amount;
    std::time_t timestamp;

    // Default constructor
    Transaction() : transactionId(""), fromAccountId(""), toAccountId(""), amount(0.0), timestamp(0) {}

    // Parameterized constructor
    Transaction(const std::string& transactionId, const std::string& fromAccountId,
                const std::string& toAccountId, double amount)
        : transactionId(transactionId), fromAccountId(fromAccountId),
          toAccountId(toAccountId), amount(amount), timestamp(std::time(nullptr)) {}
};

class PaymentProcessor {
private:
    std::unordered_map<std::string, Account> accounts;
    std::vector<Transaction> transactions;
    std::mutex mtx;

    std::string generateTransactionId() {
        static int idCounter = 0;
        std::stringstream ss;
        ss << "TX" << std::setw(6) << std::setfill('0') << ++idCounter;
        return ss.str();
    }

public:
    void createAccount(const std::string& ownerName, double initialBalance) {
        std::lock_guard<std::mutex> lock(mtx);
        static int accountCounter = 0;
        std::stringstream ss;
        ss << "AC" << std::setw(6) << std::setfill('0') << ++accountCounter;
        std::string accountId = ss.str();

        accounts.emplace(accountId, Account(accountId, ownerName, initialBalance));
        std::cout << "Account created: " << accountId << ", Owner: " << ownerName << ", Initial Balance: " << initialBalance << std::endl;
    }

    bool processTransaction(const std::string& fromAccountId, const std::string& toAccountId, double amount) {
        std::lock_guard<std::mutex> lock(mtx);
        if (accounts.find(fromAccountId) == accounts.end() || accounts.find(toAccountId) == accounts.end()) {
            std::cerr << "Transaction failed: One or both accounts not found." << std::endl;
            return false;
        }
        if (accounts[fromAccountId].balance < amount) {
            std::cerr << "Transaction failed: Insufficient funds." << std::endl;
            return false;
        }

        accounts[fromAccountId].balance -= amount;
        accounts[toAccountId].balance += amount;

        std::string transactionId = generateTransactionId();
        transactions.emplace_back(transactionId, fromAccountId, toAccountId, amount);
        std::cout << "Transaction successful: " << transactionId << ", Amount: " << amount << " from " << fromAccountId << " to " << toAccountId << std::endl;
        return true;
    }

    double getAccountBalance(const std::string& accountId) {
        std::lock_guard<std::mutex> lock(mtx);
        if (accounts.find(accountId) == accounts.end()) {
            std::cerr << "Account not found." << std::endl;
            return -1.0;
        }
        return accounts[accountId].balance;
    }
};

int main() {
    PaymentProcessor processor;

    // Create two new accounts
    processor.createAccount("Alice", 1000.0);
    processor.createAccount("Bob", 500.0);

    // Process a transaction
    processor.processTransaction("AC000001", "AC000002", 200.0);

    // Display the current balance of each account
    double aliceBalance = processor.getAccountBalance("AC000001");
    double bobBalance = processor.getAccountBalance("AC000002");

    std::cout << "Alice's Balance: " << aliceBalance << std::endl;
    std::cout << "Bob's Balance: " << bobBalance << std::endl;

    return 0;
}
