#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 1000
#define MAX_NOME 100

typedef struct {
    char nome[MAX_NOME];
    int quantidade;
} Produto;

int carregarEstoque(Produto estoque[]) {
    FILE *arquivo = fopen("inventory.txt", "r");

    if (arquivo == NULL) {
        return 0;
    }

    int total = 0;

    while (total < MAX_ITENS &&
           fgets(estoque[total].nome, MAX_NOME, arquivo) != NULL) {

        estoque[total].nome[strcspn(estoque[total].nome, "\n")] = '\0';

        if (fscanf(arquivo, "%d", &estoque[total].quantidade) == 1) {
            total++;
        } else {
            break;
        }

        int c;
        while ((c = fgetc(arquivo)) != '\n' && c != EOF);
    }

    fclose(arquivo);

    return total;
}

void salvarEstoque(Produto estoque[], int total) {
    FILE *arquivo = fopen("inventory.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "%s\n%d\n",
                estoque[i].nome,
                estoque[i].quantidade);
    }

    fclose(arquivo);
}

void adicionarItem() {
    Produto estoque[MAX_ITENS];
    int total = carregarEstoque(estoque);

    char nome[MAX_NOME];
    int quantidade;

    printf("\nDigite o nome do item: ");
    scanf(" %99[^\n]", nome);

    printf("Digite a quantidade: ");
    scanf("%d", &quantidade);

    if (quantidade <= 0) {
        printf("Quantidade inválida!\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        if (strcmp(estoque[i].nome, nome) == 0) {
            estoque[i].quantidade += quantidade;
            salvarEstoque(estoque, total);

            printf("Item adicionado com sucesso!\n");
            return;
        }
    }

    if (total >= MAX_ITENS) {
        printf("Limite de produtos atingido!\n");
        return;
    }

    strcpy(estoque[total].nome, nome);
    estoque[total].quantidade = quantidade;

    total++;

    salvarEstoque(estoque, total);

    printf("Item adicionado com sucesso!\n");
}

void removerItem() {
    Produto estoque[MAX_ITENS];
    int total = carregarEstoque(estoque);

    char nome[MAX_NOME];
    int quantidade;

    printf("\nDigite o nome do item: ");
    scanf(" %99[^\n]", nome);

    printf("Digite a quantidade a ser removida: ");
    scanf("%d", &quantidade);

    if (quantidade <= 0) {
        printf("Quantidade inválida!\n");
        return;
    }

    int encontrado = 0;

    for (int i = 0; i < total; i++) {

        if (strcmp(estoque[i].nome, nome) == 0) {

            encontrado = 1;

            if (quantidade > estoque[i].quantidade) {
                printf("Estoque insuficiente. Quantidade disponível: %d\n",
                       estoque[i].quantidade);
                return;
            }

            estoque[i].quantidade -= quantidade;

            if (estoque[i].quantidade == 0) {

                for (int j = i; j < total - 1; j++) {
                    estoque[j] = estoque[j + 1];
                }

                total--;

                salvarEstoque(estoque, total);

                printf("Item removido do estoque!\n");

            } else {

                salvarEstoque(estoque, total);

                printf("Quantidade atualizada com sucesso!\n");
            }

            return;
        }
    }

    if (!encontrado) {
        printf("Item não encontrado.\n");
    }
}

void listarEstoque() {
    Produto estoque[MAX_ITENS];

    int total = carregarEstoque(estoque);

    printf("\n=============================\n");
    printf("        Estoque Atual\n");
    printf("=============================\n");

    if (total == 0) {
        printf("O estoque está vazio.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        printf("Nome: %s\n", estoque[i].nome);
        printf("Quantidade: %d\n\n", estoque[i].quantidade);
    }
}

int main() {

    int opcao;
    char continuar;

    do {

        printf("\n==================================\n");
        printf("       Controle de Estoque\n");
        printf("==================================\n");

        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Estoque\n");
        printf("4. Sair\n");

        printf("Opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Opção inválida! Tente novamente.\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            continue;
        }

        if (opcao < 1 || opcao > 4) {
            printf("Opção inválida! Tente novamente.\n");
            continue;
        }

        switch (opcao) {

            case 1:
                adicionarItem();
                break;

            case 2:
                removerItem();
                break;

            case 3:
                listarEstoque();
                break;

            case 4:
                printf("\nObrigado por usar o Controle de Estoques! Até a próxima.\n");
                return 0;
        }

        if (opcao >= 1 && opcao <= 3) {

            printf("\nDeseja realizar outra operação? (s/n): ");
            scanf(" %c", &continuar);

            if (continuar != 's' && continuar != 'S') {
                break;
            }
        }

    } while (1);

    printf("\nObrigado por usar o Controle de Estoques! Até a próxima.\n");

    return 0;
}
