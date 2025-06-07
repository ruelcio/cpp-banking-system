#include "Account.h"
#include "Bank.h"      // Inclui a sua nova classe Bank
#include <iostream>    // Para usar std::cout e std::endl
#include <vector>      // Já incluído por Bank.h, mas pode manter para clareza
#include <string>      // Já incluído por Account.h e Bank.h, mas pode manter para clareza
#include <limits>
#include <utility>      // *** IMPORTANTE: Adicionado para std::numeric_limits ***
#include <ios>         // Para std::streamsize, que é usado com std::numeric_limits
#include <iomanip>
#include <windows.h>
#include <regex>
#include <ctime>
#include <sstream>

// Constantes para o contexto angolano
const std::string MOEDA = "AOA"; // Kwanza

// Função para definir a cor do texto no console
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Cores disponíveis
const int COLOR_WHITE = 7;
const int COLOR_GREEN = 10;
const int COLOR_BLUE = 11;
const int COLOR_RED = 12;
const int COLOR_YELLOW = 14;

// Função para limpar a tela
void clearScreen() {
    system("cls");
}

// Função para desenhar uma linha
void drawLine(char symbol = '=', int length = 50) {
    std::cout << std::string(length, symbol) << std::endl;
}

// Função para centralizar texto
void centerText(const std::string& text, int width = 50) {
    int padding = (width - text.length()) / 2;
    std::cout << std::string(padding, ' ') << text << std::endl;
}

// Função para exibir o cabeçalho
void displayHeader() {
    clearScreen();
    setColor(COLOR_BLUE);
    drawLine();
    centerText("UMABANK - Sistema Bancário");
    drawLine();
    setColor(COLOR_WHITE);
}

// Função para validar BI (número de identificação)
bool isValidBI(const std::string& bi) {
    // Remove espaços e hífens
    std::string clean_bi = bi;
    clean_bi.erase(std::remove_if(clean_bi.begin(), clean_bi.end(), 
        [](char c) { return c == ' ' || c == '-'; }), clean_bi.end());
    
    // Verifica se tem entre 8 e 15 dígitos (mais flexível)
    if (clean_bi.length() < 8 || clean_bi.length() > 15) return false;
    
    // Verifica se todos são dígitos
    return std::all_of(clean_bi.begin(), clean_bi.end(), ::isdigit);
}

// Função para validar data de nascimento (formato DD-MM-AAAA)
bool isValidDate(const std::string& date) {
    std::regex date_pattern("^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-\\d{4}$");
    if (!std::regex_match(date, date_pattern)) return false;

    // Extrai dia, mês e ano
    int day = std::stoi(date.substr(0, 2));
    int month = std::stoi(date.substr(3, 2));
    int year = std::stoi(date.substr(6, 4));

    // Verifica se a data é válida
    if (year < 1900 || year > 2024) return false;
    if (month < 1 || month > 12) return false;
    
    // Verifica dias por mês
    int days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) days_in_month[1] = 29;
    
    return day >= 1 && day <= days_in_month[month-1];
}

// Função para validar nome completo
bool isValidName(const std::string& name) {
    if (name.empty() || name.length() < 3) return false;
    
    // Verifica se contém apenas letras, espaços e caracteres especiais comuns em nomes
    std::regex name_pattern("^[A-Za-zÀ-ÿ\\s'-]+$");
    return std::regex_match(name, name_pattern);
}

// Função para validar valor monetário (ajustado para Kwanza)
bool isValidAmount(double amount) {
    return amount > 0 && amount <= 10000000; // Limite máximo de 10 milhões Kwanza
}

// Função para obter um valor numérico (double ou int) de forma segura
template <typename T>
T getNumericInput(const std::string& prompt, bool validate = false) {
    T value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Entrada invalida. Por favor, digite um numero valido." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!validate || isValidAmount(value)) {
                return value;
            } else {
                std::cout << "Valor invalido. O valor deve estar entre 0 e 10000000." << std::endl;
            }
        }
    }
}

// Função para obter uma string com validação
std::string getStringInput(const std::string& prompt, bool validate = false, const std::string& type = "") {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        
        if (validate) {
            bool isValid = false;
            if (type == "name") {
                isValid = isValidName(value);
                if (!isValid) std::cout << "Nome invalido. Use apenas letras e caracteres especiais comuns em nomes." << std::endl;
            } else if (type == "bi") {
                isValid = isValidBI(value);
                if (!isValid) std::cout << "BI invalido. Deve conter entre 8 e 15 digitos." << std::endl;
            } else if (type == "date") {
                isValid = isValidDate(value);
                if (!isValid) std::cout << "Data invalida. Use o formato DD-MM-AAAA." << std::endl;
            }
            
            if (isValid) return value;
        } else {
            return value;
        }
    }
}

std::string formatAccountNumber(int number, int width) {
    std::string s = std::to_string(number);
    // Se o número for menor que a largura desejada, adiciona zeros à esquerda
    if (s.length() < width) {
        return std::string(width - s.length(), '0') + s;
    }
    return s;
}

// Função para formatar valores em Kwanza
std::string formatarKwanza(double valor) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << valor << " " << MOEDA;
    return ss.str();
}

