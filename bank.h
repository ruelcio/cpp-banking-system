#ifndef BANK_H
#define BANK_H

#include "account.h"
#include <vector>
#include <string>

/**
 * Classe que gerencia o banco e suas operações.
 * Responsável por manter o registro de contas e realizar operações bancárias.
 */
class Bank {
private:
    std::vector<Account> accounts;      // Lista de todas as contas
    int next_account_number;            // Próximo número de conta disponível
    std::string generate_iban(int account_number) const;  // Gera IBAN para nova conta

public:
    Bank();  // Inicializa o banco com número inicial de conta 1000

    // Operações de gerenciamento de contas
    int open_account(const std::string& full_name,
                    const std::string& national_id,
                    const std::string& nationality,
                    const std::string& birth_date,
                    double initial_balance);

    bool close_account(int account_number);    // Encerra uma conta existente
    bool deposit(int account_number, double amount);    // Realiza depósito
    bool withdraw(int account_number, double amount);   // Realiza levantamento
    bool transfer(int from_account, int to_account, double amount);  // Transfere entre contas
    void list_accounts() const;    // Lista todas as contas

    // Persistência de dados
    void save_accounts() const;    // Salva contas em arquivo
    void load_accounts();          // Carrega contas do arquivo
};

#endif