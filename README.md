# Simplified Payment Processor

## Overview
This project implements a simplified payment processor in C++. The application includes classes for managing user accounts and transactions, along with a payment processor to handle the creation of accounts, processing transactions, and retrieving account balances. The implementation ensures thread safety to handle concurrent transactions.

## Classes
### Account
Represents a user account with the following attributes:
- `accountId` (std::string): Unique identifier for the account.
- `ownerName` (std::string): Name of the account owner.
- `balance` (double): Current balance of the account.

### Transaction
Represents a transaction between two accounts with the following attributes:
- `transactionId` (std::string): Unique identifier for the transaction.
- `fromAccountId` (std::string): Account ID from which the amount is debited.
- `toAccountId` (std::string): Account ID to which the amount is credited.
- `amount` (double): Amount transferred in the transaction.
- `timestamp` (std::time_t): Timestamp of when the transaction was processed.

### PaymentProcessor
Manages accounts and transactions, and provides the following functionalities:
- `void createAccount(const std::string& ownerName, double initialBalance)`: Creates a new account with the specified owner name and initial balance.
- `bool processTransaction(const std::string& fromAccountId, const std::string& toAccountId, double amount)`: Processes a transaction between two accounts and returns true if successful.
- `double getAccountBalance(const std::string& accountId)`: Retrieves the balance of a specific account.

## Features
- Thread-safe handling of account creation and transactions using `std::mutex`.
- In-memory storage for accounts and transactions.
- Unique ID generation for accounts and transactions.

## Getting Started

### Prerequisites
- C++11 or later compiler

### Building and Running
1. Clone the repository or download the source code.
2. Compile the code using a C++ compiler. For example, with `g++`:
    ```sh
    g++ -o payment_processor main.cpp
    ```
3. Run the executable:
    ```sh
    ./payment_processor
    ```

### Example Usage
The main function demonstrates the usage of the `PaymentProcessor` class:

```cpp
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