int main() {
    Bank umabank; // Cria uma instância do seu banco
    umabank.loadAccountsFromFile("contas.txt");

    int choice;
    do {
        std::cout << "\n=== UMABANK - Banco Angolano ===" << std::endl;
        std::cout << "1. Abrir Conta" << std::endl;
        std::cout << "2. Depositar" << std::endl;
        std::cout << "3. Levantar" << std::endl;
        std::cout << "4. Consultar Conta" << std::endl;
        std::cout << "5. Listar Todas as Contas" << std::endl;
        std::cout << "6. Transferencia" << std::endl;
        std::cout << "7. Pagamento de Servicos" << std::endl;
        std::cout << "8. Salvar Contas no Ficheiro" << std::endl;
        std::cout << "0. Sair" << std::endl;
        std::cout << "Escolha uma opcao: ";
        choice = getNumericInput<int>("");

        switch (choice) {
            case 1: {
                std::cout << "\n--- Abrir Nova Conta ---" << std::endl;
                std::string full_name = getStringInput("Nome Completo: ", true, "name");
                std::string national_id = getStringInput("Numero do BI: ", true, "bi");
                std::string nationality = getStringInput("Nacionalidade: ", true, "name");
                std::string birth_date = getStringInput("Data de Nascimento (DD-MM-AAAA): ", true, "date");

                int new_account_num = umabank.generateNextAccountNumber();
                std::string formatted_account_num = formatAccountNumber(new_account_num, 7);
                std::string new_iban = "AO0600010000" + formatted_account_num + "00";

                Account newAccount(new_account_num, 0.0, full_name, national_id, nationality, birth_date, new_iban);
                umabank.addAccount(newAccount);
                std::cout << "Conta aberta com sucesso!" << std::endl;
                std::cout << "Numero: " << new_account_num << std::endl;
                std::cout << "IBAN: " << new_iban << std::endl;
                std::cout << newAccount << std::endl;
                break;
            }
            case 2: {
                int acc_num_deposit = getNumericInput<int>("Numero da conta para depositar: ");
                double amount_deposit = getNumericInput<double>("Valor a depositar: ", true);

                Account* acc_to_deposit = umabank.findAccount(acc_num_deposit);
                if (acc_to_deposit) {
                    acc_to_deposit->deposit(amount_deposit);
                    std::cout << "Deposito realizado. Novo saldo: " << formatarKwanza(acc_to_deposit->get_balance()) << std::endl;
                } else {
                    std::cout << "Conta nao encontrada." << std::endl;
                }
                break;
            }
            case 3: {
                int acc_num_withdraw = getNumericInput<int>("Numero da conta para levantar: ");
                double amount_withdraw = getNumericInput<double>("Valor a levantar: ", true);

                Account* acc_to_withdraw = umabank.findAccount(acc_num_withdraw);
                if (acc_to_withdraw) {
                    if (acc_to_withdraw->withdraw(amount_withdraw)) {
                        std::cout << "Levantamento realizado. Novo saldo: " << formatarKwanza(acc_to_withdraw->get_balance()) << std::endl;
                    } else {
                        std::cout << "Levantamento falhou. Valor invalido ou saldo insuficiente." << std::endl;
                    }
                } else {
                    std::cout << "Conta nao encontrada." << std::endl;
                }
                break;
            }
            case 4: { // Consultar
                int acc_num_consult = getNumericInput<int>("Numero da conta para consultar: ");
                Account* acc_to_consult = umabank.findAccount(acc_num_consult);
                if (acc_to_consult) {
                    std::cout << *acc_to_consult << std::endl; // Imprime os detalhes da conta
                } else {
                    std::cout << "Conta nao encontrada." << std::endl;
                }
                break;
            }
            case 5: { // Listar Todas as Contas
                umabank.listAllAccounts();
                break;
            }
            case 6: { 
                std::cout << "\n--- Realizar Transferencia ---" << std::endl;
                int from_acc_num = getNumericInput<int>("Numero da conta de origem: ");
                int to_acc_num = getNumericInput<int>("Numero da conta de destino: ");
                double transfer_amount = getNumericInput<double>("Valor a transferir: ", true);

            umabank.transfer(from_acc_num, to_acc_num, transfer_amount);
            break;
            }
            case 7: { 
                std::cout << "\n--- Pagamento de Servicos ---" << std::endl;
                umabank.listAvailableServices(); // Mostra os serviços disponíveis

                int acc_num_pay = getNumericInput<int>("Numero da conta para pagar: ");
                int service_id_pay = getNumericInput<int>("Digite o ID do servico a pagar: ");

                umabank.payService(acc_num_pay, service_id_pay);
                break;
            }
            case 8: { // Salvar Contas
                umabank.saveAccountsToFile("contas.txt");
                break;
            }
            case 0: { // Sair
                std::cout << "Saindo do UMABANK. Ate logo!" << std::endl;
                umabank.saveAccountsToFile("contas.txt"); // Salva ao sair para nao perder dados
                break;
            }
            default: {
                std::cout << "Opcao invalida. Por favor, tente novamente." << std::endl;
                break;
            }
        }
    } while (choice != 0);

    return 0;
}