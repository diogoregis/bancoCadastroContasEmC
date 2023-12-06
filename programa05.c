#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Conta {
    char numeroConta[20];
    float saldo;
    char nome[50];
    char cpf[15];
    char telefone[20];
    int status;
};

int buscaConta(struct Conta *contas, int totalContas, const char *numeroContaBusca);
void salvarDadosArquivo(struct Conta *contas, int totalContas, const char *nomeArquivo);
void carregarDadosArquivo(struct Conta **contas, int *totalContas, const char *nomeArquivo);
void consultarSaldo(struct Conta *contas, int totalContas, const char *numeroContaConsulta);
void depositar(struct Conta *contas, int totalContas, const char *numeroContaDeposito, float valorDeposito);
void sacar(struct Conta *contas, int totalContas, const char *numeroContaSaque, float valorSaque);
void exibirContas(struct Conta *contas, int totalContas);
void removerConta(struct Conta *contas, int totalContas, const char *numeroContaRemover);
void limparContasInativas(struct Conta **contas, int *totalContas);
void cadastrarCliente(struct Conta **contas, int *totalContas);


int main() {
    struct Conta *contas = NULL;
    int totalContas = 0;
    char numeroContaConsulta[20], numeroContaDeposito[20], numeroContaSaque[20], numeroContaRemover[20];
    int opcao;

    
    carregarDadosArquivo(&contas, &totalContas, "cadastros.txt");

    do {
        // MENU
        printf("\n---- Menu ----\n");
        printf("1. Cadastrar nova conta\n");
        printf("2. Consultar Saldo\n");
        printf("3. Fazer depósito\n");
        printf("4. Fazer saque\n");
        printf("5. Exibir contas\n");
        printf("6. Remover conta\n");
        printf("0. Sair\n");
        // MENU

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarCliente(&contas, &totalContas);
                break;
            case 2:
                printf("Informe o número da conta para consultar o saldo: ");
                scanf("%s", numeroContaConsulta);
                consultarSaldo(contas, totalContas, numeroContaConsulta);
                break;
            case 3:
                printf("Informe o número da conta para fazer o depósito: ");
                scanf("%s", numeroContaDeposito);
                float valorDeposito;
                printf("Informe o valor do depósito: ");
                scanf("%f", &valorDeposito);
                depositar(contas, totalContas, numeroContaDeposito, valorDeposito);
                break;
            case 4:
                printf("Informe o número da conta para fazer o saque: ");
                scanf("%s", numeroContaSaque);
                float valorSaque;
                printf("Informe o valor do saque: ");
                scanf("%f", &valorSaque);
                sacar(contas, totalContas, numeroContaSaque, valorSaque);
                break;
            case 5:
                exibirContas(contas, totalContas);
                break;
            case 6:
                printf("Informe o número da conta para remover: ");
                scanf("%s", numeroContaRemover);
                removerConta(contas, totalContas, numeroContaRemover);
                break;
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    salvarDadosArquivo(contas, totalContas, "cadastros.txt");

    limparContasInativas(&contas, &totalContas);

    free(contas);

    return 0;
}


void cadastrarCliente(struct Conta **contas, int *totalContas) {
    *contas = realloc(*contas, (*totalContas + 1) * sizeof(struct Conta));

    if (*contas == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }

    printf("Informe o número da conta: ");
    scanf("%s", (*contas + *totalContas)->numeroConta);

    printf("Informe o saldo: ");
    scanf("%f", &((*contas + *totalContas)->saldo));

    printf("Informe o nome: ");
    getchar(); 
    fgets((*contas + *totalContas)->nome, sizeof((*contas + *totalContas)->nome), stdin);
    (*contas + *totalContas)->nome[strcspn((*contas + *totalContas)->nome, "\n")] = '\0';

    printf("Informe o CPF: ");
    getchar(); 
    fgets((*contas + *totalContas)->cpf, sizeof((*contas + *totalContas)->cpf), stdin);
    (*contas + *totalContas)->cpf[strcspn((*contas + *totalContas)->cpf, "\n")] = '\0';

    printf("Informe o telefone: ");
    scanf("%s", (*contas + *totalContas)->telefone);

    (*contas + *totalContas)->status = 1;

    (*totalContas)++;
}

void consultarSaldo(struct Conta *contas, int totalContas, const char *numeroContaConsulta) {
    int posicao = -1;

    for (int i = 0; i < totalContas; i++) {
        if (strcmp(contas[i].numeroConta, numeroContaConsulta) == 0 && contas[i].status == 1) {
            posicao = i;
            break;
        }
    }

    if (posicao >= 0) {
        printf("Saldo da conta %s: %.2f\n", numeroContaConsulta, contas[posicao].saldo);
    } else {
        printf("Conta %s não cadastrada ou inativa.\n", numeroContaConsulta);
    }
}

void exibirContas(struct Conta *contas, int totalContas) {
    printf("\n---- Lista de Contas ----\n");
    for (int i = 0; i < totalContas; i++) {
        if (contas[i].status == 1) {
            printf("Número da conta: %s\n", contas[i].numeroConta);
            printf("Nome do Titular: %s\n", contas[i].nome);
            printf("Telefone: %s\n", contas[i].telefone);
            printf("------------------------\n");
        }
    }
}


void removerConta(struct Conta *contas, int totalContas, const char *numeroContaRemover) {
    for (int i = 0; i < totalContas; i++) {
        if (strcmp(contas[i].numeroConta, numeroContaRemover) == 0 && contas[i].status == 1) {
            contas[i].status = 0; 
            printf("Conta %s removida com sucesso.\n", numeroContaRemover);
            return;
        }
    }

    printf("Conta %s não encontrada ou inativa.\n", numeroContaRemover);
}

void depositar(struct Conta *contas, int totalContas, const char *numeroContaDeposito, float valorDeposito) {
    int posicao = buscaConta(contas, totalContas, numeroContaDeposito);

    if (posicao >= 0) {
        if (valorDeposito >= 0) {
            contas[posicao].saldo += valorDeposito;
            printf("Depósito de %.2f realizado na conta %s. Novo saldo: %.2f\n", valorDeposito, numeroContaDeposito, contas[posicao].saldo);
        } else {
            printf("Valor de depósito inválido. Tente novamente.\n");
        }
    } else {
        printf("Conta %s não encontrada ou inativa.\n", numeroContaDeposito);
    }
}

void sacar(struct Conta *contas, int totalContas, const char *numeroContaSaque, float valorSaque) {
    int posicao = buscaConta(contas, totalContas, numeroContaSaque);

    if (posicao >= 0) {
        if (valorSaque >= 0 && valorSaque <= contas[posicao].saldo) {
            contas[posicao].saldo -= valorSaque;
            printf("Saque de %.2f realizado na conta %s. Novo saldo: %.2f\n", valorSaque, numeroContaSaque, contas[posicao].saldo);
        } else {
            printf("Valor de saque inválido. Tente novamente.\n");
        }
    } else {
        printf("Conta %s não encontrada ou inativa.\n", numeroContaSaque);
    }
}

void limparContasInativas(struct Conta **contas, int *totalContas) {
    struct Conta *temp = malloc(*totalContas * sizeof(struct Conta));
    int count = 0;

    if (temp == NULL) {
        printf("Erro ao alocar memória temporária.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *totalContas; i++) {
        if ((*contas)[i].status == 1) {
            temp[count++] = (*contas)[i];
        }
    }

    free(*contas);

    *contas = malloc(count * sizeof(struct Conta));

    if (*contas == NULL) {
        printf("Erro ao alocar memória para contas.\n");
        exit(EXIT_FAILURE);
    }

    // Aqui é só pra copiar de volta para o original
    for (int i = 0; i < count; i++) {
        (*contas)[i] = temp[i];
    }
    
    *totalContas = count;

    free(temp);
    // Aqui é só pra copiar de volta para o original
}

void salvarDadosArquivo(struct Conta *contas, int totalContas, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "wb");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        exit(EXIT_FAILURE);
    }

    fwrite(&totalContas, sizeof(int), 1, arquivo);

    fwrite(contas, sizeof(struct Conta), totalContas, arquivo);

    fclose(arquivo);
}

