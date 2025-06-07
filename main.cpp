#include "account.h"
#include <iostream>
#include <vector>
#include <string>

int main(void)
{
    std::cout << "--- Testando o Construtor e Getters ---" << std::endl;

    // Criar uma nova conta
    Account minhaConta(1001, 500.0, "Alice Silva", "123456789AZ", "Portuguesa", "15-05-1990", "PT50123456789012345678901");

    // Imprimir detalhes usando os getters
    std::cout << "Numero da Conta: " << minhaConta.get_account_number() << std::endl;
    std::cout << "Titular: " << minhaConta.get_full_name() << std::endl;
    std::cout << "Saldo Inicial: " << minhaConta.get_balance() << " EUR" << std::endl;
    std::cout << "IBAN: " << minhaConta.get_iban() << std::endl;
    std::cout << std::endl;

    std::cout << "--- Testando o Operador de Impressao (<<) ---" << std::endl;
    std::cout << minhaConta << std::endl; // Isto chamará o seu operator<<
    std::cout << std::endl;

     std::cout << "--- Testando Deposit ---" << std::endl;

    minhaConta.deposit(200.0);
    std::cout << "Saldo apos deposito de 200 EUR: " << minhaConta.get_balance() << " EUR" << std::endl;

    minhaConta.deposit(-50.0); // Testar deposito negativo (nao deve mudar o saldo)
    std::cout << "Saldo apos tentativa de deposito negativo: " << minhaConta.get_balance() << " EUR" << std::endl;
    std::cout << std::endl;

    std::cout << "--- Testando Withdraw ---" << std::endl;

    // Testar levantamento bem-sucedido
    if (minhaConta.withdraw(100.0)) {
        std::cout << "Saldo apos levantamento de 100 EUR: " << minhaConta.get_balance() << " EUR" << std::endl;
    } else {
        std::cout << "Levantamento de 100 EUR falhou." << std::endl;
    }

    // Testar levantamento com saldo insuficiente
    if (minhaConta.withdraw(1000.0)) { // Deve falhar
        std::cout << "Saldo apos levantamento de 1000 EUR: " << minhaConta.get_balance() << " EUR" << std::endl;
    } else {
        std::cout << "Levantamento de 1000 EUR falhou (saldo insuficiente)." << std::endl;
    }

    // Testar levantamento negativo (nao deve mudar o saldo e deve retornar false)
    if (minhaConta.withdraw(-20.0)) {
        std::cout << "Levantamento de -20 EUR (erro) bem-sucedido. Saldo: " << minhaConta.get_balance() << " EUR" << std::endl;
    } else {
        std::cout << "Levantamento de -20 EUR (erro) falhou." << std::endl;
    }
    std::cout << std::endl;

    std::cout << "--- Testando Persistencia (toFileString e fromFileString) ---" << std::endl;

    // Converter a conta existente para uma string de ficheiro
    std::string contaString = minhaConta.toFileString();
    std::cout << "Conta convertida para string: " << contaString << std::endl;

    // Criar uma nova conta a partir da string
    Account contaCarregada = Account::fromFileString(contaString); // Chamando o metodo estatico
    std::cout << "Conta carregada da string (usando operator<<): " << std::endl;
    std::cout << contaCarregada << std::endl;

    // Verificar se os dados sao os mesmos
    std::cout << "Verificacao: Numero da conta carregada: " << contaCarregada.get_account_number() << std::endl;
    std::cout << "Verificacao: Saldo da conta carregada: " << contaCarregada.get_balance() << " EUR" << std::endl;
    std::cout << std::endl;
    return (0);
}