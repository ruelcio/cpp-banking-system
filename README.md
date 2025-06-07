# UMA Banking System

Um sistema bancário simples desenvolvido em C++ para gerenciamento de contas bancárias, com foco no contexto angolano.

## Funcionalidades

- Abertura de contas com validação de dados
- Depósitos e levantamentos
- Transferências entre contas
- Consulta de saldo e detalhes da conta
- Listagem de todas as contas
- Encerramento de contas
- Persistência de dados em arquivo
- Validação de BI (Bilhete de Identidade)
- Suporte à moeda Kwanza (AOA)
- Geração automática de IBAN

## Requisitos do Sistema

- Compilador C++ com suporte a C++11 ou superior
- Sistema operacional: Windows, Linux ou macOS

## Compilação

Para compilar o projeto, execute o seguinte comando no terminal:

```bash
g++ -o meu_programa_bancario.exe main.cpp bank.cpp account.cpp -std=c++11
```

## Estrutura do Projeto

- `main.cpp`: Contém a interface do usuário e funções de validação
- `bank.cpp` e `bank.h`: Implementação da classe Bank
- `account.cpp` e `account.h`: Implementação da classe Account
- `contas.txt`: Arquivo de persistência das contas

## Validações Implementadas

- **BI (Bilhete de Identidade)**: 8 a 15 dígitos
- **Nome**: Apenas letras e caracteres especiais comuns em nomes
- **Data de Nascimento**: Formato DD-MM-AAAA
- **Valores Monetários**: Entre 0 e 10.000.000 AOA

## Formato dos Dados

### Conta Bancária
- Número da conta (gerado automaticamente)
- IBAN (gerado automaticamente)
- Nome completo do titular
- BI (Bilhete de Identidade)
- Nacionalidade
- Data de nascimento
- Saldo em Kwanza (AOA)

### Arquivo de Contas (contas.txt)
As contas são salvas no formato:
```
numero_conta|saldo|nome_completo|bi|nacionalidade|data_nascimento|iban
```

## Uso

1. Execute o programa compilado:
```bash
./meu_programa_bancario.exe
```

2. Siga o menu interativo para:
   - Abrir nova conta
   - Realizar depósitos
   - Fazer levantamentos
   - Consultar contas
   - Realizar transferências
   - Encerrar contas

## Contribuição

Para contribuir com o projeto:
1. Faça um fork do repositório
2. Crie uma branch para sua feature (`git checkout -b feature/nova-feature`)
3. Commit suas mudanças (`git commit -m 'Adiciona nova feature'`)
4. Push para a branch (`git push origin feature/nova-feature`)
5. Abra um Pull Request

## Licença

Este projeto está sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.

## Autor

Ruélcio João Muliata

## Contato

Para sugestões ou reportar problemas, por favor abra uma issue no repositório.
