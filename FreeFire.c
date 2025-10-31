#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20
#define BUFFER_SIZE 100

/* Struct Item:
   Armazena informações essenciais de cada objeto coletado:
   - nome: nome do item (ex: "Pistola")
   - tipo: tipo do item (ex: "arma", "munição", "cura")
   - quantidade: quantidade do item (inteiro)
*/
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

/* Função auxiliar:
   remove o '\n' deixado por fgets() ao final da string (se existir)
*/
void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

/* Busca sequencial por nome:
   Retorna o índice do item no vetor (0..count-1) ou -1 se não encontrado.
   A comparação é case-sensitive; se quiser case-insensitive, use strcasecmp.
*/
int buscarItem(Item itens[], int count, const char *nome) {
    for (int i = 0; i < count; i++) {
        if (strcmp(itens[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

/* Lista todos os itens registrados:
   Exibe índice, nome, tipo e quantidade. Chamado após cada operação para feedback.
*/
void listarItens(Item itens[], int count) {
    printf("\n--- Conteúdo da mochila (%d/%d) ---\n", count, MAX_ITENS);
    if (count == 0) {
        printf("Mochila vazia.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
                   i + 1, itens[i].nome, itens[i].tipo, itens[i].quantidade);
        }
    }
    printf("-----------------------------------\n");
}

/* Inserir item:
   - Se existir um item com o mesmo nome, soma a quantidade informada.
   - Se não existir e houver espaço, adiciona novo item.
   - Se estiver cheio, informa o usuário.
*/
void inserirItem(Item itens[], int *count) {
    if (*count >= MAX_ITENS) {
        printf("Mochila cheia! Remova um item antes de inserir.\n");
        listarItens(itens, *count);
        return;
    }

    char buffer[BUFFER_SIZE];
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade = 0;

    printf("Informe o nome do item: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
    trim_newline(buffer);
    strncpy(nome, buffer, MAX_NOME - 1);
    nome[MAX_NOME - 1] = '\0';

    printf("Informe o tipo do item (ex: arma, munição, cura): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
    trim_newline(buffer);
    strncpy(tipo, buffer, MAX_TIPO - 1);
    tipo[MAX_TIPO - 1] = '\0';

    printf("Informe a quantidade (numero inteiro): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
    quantidade = atoi(buffer);
    if (quantidade <= 0) {
        printf("Quantidade inválida. Cadastro cancelado.\n");
        return;
    }

    int idx = buscarItem(itens, *count, nome);
    if (idx != -1) {
        // Item já existe: soma a quantidade
        itens[idx].quantidade += quantidade;
        printf("Item '%s' já existe. Quantidade atualizada para %d.\n",
               itens[idx].nome, itens[idx].quantidade);
    } else {
        // Novo item
        strncpy(itens[*count].nome, nome, MAX_NOME);
        strncpy(itens[*count].tipo, tipo, MAX_TIPO);
        itens[*count].quantidade = quantidade;
        (*count)++;
        printf("Item '%s' cadastrado com sucesso.\n", nome);
    }

    listarItens(itens, *count);
}

/* Remover item por nome:
   - Procura o item; se encontrado, remove e desloca o restante do vetor.
   - Se não encontrado, informa o usuário.
*/
void removerItem(Item itens[], int *count) {
    if (*count == 0) {
        printf("Mochila vazia. Nada a remover.\n");
        return;
    }

    char buffer[BUFFER_SIZE];
    char nome[MAX_NOME];

    printf("Informe o nome do item a remover: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
    trim_newline(buffer);
    strncpy(nome, buffer, MAX_NOME - 1);
    nome[MAX_NOME - 1] = '\0';

    int idx = buscarItem(itens, *count, nome);
    if (idx == -1) {
        printf("Item '%s' não encontrado na mochila.\n", nome);
    } else {
        // Remove deslocando os elementos à direita
        for (int i = idx; i < (*count) - 1; i++) {
            itens[i] = itens[i + 1];
        }
        (*count)--;
        printf("Item '%s' removido com sucesso.\n", nome);
    }

    listarItens(itens, *count);
}

/* Buscar item:
   - Faz busca sequencial por nome e exibe os dados caso encontrado.
*/
void buscarEExibirItem(Item itens[], int count) {
    if (count == 0) {
        printf("Mochila vazia. Nada a buscar.\n");
        return;
    }

    char buffer[BUFFER_SIZE];
    char nome[MAX_NOME];

    printf("Informe o nome do item a buscar: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
    trim_newline(buffer);
    strncpy(nome, buffer, MAX_NOME - 1);
    nome[MAX_NOME - 1] = '\0';

    int idx = buscarItem(itens, count, nome);
    if (idx == -1) {
        printf("Item '%s' não encontrado.\n", nome);
    } else {
        printf("Item encontrado:\n");
        printf("Nome: %s\n", itens[idx].nome);
        printf("Tipo: %s\n", itens[idx].tipo);
        printf("Quantidade: %d\n", itens[idx].quantidade);
    }

    listarItens(itens, count);
}

/* Mostra o menu principal */
void mostrarMenu() {
    printf("\n=== Sistema de Inventário (Mochila) ===\n");
    printf("1) Inserir item\n");
    printf("2) Remover item\n");
    printf("3) Buscar item por nome\n");
    printf("4) Listar itens\n");
    printf("5) Sair\n");
    printf("Escolha uma opção: ");
}

/* Função principal:
   - Loop principal com menu para o jogador realizar operações.
   - Todas as operações chamam listarItens() ao final para feedback imediato.
*/
int main() {
    Item mochila[MAX_ITENS];
    int quantidadeItens = 0;
    char buffer[BUFFER_SIZE];
    int opcao = 0;

    printf("Bem-vindo ao sistema de inventário!\n");
    printf("Você pode cadastrar até %d itens na mochila.\n", MAX_ITENS);

    while (1) {
        mostrarMenu();
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        opcao = atoi(buffer);

        switch (opcao) {
            case 1:
                inserirItem(mochila, &quantidadeItens);
                break;
            case 2:
                removerItem(mochila, &quantidadeItens);
                break;
            case 3:
                buscarEExibirItem(mochila, quantidadeItens);
                break;
            case 4:
                listarItens(mochila, quantidadeItens);
                break;
            case 5:
                printf("Saindo... Até a próxima!\n");
                return 0;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}