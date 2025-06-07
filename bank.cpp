#include "Bank.h"
#include <iostream>   // For std::cout
#include <fstream>    // For handling files (save/load)
#include <limits>     // For std::numeric_limits (useful for input buffer cleanup)
#include <utility>    // For std::pair (used in the map value)
#include <iomanip>
#include <sstream>

// Construtor: Inicializa o próximo número de conta e os serviços
Bank::Bank() : next_account_number(1000) { // Starts from 1000, or whatever you want
    // Initialize the 5 services
    services[101] = {"Agua", 5000.0};      // ID 101: Agua, Value 5000 Kz (assuming Kwanzas as currency)
    services[102] = {"Electricidade", 7500.0}; // ID 102: Electricidade, Value 7500 Kz
    services[103] = {"Internet", 10000.0}; // ID 103: Internet, Value 10000 Kz
    services[104] = {"Telefone", 3000.0};    // ID 104: Telefone, Value 3000 Kz
    services[105] = {"Gas", 4000.0};         // ID 105: Gas, Value 4000 Kz

    // Optional: try to load accounts from a file when the bank starts
    loadAccountsFromFile("contas.txt");
}

// Method to add an existing account (useful for loading from file)
void Bank::addAccount(const Account& new_account) {
    accounts.push_back(new_account);
}

// Method to find an account by number
Account* Bank::findAccount(int account_number) {
    for (size_t i = 0; i < accounts.size(); ++i) { // Iterate over accounts
        if (accounts[i].get_account_number() == account_number) {
            return &accounts[i]; // Return a pointer to the found account
        }
    }
    return nullptr; // Return nullptr if the account is not found
}

// Implementation of the method to generate the next account number
int Bank::generateNextAccountNumber() {
    return next_account_number++; // Returns the current value and then increments it
}

// Implementation of the transfer method
bool Bank::transfer(int from_account_num, int to_account_num, double amount) {
    Account* from_account = findAccount(from_account_num);
    Account* to_account = findAccount(to_account_num);

    if (!from_account || !to_account) {
        std::cout << "Uma ou ambas as contas nao foram encontradas." << std::endl;
        return false;
    }

    if (from_account->withdraw(amount)) {
        to_account->deposit(amount);
        std::cout << "Transferencia de " << std::fixed << std::setprecision(2) << amount << " AOA de " 
                  << from_account_num << " para " << to_account_num << " realizada com sucesso!" << std::endl;
        std::cout << "Novo saldo da conta " << from_account_num << ": " 
                  << std::fixed << std::setprecision(2) << from_account->get_balance() << " AOA" << std::endl;
        std::cout << "Novo saldo da conta " << to_account_num << ": " 
                  << std::fixed << std::setprecision(2) << to_account->get_balance() << " AOA" << std::endl;
        return true;
    }

    std::cout << "Transferencia falhou. Saldo insuficiente na conta de origem." << std::endl;
    return false;
}

// Implementation of the method to list available services
void Bank::listAvailableServices() const {
    if (services.empty()) {
        std::cout << "Nenhum servico disponivel no momento." << std::endl;
        return;
    }
    std::cout << "\n--- Servicos Disponiveis ---" << std::endl;
    std::cout << "ID\tServico\t\tValor Padrao (Kz)" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    for (const auto& entry : services) {
        // Adjust spacing based on service name to align better
        std::cout << entry.first << "\t" << entry.second.first;
        if (entry.second.first.length() < 8) std::cout << "\t"; // Extra spacing for short names
        std::cout << "\t" << entry.second.second << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
}

void Bank::listAllAccounts() const {
    if (accounts.empty()) {
        std::cout << "Nao ha contas registadas no banco." << std::endl;
        return;
    }
    std::cout << "--- Contas no UMABANK ---" << std::endl;
    for (const auto& acc : accounts) { // Usa o operator<< da classe Account
        std::cout << acc << std::endl;
    }
    std::cout << "------------------------" << std::endl;
}

// Implementation of the Service Payment method
bool Bank::payService(int account_num, int service_id) {
    // 1. Find the account
    Account* account = findAccount(account_num);
    if (account == nullptr) {
        std::cout << "Erro no pagamento: Conta " << account_num << " nao encontrada." << std::endl;
        return false;
    }

    // 2. Find the service
    auto service_it = services.find(service_id);
    if (service_it == services.end()) { // service_it == services.end() means the ID was not found
        std::cout << "Erro no pagamento: Servico com ID " << service_id << " nao encontrado." << std::endl;
        return false;
    }

    std::string service_name = service_it->second.first;
    double service_cost = service_it->second.second;

    // 3. Try to withdraw the amount from the account
    // The withdraw method already validates the balance and if the value is positive
    if (!account->withdraw(service_cost)) {
        std::cout << "Erro no pagamento do servico '" << service_name << "': Saldo insuficiente na conta "
                  << account_num << " ou valor invalido." << std::endl;
        return false;
    }

    std::cout << "Pagamento do servico '" << service_name << "' (" << service_cost
              << " Kz) da conta " << account_num << " realizado com sucesso!" << std::endl;
    std::cout << "Novo saldo da conta " << account_num << ": " << account->get_balance() << " Kz" << std::endl;
    return true;
}

// Implementation of saveAccountsToFile
void Bank::saveAccountsToFile(const std::string& filename) const {
    std::ofstream ofs(filename); // Open file for writing
    if (!ofs.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o ficheiro " << filename << " para escrita." << std::endl;
        return;
    }
    for (const auto& acc : accounts) {
        ofs << acc.toFileString() << std::endl; // Save each account as a line
    }
    ofs.close();
    std::cout << "Contas salvas em " << filename << std::endl;
}

// Implementation of loadAccountsFromFile
void Bank::loadAccountsFromFile(const std::string& filename) {
    std::ifstream ifs(filename); // Open file for reading
    if (!ifs.is_open()) {
        std::cerr << "Aviso: Nao foi possivel abrir o ficheiro " << filename << " para leitura. Comecando com contas vazias." << std::endl;
        return;
    }

    accounts.clear(); // Clear existing accounts before loading new ones
    std::string line;
    while (std::getline(ifs, line)) {
        if (!line.empty()) {
            accounts.push_back(Account::fromFileString(line)); // Use the static method to create the account
        }
    }
    ifs.close();
    std::cout << "Contas carregadas de " << filename << std::endl;

    // Update next_account_number to be greater than any loaded account
    if (!accounts.empty()) {
        int max_acc_num = 0;
        for (const auto& acc : accounts) {
            if (acc.get_account_number() > max_acc_num) {
                max_acc_num = acc.get_account_number();
            }
        }
        next_account_number = max_acc_num + 1;
    } else {
        next_account_number = 1000; // If none loaded, revert to initial
    }
}