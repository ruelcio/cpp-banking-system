#ifndef BANK_H
#define BANK_H

#include <vector>
#include <string>
#include <memory>
#include <map>

#include "account.h"

class Bank {
private:
    std::vector<Account> accounts; // Um vetor para guardar todas as contas
    int next_account_number;       // Para atribuir números de conta automaticamente
    std::map<int, std::pair<std::string, double>> services;

public:
    // Construtor do Bank
    Bank();

    // Métodos essenciais
    void addAccount(const Account& new_account);
    Account* findAccount(int account_number);
    void listAllAccounts() const;
    int generateNextAccountNumber();
    
    // Métodos para persistência (salvar/carregar)
    void saveAccountsToFile(const std::string& filename) const;
    void loadAccountsFromFile(const std::string& filename);

    bool transfer(int from_account_num, int to_account_num, double amount);

    bool payService(int account_num, int service_id);
    void listAvailableServices() const;
};

#endif