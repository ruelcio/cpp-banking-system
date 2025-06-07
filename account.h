#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <string>

/**
 * Classe que representa uma conta bancária.
 * Gerencia os dados básicos da conta e operações de depósito/levantamento.
 */
class Account {
private:
    // Dados básicos da conta
    int account_number;      // Número único da conta
    double balance;          // Saldo atual em AOA
    std::string full_name;   // Nome completo do titular
    std::string national_id; // BI do titular
    std::string nationality; // Nacionalidade do titular
    std::string birth_date;  // Data de nascimento
    std::string iban;        // Número IBAN da conta

public:
    // Construtor com todos os dados necessários
    Account(int account_number_param, double balance_param,
            const std::string& full_name_param,
            const std::string& national_id_param,
            const std::string& nationality_param,
            const std::string& birth_date_param,
            const std::string& iban_param);

    // Getters para acessar os dados da conta
    int get_account_number() const;
    double get_balance() const;
    std::string get_full_name() const;
    std::string get_national_id() const;
    std::string get_nationality() const;
    std::string get_birth_date() const;
    std::string get_iban() const;

    // Operações bancárias básicas
    void deposit(double amount);    // Adiciona valor ao saldo
    bool withdraw(double amount);   // Remove valor do saldo se houver fundos

    // Métodos para persistência de dados
    std::string toFileString() const;                    // Converte conta para string (formato arquivo)
    static Account fromFileString(const std::string& line); // Cria conta a partir de string

    // Operador para exibir detalhes da conta
    friend std::ostream& operator<<(std::ostream& os, const Account& acc);
};

#endif