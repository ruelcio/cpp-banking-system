#include "Account.h"
#include "Bank.h"      // Inclui a sua nova classe Bank
#include <iostream>    // Para usar std::cout e std::endl
#include <vector>      // Já incluído por Bank.h, mas pode manter para clareza
#include <string>      // Já incluído por Account.h e Bank.h, mas pode manter para clareza
#include <limits>      // *** IMPORTANTE: Adicionado para std::numeric_limits ***
#include <ios>         // Para std::streamsize, que é usado com std::numeric_limits

// Função auxiliar para obter um valor numérico (double ou int) de forma segura
template <typename T>
T getNumericInput(const std::string& prompt) {
    T value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Entrada invalida. Por favor, digite um numero valido." << std::endl;
            std::cin.clear();
            // Limpa o buffer de entrada até o próximo '\n'
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer
            return value;
        }
    }
}

// Função auxiliar para obter uma string (com espaços) de forma segura
std::string getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

int main() {
    Bank umabank; // Cria uma instância do seu banco

    // Opcional: Carregar contas existentes ao iniciar o programa
    umabank.loadAccountsFromFile("contas.txt");

    int choice;
    do {
        std::cout << "\n--- Menu UMABANK ---" << std::endl;
        std::cout << "1. Abrir Conta" << std::endl;
        std::cout << "2. Depositar" << std::endl;
        std::cout << "3. Levantar" << std::endl;
        std::cout << "4. Consultar Conta" << std::endl;
        std::cout << "5. Listar Todas as Contas" << std::endl;
        std::cout << "6. Transferencia" << std::endl;
        std::cout << "7. Pagamento de Servicos (Nao implementado)" << std::endl;
        std::cout << "8. Salvar Contas no Ficheiro" << std::endl;
        std::cout << "0. Sair" << std::endl;
        std::cout << "Escolha uma opcao: ";
        // Usa a função auxiliar para ler a escolha, garantindo que seja um número
        choice = getNumericInput<int>(""); // Passa string vazia para o prompt pois já foi exibido

        switch (choice) {
            case 1: { // Abrir Conta
                std::cout << "\n--- Abrir Nova Conta ---" << std::endl;
                std::string full_name = getStringInput("Nome Completo: ");
                std::string national_id = getStringInput("Numero do BI: ");
                std::string nationality = getStringInput("Nacionalidade: ");
                std::string birth_date = getStringInput("Data de Nascimento (DD-MM-AAAA): ");

                // Geracao automatica de numero de conta e IBAN
                // *** CORREÇÃO AQUI: Usando o método publico do Bank ***
                int new_account_num = umabank.generateNextAccountNumber();

                // IBAN simplificado (pode ser melhorado depois para ser mais realista)
                std::string new_iban = "AO060001" + std::to_string(new_account_num) + "00000000000";

                Account newAccount(new_account_num, 0.0, full_name, national_id, nationality, birth_date, new_iban);
                umabank.addAccount(newAccount);
                std::cout << "Conta aberta com sucesso! Numero: " << new_account_num << ", IBAN: " << new_iban << std::endl;
                std::cout << newAccount << std::endl; // Imprime os detalhes da nova conta
                break;
            }
            case 2: { // Depositar
                int acc_num_deposit = getNumericInput<int>("Numero da conta para depositar: ");
                double amount_deposit = getNumericInput<double>("Valor a depositar: ");

                Account* acc_to_deposit = umabank.findAccount(acc_num_deposit);
                if (acc_to_deposit) {
                    acc_to_deposit->deposit(amount_deposit);
                    std::cout << "Deposito realizado. Novo saldo: " << acc_to_deposit->get_balance() << " EUR" << std::endl;
                } else {
                    std::cout << "Conta nao encontrada." << std::endl;
                }
                break;
            }
            case 3: { // Levantar
                int acc_num_withdraw = getNumericInput<int>("Numero da conta para levantar: ");
                double amount_withdraw = getNumericInput<double>("Valor a levantar: ");

                Account* acc_to_withdraw = umabank.findAccount(acc_num_withdraw);
                if (acc_to_withdraw) {
                    if (acc_to_withdraw->withdraw(amount_withdraw)) {
                        std::cout << "Levantamento realizado. Novo saldo: " << acc_to_withdraw->get_balance() << " EUR" << std::endl;
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
                double transfer_amount = getNumericInput<double>("Valor a transferir: ");

            umabank.transfer(from_acc_num, to_acc_num, transfer_amount);
            break;
            }
            case 7: { // Pagamento de Servicos (Nao implementado ainda)
                std::cout << "Funcionalidade de Pagamento de Servicos ainda nao implementada." << std::endl;
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