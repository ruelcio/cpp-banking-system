#include "account.h"
#include <sstream>
#include <algorithm>
#include <string>

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
                   iban(iban_param)
                   {

                   }

int Account::get_account_number() const
{
    return account_number;
}

double  Account::get_balance() const
{
    return balance;
}

std::string Account::get_full_name() const
{
    return full_name;
}

std::string Account::get_national_id() const
{
    return national_id;
}

std::string Account::get_birth_date() const
{
    return birth_date;
}

std::string Account::get_iban() const
{
    return iban;
}

void    Account::deposit(double amount)
{
    if (amount <= 0)
        return;
    this->balance = balance + amount;

}

bool    Account::withdraw(double amount)
{
    if (amount <= 0)
        return (false);
    if (balance >= amount)
    {
        this->balance = balance - amount;
        return (true);
    } else {
        return (false);
    }
}

std::string Account::toFileString() const
{
    std::string result_string;

    result_string = std::to_string(account_number) + ";" +
                    std::to_string(balance) + ";" +
                    full_name + ";" +
                    national_id + ";" +
                    nationality + ";" +
                    birth_date + ";" +
                    iban;

    return (result_string);
}

Account Account::fromFileString(const std::string& line)
{
    std::stringstream ss(line);
    std::string token;
    int account_num_temp;
    double  balance_temp;
    std::string full_name_temp;
    std::string national_id_temp;
    std::string nationality_temp;
    std::string birth_date_temp;
    std::string iban_temp;

    std::getline(ss, token, ';');
    account_num_temp = std::stoi(token);
    std::getline(ss, token, ';'); 
    balance_temp = std::stod(token);
    std::getline(ss, full_name_temp, ';');
    std::getline(ss, national_id_temp, ';'); 
    std::getline(ss, nationality_temp, ';');
    std::getline(ss, birth_date_temp, ';');
    std::getline(ss, iban_temp);

    return Account(account_num_temp, balance_temp, full_name_temp, national_id_temp,
               nationality_temp, birth_date_temp, iban_temp);
}

std::ostream& operator<<(std::ostream& os, const Account& acc)
{
    os << "Conta: " << acc.get_account_number()
       << " | Titular: " << acc.get_full_name()
       << " | Saldo: " << acc.get_balance()
       << " | BI: " << acc.get_national_id()
       << " | IBAN: " << acc.get_iban()
       << std::endl;

       return os;
}
