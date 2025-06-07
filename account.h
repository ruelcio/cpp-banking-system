#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <string>

class Account {
private:
    int account_number;
    double balance;
    std::string full_name;
    std::string national_id;
    std::string nationality;
    std::string birth_date;
    std::string iban;

public:
    Account(int account_number_param, double balance_param,
            const std::string& full_name_param,
            const std::string& national_id_param,
            const std::string& nationality_param,
            const std::string& birth_date_param,
            const std::string& iban_param);

    int get_account_number() const;
    double get_balance() const;
    std::string get_full_name() const;
    std::string get_national_id() const;
    std::string get_nationality() const;
    std::string get_birth_date() const;
    std::string get_iban() const;

    void deposit(double amount);
    bool withdraw(double amount);

    std::string toFileString() const;
    static Account fromFileString(const std::string& line);

    friend std::ostream& operator<<(std::ostream& os, const Account& acc);
};

#endif