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

void listarContas(struct Conta *contas, int totalContas){
	
	for(int x = 0; x < totalContas; x++){
		printf("Número da conta: %s\n", contas[x].numeroConta);
		printf("Saldo: %.2f\n", contas[x].saldo);
		printf("Nome do Cliente: %s\n", contas[x].nome);
	}
	//return 0;
}

void consultarSaldo(struct Conta *contas, int totalContas, const char *numeroContaConsulta) {
    
    for (int i = 0; i < totalContas; i++) {
        if (strcmp(contas[i].numeroConta, numeroContaConsulta) == 0) {
            
            printf("Saldo da conta %s: %.2f\n", numeroContaConsulta, contas[i].saldo);
            return;
        }
    }

    
    printf("Conta %s nao encontrada.\n", numeroContaConsulta);
}

void salvarDadosArquivo(struct Conta *contas, int totalContas, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "wb");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        exit(EXIT_FAILURE);
    }

    // HEAD
    fwrite(&totalContas, sizeof(int), 1, arquivo);

    
    fwrite(contas, sizeof(struct Conta), totalContas, arquivo);

    fclose(arquivo);
}

void carregarDadosArquivo(struct Conta **contas, int *totalContas, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "rb");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        exit(EXIT_FAILURE);
    }

    // HEAD
    fread(totalContas, sizeof(int), 1, arquivo);

    // Aumentar array
    *contas = malloc(*totalContas * sizeof(struct Conta));

    
    if (*contas == NULL) {
        printf("Erro ao alocar memoria para contas.\n");
        exit(EXIT_FAILURE);
    }

    
    fread(*contas, sizeof(struct Conta), *totalContas, arquivo);

    fclose(arquivo);
}

int main() {
    struct Conta *contas = NULL;
    int totalContas = 0;
    char numeroContaConsulta[20];
    int opcao;

	
	do {
        // MENU
        printf("\n---- Menu ----\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Consultar Saldo\n");
        printf("3. Listar Contas\n");
        printf("4. Ler Arquivo\n");
        printf("5. Salvar Arquivo\n");
        printf("0. Sair\n");
        // MENU

        
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        
        switch (opcao) {
            case 1:
                cadastrarCliente(&contas, &totalContas);
                break;
            case 2:
                consultarSaldo(contas, totalContas, numeroContaConsulta);
                break;
            case 3:
            	listarContas(contas, totalContas);
            	break;
            case 4:
            	carregarDadosArquivo(&contas,  &totalContas, "cadastros.txt");
            	break;
            case 5:
            	salvarDadosArquivo(contas, totalContas, "cadastros.txt");
            	break;            		
            case 0:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 0);
	
    
    free(contas);

    return 0;
}