void carregarDadosArquivo(struct Conta **contas, int *totalContas, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "rb");

    if (arquivo == NULL) {
        printf("Arquivo não encontrado. Criando novo arquivo.\n");
        return;
    }

    // HEAD
    fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    fread(totalContas, sizeof(int), 1, arquivo);

    *contas = malloc(*totalContas * sizeof(struct Conta));

    if (*contas == NULL) {
        printf("Erro ao alocar memória para contas.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *totalContas; i++) {
        fread(&((*contas)[i]), sizeof(struct Conta), 1, arquivo);

        (*contas)[i].nome[strcspn((*contas)[i].nome, "\n")] = '\0';
        (*contas)[i].numeroConta[strcspn((*contas)[i].numeroConta, "\n")] = '\0';
        (*contas)[i].cpf[strcspn((*contas)[i].cpf, "\n")] = '\0';
        (*contas)[i].telefone[strcspn((*contas)[i].telefone, "\n")] = '\0';
    }

    fclose(arquivo);
}

int buscaConta(struct Conta *contas, int totalContas, const char *numeroContaBusca) {
    if (contas == NULL) {
        return -2;
    }

    for (int i = 0; i < totalContas; i++) {
        if (strcmp(contas[i].numeroConta, numeroContaBusca) == 0) {
            return i;
        }
    }

    return -1; 
}


