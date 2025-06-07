#include "Account.h"
#include "Bank.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <utility>
#include <ios>
#include <regex>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>

// Constantes do sistema
const std::string MOEDA = "AOA";  // Moeda utilizada (Kwanza)

// Função para exibir o menu principal
void displayMenu() {
    std::cout << "\n=== UMABANK - Banco Angolano ===" << std::endl;
    std::cout << "1. Abrir Conta" << std::endl;
    std::cout << "2. Depositar" << std::endl;
    std::cout << "3. Levantar" << std::endl;
    std::cout << "4. Consultar Conta" << std::endl;
    std::cout << "5. Listar Todas as Contas" << std::endl;
    std::cout << "6. Transferencia" << std::endl;
    std::cout << "7. Encerrar Conta" << std::endl;
    std::cout << "0. Sair" << std::endl;
}

/**
 * Funções de validação para garantir a integridade dos dados
 */
bool isValidBI(const std::string& bi) {
    // Remove espaços e hífens do BI
    std::string clean_bi = bi;
    clean_bi.erase(std::remove_if(clean_bi.begin(), clean_bi.end(), 
        [](char c) { return c == ' ' || c == '-'; }), clean_bi.end());
    
    // Verifica se o BI tem entre 8 e 15 dígitos
    if (clean_bi.length() < 8 || clean_bi.length() > 15) return false;
    return std::all_of(clean_bi.begin(), clean_bi.end(), ::isdigit);
}

bool isValidDate(const std::string& date) {
    // Valida formato DD-MM-AAAA e verifica se a data é válida
    std::regex date_pattern("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-\\d{4}$");
    if (!std::regex_match(date, date_pattern)) return false;

    int day = std::stoi(date.substr(0, 2));
    int month = std::stoi(date.substr(3, 2));
    int year = std::stoi(date.substr(6, 4));

    // Validações básicas de data
    if (year < 1900 || year > 2024) return false;
    if (month < 1 || month > 12) return false;
    
    // Verifica dias do mês, incluindo anos bissextos
    int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) days_in_month[1] = 29;
    
    return day >= 1 && day <= days_in_month[month-1];
}

bool isValidName(const std::string& name) {
    // Valida nome: apenas letras, espaços e caracteres especiais comuns
    if (name.empty() || name.length() < 3) return false;
    std::regex name_pattern("^[A-Za-zÀ-ÿ\\s'-]+$");
    return std::regex_match(name, name_pattern);
}

bool isValidAmount(double amount) {
    // Valida valores monetários: entre 0 e 10 milhões
    return amount > 0 && amount <= 10000000;
}

/**
 * Funções de entrada de dados com validação
 */
template <typename T>
T getValidatedInput(const std::string& prompt, bool validate = false) {
    // Função genérica para entrada de dados numéricos
    T value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Entrada invalida. Por favor, digite um valor valido." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (typeid(T) == typeid(double)) {
                if (!validate || isValidAmount(value)) {
                    return value;
                } else {
                    std::cout << "Valor invalido. O valor deve estar entre 0 e 10.000.000." << std::endl;
                }
            } else {
                return value;
            }
        }
    }
}

std::string getValidatedStringInput(const std::string& prompt, bool validate = false) {
    // Função específica para entrada de strings com validação
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (!validate) return value;
        if (prompt.find("Nome") != std::string::npos) {
            if (isValidName(value)) return value;
            std::cout << "Nome invalido. Use apenas letras e caracteres especiais comuns em nomes." << std::endl;
        } else if (prompt.find("BI") != std::string::npos) {
            if (isValidBI(value)) return value;
            std::cout << "BI invalido. Deve conter entre 8 e 15 digitos." << std::endl;
        } else if (prompt.find("Data") != std::string::npos) {
            if (isValidDate(value)) return value;
            std::cout << "Data invalida. Use o formato DD-MM-AAAA." << std::endl;
        } else {
            return value;
        }
    }
}

std::string formatarKwanza(double valor) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << valor << " " << MOEDA;
    return ss.str();
}

std::string formatAccountNumber(int number, int width) {
    std::string s = std::to_string(number);
    return s.length() < width ? std::string(width - s.length(), '0') + s : s;
}

/**
 * Função principal do programa
 */
int main() {
    Bank umabank;
    umabank.load_accounts();  // Carrega contas existentes

    int choice;
    do {
        displayMenu();
        choice = getValidatedInput<int>("Escolha uma opcao: ");

        // Menu de operações bancárias
        switch (choice) {
            case 1: {  // Abrir conta
                std::string full_name = getValidatedStringInput("Nome completo: ", true);
                std::string national_id = getValidatedStringInput("BI: ", true);
                std::string nationality = getValidatedStringInput("Nacionalidade: ", true);
                std::string birth_date = getValidatedStringInput("Data de nascimento (DD/MM/AAAA): ", true);
                double initial_balance = getValidatedInput<double>("Saldo inicial: ");

                int account_number = umabank.open_account(full_name, national_id, nationality, birth_date, initial_balance);
                std::cout << "\nConta aberta com sucesso! Numero da conta: " << account_number << std::endl;
                break;
            }
            case 2: {  // Depositar
                int acc_num_deposit = getValidatedInput<int>("Numero da conta: ");
                double amount_deposit = getValidatedInput<double>("Valor a depositar: ");

                if (umabank.deposit(acc_num_deposit, amount_deposit)) {
                    std::cout << "\nDeposito realizado com sucesso!" << std::endl;
                } else {
                    std::cout << "\nErro ao realizar deposito. Verifique o numero da conta." << std::endl;
                }
                break;
            }
            case 3: {  // Levantar
                int acc_num_withdraw = getValidatedInput<int>("Numero da conta: ");
                double amount_withdraw = getValidatedInput<double>("Valor a levantar: ");

                if (umabank.withdraw(acc_num_withdraw, amount_withdraw)) {
                    std::cout << "\nLevantamento realizado com sucesso!" << std::endl;
                } else {
                    std::cout << "\nErro ao realizar levantamento. Verifique o numero da conta e o saldo." << std::endl;
                }
                break;
            }
            case 4: {  // Consultar conta
                int acc_num_consult = getValidatedInput<int>("Numero da conta: ");
                umabank.list_accounts();
                break;
            }
            case 5: {  // Listar todas as contas
                umabank.list_accounts();
                break;
            }
            case 6: {  // Transferência
                int from_account = getValidatedInput<int>("Conta de origem: ");
                int to_account = getValidatedInput<int>("Conta de destino: ");
                double amount = getValidatedInput<double>("Valor a transferir: ");

                if (umabank.transfer(from_account, to_account, amount)) {
                    std::cout << "\nTransferencia realizada com sucesso!" << std::endl;
                } else {
                    std::cout << "\nErro ao realizar transferencia. Verifique os numeros das contas e o saldo." << std::endl;
                }
                break;
            }
            case 7: {  // Encerrar conta
                int acc_num_close = getValidatedInput<int>("Numero da conta a encerrar: ");
                if (umabank.close_account(acc_num_close)) {
                    std::cout << "\nConta encerrada com sucesso!" << std::endl;
                } else {
                    std::cout << "\nErro ao encerrar conta. Verifique o numero da conta." << std::endl;
                }
                break;
            }
            case 0:  // Sair
                umabank.save_accounts();
                std::cout << "\nObrigado por usar o UMA Banking System!" << std::endl;
                break;
            default:
                std::cout << "\nOpcao invalida. Tente novamente." << std::endl;
        }
    } while (choice != 0);

    return 0;
}