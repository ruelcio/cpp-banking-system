#ifndef BANK_H
#define BANK_H

#include "account.h"
#include <vector>
#include <string>

class Bank {
private:
    std::vector<Account> accounts;
    int next_account_number;
    std::string generate_iban(int account_number) const;

public:
    Bank();

    int open_account(const std::string& full_name,
                    const std::string& national_id,
                    const std::string& nationality,
                    const std::string& birth_date,
                    double initial_balance);

    bool close_account(int account_number);
    bool deposit(int account_number, double amount);
    bool withdraw(int account_number, double amount);
    bool transfer(int from_account, int to_account, double amount);
    void list_accounts() const;

    void save_accounts() const;
    void load_accounts();
};

#endif