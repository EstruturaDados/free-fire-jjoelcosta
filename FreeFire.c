/*
  inventario_novato.c
  Nível Novato - Sistema simples de inventário (mochila)

  Funcionalidades:
  - Adicionar itens (nome, tipo, quantidade)
  - Remover itens pelo nome
  - Listar itens cadastrados em formato de tabela
  - Uso de struct Item e vetor estático com capacidade para 10 itens
  - Menu interativo com do-while e switch
  - Leitura via scanf e escrita via printf

  Observações:
  - Para simplicidade e conforme solicitado, este arquivo NÃO utiliza
    ponteiros em assinaturas de função — as operações usam variáveis globais.
  - Se um item com o mesmo nome for inserido novamente, a quantidade é somada.
*/

#include <stdio.h>
#include <string.h>

#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

/* Struct que representa um item */
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

/* Vetor estático da mochila e contador global (evita passar ponteiros) */
Item mochila[MAX_ITENS];
int numItens = 0;

/* Mostra o menu principal */
void mostrarMenu(void) {
    printf("\n=== MOCHILA - NÍVEL NOVATO ===\n");
    printf("Itens na mochila: %d/%d\n", numItens, MAX_ITENS);
    printf("1 - Adicionar item\n");
    printf("2 - Remover item\n");
    printf("3 - Listar itens\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}