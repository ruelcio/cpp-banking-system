#include "bank.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

Bank::Bank() : next_account_number(1000) {}

int Bank::open_account(const std::string& full_name,
                      const std::string& national_id,
                      const std::string& nationality,
                      const std::string& birth_date,
                      double initial_balance) {
    int account_number = next_account_number++;
    std::string iban = generate_iban(account_number);
    
    Account new_account(account_number, initial_balance, full_name,
                       national_id, nationality, birth_date, iban);
    
    accounts.push_back(new_account);
    save_accounts();
    return account_number;
}

bool Bank::close_account(int account_number) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
        [account_number](const Account& acc) {
            return acc.get_account_number() == account_number;
        });

    if (it != accounts.end()) {
        accounts.erase(it);
        save_accounts();
        return true;
    }
    return false;
}

bool Bank::deposit(int account_number, double amount) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
        [account_number](const Account& acc) {
            return acc.get_account_number() == account_number;
        });

    if (it != accounts.end()) {
        it->deposit(amount);
        save_accounts();
        return true;
    }
    return false;
}

bool Bank::withdraw(int account_number, double amount) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
        [account_number](const Account& acc) {
            return acc.get_account_number() == account_number;
        });

    if (it != accounts.end() && it->withdraw(amount)) {
        save_accounts();
        return true;
    }
    return false;
}

bool Bank::transfer(int from_account, int to_account, double amount) {
    auto from_it = std::find_if(accounts.begin(), accounts.end(),
        [from_account](const Account& acc) {
            return acc.get_account_number() == from_account;
        });

    auto to_it = std::find_if(accounts.begin(), accounts.end(),
        [to_account](const Account& acc) {
            return acc.get_account_number() == to_account;
        });

    if (from_it != accounts.end() && to_it != accounts.end() && from_it->withdraw(amount)) {
        to_it->deposit(amount);
        save_accounts();
        return true;
    }
    return false;
}

void Bank::list_accounts() const {
    if (accounts.empty()) {
        std::cout << "\nNenhuma conta cadastrada." << std::endl;
        return;
    }

    std::cout << "\n=== LISTA DE CONTAS ===" << std::endl;
    for (const auto& account : accounts) {
        std::cout << account << std::endl;
    }
}

void Bank::save_accounts() const {
    std::ofstream file("contas.txt");
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo para salvar contas." << std::endl;
        return;
    }

    for (const auto& account : accounts) {
        file << account.toFileString() << std::endl;
    }
}

void Bank::load_accounts() {
    std::ifstream file("contas.txt");
    if (!file.is_open()) {
        return;
    }

    accounts.clear();
    std::string line;
    while (std::getline(file, line)) {
        try {
            accounts.push_back(Account::fromFileString(line));
        } catch (const std::exception& e) {
            std::cerr << "Erro ao carregar conta: " << e.what() << std::endl;
        }
    }

    if (!accounts.empty()) {
        next_account_number = accounts.back().get_account_number() + 1;
    }
}

std::string Bank::generate_iban(int account_number) const {
    std::stringstream ss;
    ss << "AO06" << std::setw(21) << std::setfill('0') << account_number;
    return ss.str();
}
