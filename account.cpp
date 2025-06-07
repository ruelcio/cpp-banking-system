#include "account.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

Account::Account(int account_number_param, double balance_param,
                const std::string& full_name_param,
                const std::string& national_id_param,
                const std::string& nationality_param,
                const std::string& birth_date_param,
                const std::string& iban_param)
    : account_number(account_number_param),
      balance(balance_param),
      full_name(full_name_param),
      national_id(national_id_param),
      nationality(nationality_param),
      birth_date(birth_date_param),
      iban(iban_param) {}

int Account::get_account_number() const {
    return account_number;
}

double Account::get_balance() const {
    return balance;
}

std::string Account::get_full_name() const {
    return full_name;
}

std::string Account::get_national_id() const {
    return national_id;
}

std::string Account::get_nationality() const {
    return nationality;
}

std::string Account::get_birth_date() const {
    return birth_date;
}

std::string Account::get_iban() const {
    return iban;
}

void Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
    }
}

bool Account::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        return true;
    }
    return false;
}

std::string Account::toFileString() const {
    std::stringstream ss;
    ss << account_number << "|"
       << balance << "|"
       << full_name << "|"
       << national_id << "|"
       << nationality << "|"
       << birth_date << "|"
       << iban;
    return ss.str();
}

Account Account::fromFileString(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() != 7) {
        throw std::runtime_error("Formato de arquivo invalido");
    }

    return Account(
        std::stoi(tokens[0]),
        std::stod(tokens[1]),
        tokens[2],
        tokens[3],
        tokens[4],
        tokens[5],
        tokens[6]
    );
}

std::ostream& operator<<(std::ostream& os, const Account& acc) {
    os << "\n=== DETALHES DA CONTA ===" << std::endl;
    os << "Numero da Conta: " << acc.get_account_number() << std::endl;
    os << "IBAN: " << acc.get_iban() << std::endl;
    os << "Titular: " << acc.get_full_name() << std::endl;
    os << "BI: " << acc.get_national_id() << std::endl;
    os << "Nacionalidade: " << acc.get_nationality() << std::endl;
    os << "Data de Nascimento: " << acc.get_birth_date() << std::endl;
    os << "Saldo: " << std::fixed << std::setprecision(2) << acc.get_balance() << " AOA" << std::endl;
    return os;
}
