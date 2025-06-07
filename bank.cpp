#include "Bank.h"
#include <iostream>   // Para std::cout
#include <fstream>    // Para lidar com ficheiros (save/load)
#include <limits>     // Para std::numeric_limits (útil para limpeza de buffer de input)

// Construtor: Inicializa o próximo número de conta
Bank::Bank() : next_account_number(1000) { // Começa a partir de 1000, ou o que desejar
    // Opcional: tentar carregar contas de um ficheiro ao iniciar o banco
    // loadAccountsFromFile("accounts.txt");
}

int Bank::generateNextAccountNumber() {
    return next_account_number++; // Retorna o valor atual e depois o incrementa
}

// Método para adicionar uma conta existente (útil para carregar do ficheiro)
void Bank::addAccount(const Account& new_account) {
    accounts.push_back(new_account);
}

// Método para encontrar uma conta por número
Account* Bank::findAccount(int account_number) {
    for (size_t i = 0; i < accounts.size(); ++i) { // Iterar sobre as contas
        if (accounts[i].get_account_number() == account_number) {
            return &accounts[i]; // Retorna um ponteiro para a conta encontrada
        }
    }
    return nullptr; // Retorna nullptr se a conta não for encontrada
}

// Método para listar todas as contas
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

bool Bank::transfer(int from_account_num, int to_account_num, double amount) {
    // 1. Validar o valor da transferência
    if (amount <= 0) {
        std::cout << "Erro na transferencia: O valor deve ser positivo." << std::endl;
        return false;
    }

    // 2. Encontrar as contas de origem e destino
    Account* from_account = findAccount(from_account_num);
    Account* to_account = findAccount(to_account_num);

    // 3. Verificar se as contas existem
    if (from_account == nullptr) {
        std::cout << "Erro na transferencia: Conta de origem (" << from_account_num << ") nao encontrada." << std::endl;
        return false;
    }
    if (to_account == nullptr) {
        std::cout << "Erro na transferencia: Conta de destino (" << to_account_num << ") nao encontrada." << std::endl;
        return false;
    }

    // 4. Verificar se as contas de origem e destino nao sao a mesma
    if (from_account_num == to_account_num) {
        std::cout << "Erro na transferencia: As contas de origem e destino nao podem ser as mesmas." << std::endl;
        return false;
    }

    // 5. Tentar retirar o valor da conta de origem
    // O método withdraw ja valida o saldo
    if (!from_account->withdraw(amount)) {
        // withdraw retorna false se o saldo for insuficiente ou o valor for invalido
        std::cout << "Erro na transferencia: Saldo insuficiente na conta de origem ("
                  << from_account_num << ") ou valor invalido para levantar." << std::endl;
        return false;
    }

    // 6. Depositar o valor na conta de destino
    // Se o withdraw foi bem-sucedido, o deposit deve ser feito.
    // O metodo deposit ja valida o valor, mas ja validamos acima que amount > 0.
    to_account->deposit(amount);

    std::cout << "Transferencia de " << amount << " EUR de " << from_account_num
              << " para " << to_account_num << " realizada com sucesso!" << std::endl;
    std::cout << "Novo saldo da conta " << from_account_num << ": " << from_account->get_balance() << " EUR" << std::endl;
    std::cout << "Novo saldo da conta " << to_account_num << ": " << to_account->get_balance() << " EUR" << std::endl;

    return true; // Transferencia bem-sucedida
}

// Implementacao de saveAccountsToFile
void Bank::saveAccountsToFile(const std::string& filename) const {
    std::ofstream ofs(filename); // Abre o ficheiro para escrita
    if (!ofs.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o ficheiro " << filename << " para escrita." << std::endl;
        return;
    }
    for (const auto& acc : accounts) {
        ofs << acc.toFileString() << std::endl; // Salva cada conta como uma linha
    }
    ofs.close();
    std::cout << "Contas salvas em " << filename << std::endl;
}

// Implementacao de loadAccountsFromFile
void Bank::loadAccountsFromFile(const std::string& filename) {
    std::ifstream ifs(filename); // Abre o ficheiro para leitura
    if (!ifs.is_open()) {
        std::cerr << "Aviso: Nao foi possivel abrir o ficheiro " << filename << " para leitura. Comecando com contas vazias." << std::endl;
        return;
    }

    accounts.clear(); // Limpa as contas existentes antes de carregar novas
    std::string line;
    while (std::getline(ifs, line)) {
        if (!line.empty()) {
            accounts.push_back(Account::fromFileString(line)); // Usa o metodo estatico para criar a conta
        }
    }
    ifs.close();
    std::cout << "Contas carregadas de " << filename << std::endl;
    
    // Atualiza o next_account_number para ser maior que qualquer conta carregada
    if (!accounts.empty()) {
        int max_acc_num = 0;
        for (const auto& acc : accounts) {
            if (acc.get_account_number() > max_acc_num) {
                max_acc_num = acc.get_account_number();
            }
        }
        next_account_number = max_acc_num + 1;
    } else {
        next_account_number = 1000; // Se nao carregou nenhuma, volta para o inicio
    }
}