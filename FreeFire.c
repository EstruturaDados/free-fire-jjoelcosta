#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_COMPONENTES 20
#define MAX_NOME 30
#define MAX_TIPO 20
#define BUFFER_SIZE 128

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade; /* 1..10 */
} Componente;

void trim_newline(char *s) {
    size_t l = strlen(s);
    if (l > 0 && s[l-1] == '\n') s[l-1] = '\0';
}

void to_lower_copy(const char *src, char *dst, size_t dst_size) {
    size_t i;
    for (i = 0; i + 1 < dst_size && src[i] != '\0'; ++i) {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

int cmp_nome_case_insensitive(const char *a, const char *b) {
    char la[MAX_NOME], lb[MAX_NOME];
    to_lower_copy(a, la, sizeof(la));
    to_lower_copy(b, lb, sizeof(lb));
    return strcmp(la, lb);
}

void pausa_enter() {
    char buf[BUFFER_SIZE];
    printf("\nPressione Enter para continuar...");
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
 
    }
}

void mostrarComponentes(Componente arr[], int n) {
    printf("\n--- COMPONENTES (%d) ---\n", n);
    printf("---------------------------------------------------------------\n");
    printf("%-30s | %-15s | %10s\n", "NOME", "TIPO", "PRIORIDADE");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < n; ++i) {
        printf("%-30s | %-15s | %10d\n", arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
    printf("---------------------------------------------------------------\n");
}


void copiarComponentes(Componente dst[], Componente src[], int n) {
    for (int i = 0; i < n; ++i) dst[i] = src[i];
}

void bubbleSortNome(Componente arr[], int n, long *comparacoes) {
    bool trocou;
    for (int i = 0; i < n - 1; ++i) {
        trocou = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            (*comparacoes)++;
            if (cmp_nome_case_insensitive(arr[j].nome, arr[j+1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = true;
            }
        }
        if (!trocou) break;
    }
}


void insertionSortTipo(Componente arr[], int n, long *comparacoes) {
    for (int i = 1; i < n; ++i) {
        Componente chave = arr[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else break;
        }
        arr[j+1] = chave;
    }
}

void selectionSortPrioridade(Componente arr[], int n, long *comparacoes) {
    for (int i = 0; i < n - 1; ++i) {
        int idxMax = i;
        for (int j = i + 1; j < n; ++j) {
            (*comparacoes)++;
            if (arr[j].prioridade > arr[idxMax].prioridade) {
                idxMax = j;
            }
        }
        if (idxMax != i) {
            Componente tmp = arr[i];
            arr[i] = arr[idxMax];
            arr[idxMax] = tmp;
        }
    }
}

int buscaBinariaPorNome(Componente arr[], int n, const char *nome, long *comparacoes) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        (*comparacoes)++;
        int cmp = cmp_nome_case_insensitive(arr[mid].nome, nome);
        if (cmp == 0) return mid;
        if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

double medirTempo(void (*algoritmo)(Componente[], int, long*),
                   Componente arr[], int n, long *comparacoes) {
    clock_t inicio = clock();
    algoritmo(arr, n, comparacoes);
    clock_t fim = clock();
    return (double)(fim - inicio) / (double)CLOCKS_PER_SEC;
}

void lerString(const char *prompt, char *dest, size_t dest_size) {
    char buf[BUFFER_SIZE];
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) {

            buf[0] = '\0';
        }
        trim_newline(buf);
        if (strlen(buf) == 0) {
            printf("Entrada vazia. Por favor, digite algo.\n");
            continue;
        }
        strncpy(dest, buf, dest_size - 1);
        dest[dest_size - 1] = '\0';
        break;
    }
}

int lerInteiro(const char *prompt) {
    char buf[BUFFER_SIZE];
    int val;
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        trim_newline(buf);
        if (strlen(buf) == 0) {
            printf("Entrada vazia. Tente novamente.\n");
            continue;
        }
        char *endptr;
        val = (int)strtol(buf, &endptr, 10);
        if (endptr == buf || *endptr != '\0') {
            printf("Entrada invalida. Digite um numero inteiro.\n");
            continue;
        }
        return val;
    }
}

void exibirMenu(bool ordenadaPorNome, int num) {
    printf("\n=============================================\n");
    printf("   MONTAGEM DA TORRE DE RESGATE - NÍVEL MESTRE\n");
    printf("=============================================\n");
    printf("Componentes cadastrados: %d/%d\n", num, MAX_COMPONENTES);
    printf("Ordenada por nome: %s\n\n", ordenadaPorNome ? "SIM" : "NAO");
    printf("1 - Cadastrar componente\n");
    printf("2 - Remover componente por nome\n");
    printf("3 - Listar componentes\n");
    printf("4 - Ordenar (bubble/insertion/selection)\n");
    printf("5 - Buscar componente-chave (busca binaria por nome)\n");
    printf("0 - Sair\n");
    printf("---------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

int main(void) {
    Componente componentes[MAX_COMPONENTES];
    int num = 0;
    bool ordenadaPorNome = false;

    for (int i = 0; i < MAX_COMPONENTES; ++i) {
        componentes[i].nome[0] = '\0';
        componentes[i].tipo[0] = '\0';
        componentes[i].prioridade = 0;
    }

    char opt_buf[BUFFER_SIZE];
    while (1) {
        exibirMenu(ordenadaPorNome, num);
        if (fgets(opt_buf, sizeof(opt_buf), stdin) == NULL) break;
        trim_newline(opt_buf);
        if (strlen(opt_buf) == 0) continue;
        int opc = (int)strtol(opt_buf, NULL, 10);

        if (opc == 0) {
            printf("\nSaindo... Boa sorte na fuga!\n");
            break;
        }

        if (opc == 1) {

            if (num >= MAX_COMPONENTES) {
                printf("\nLimite atingido (%d componentes). Remova antes de adicionar.\n", MAX_COMPONENTES);
                pausa_enter();
                continue;
            }
            Componente novo;
            lerString("\nNome do componente: ", novo.nome, sizeof(novo.nome));
            lerString("Tipo do componente (ex: controle, suporte, propulsao): ", novo.tipo, sizeof(novo.tipo));
            int pri;
            while (1) {
                pri = lerInteiro("Prioridade (1..10): ");
                if (pri >= 1 && pri <= 10) break;
                printf("Prioridade invalida. Digite um valor entre 1 e 10.\n");
            }
            novo.prioridade = pri;

    
            int idxDup = -1;
            for (int i = 0; i < num; ++i) {
                if (cmp_nome_case_insensitive(componentes[i].nome, novo.nome) == 0) {
                    idxDup = i;
                    break;
                }
            }
            if (idxDup != -1) {
                printf("\nComponente com mesmo nome ja existe: '%s'.\n", componentes[idxDup].nome);
                printf("Deseja substituir? (1-Sim / 0-Nao): ");
                char resp_buf[BUFFER_SIZE];
                if (fgets(resp_buf, sizeof(resp_buf), stdin) != NULL) {
                    trim_newline(resp_buf);
                    int r = (int)strtol(resp_buf, NULL, 10);
                    if (r == 1) {
                        componentes[idxDup] = novo;
                        printf("Componente substituido com sucesso.\n");
                        ordenadaPorNome = false;
                    } else {
                        printf("Operacao cancelada. Nenhuma alteracao feita.\n");
                    }
                } else {
                    printf("Entrada falhou. Cancelado.\n");
                }
            } else {
                componentes[num++] = novo;
                printf("\nComponente '%s' adicionado com sucesso.\n", novo.nome);
                ordenadaPorNome = false;
            }
            pausa_enter();
        }
        else if (opc == 2) {
        
            if (num == 0) {
                printf("\nNenhum componente para remover.\n");
                pausa_enter();
                continue;
            }
            char nomeRem[MAX_NOME];
            lerString("\nDigite o nome do componente a remover: ", nomeRem, sizeof(nomeRem));
            int idx = -1;
            for (int i = 0; i < num; ++i) {
                if (cmp_nome_case_insensitive(componentes[i].nome, nomeRem) == 0) {
                    idx = i; break;
                }
            }
            if (idx == -1) {
                printf("\nComponente '%s' nao encontrado.\n", nomeRem);
            } else {
                for (int j = idx; j < num - 1; ++j) componentes[j] = componentes[j+1];
                num--;
                printf("\nComponente '%s' removido com sucesso.\n", nomeRem);
                ordenadaPorNome = false;
            }
            pausa_enter();
        }
        else if (opc == 3) {
            mostrarComponentes(componentes, num);
            pausa_enter();
        }
        else if (opc == 4) {
            if (num == 0) {
                printf("\nNada para ordenar (lista vazia).\n");
                pausa_enter();
                continue;
            }
            printf("\n--- MENU DE ORDENACAO ---\n");
            printf("1 - Bubble Sort por Nome (alfabetico)\n");
            printf("2 - Insertion Sort por Tipo (alfabetico)\n");
            printf("3 - Selection Sort por Prioridade (maior primeiro)\n");
            printf("Escolha o algoritmo: ");
            char alg_buf[BUFFER_SIZE];
            if (fgets(alg_buf, sizeof(alg_buf), stdin) == NULL) continue;
            int alg = (int)strtol(alg_buf, NULL, 10);
            if (alg < 1 || alg > 3) {
                printf("Opcao invalida.\n");
                pausa_enter();
                continue;
            }

            Componente copia[MAX_COMPONENTES];
            copiarComponentes(copia, componentes, num);

            long comparacoes = 0;
            double tempo = 0.0;

            if (alg == 1) {
                printf("\nExecutando Bubble Sort por nome...\n");
                comparacoes = 0;
                tempo = medirTempo(bubbleSortNome, copia, num, &comparacoes);
            } else if (alg == 2) {
                printf("\nExecutando Insertion Sort por tipo...\n");
                comparacoes = 0;
                tempo = medirTempo(insertionSortTipo, copia, num, &comparacoes);
            } else {
                printf("\nExecutando Selection Sort por prioridade...\n");
                comparacoes = 0;
                tempo = medirTempo(selectionSortPrioridade, copia, num, &comparacoes);
            }

            printf("\nOrdenacao concluida. Comparacoes: %ld. Tempo: %.6f s (%.3f ms)\n",
                   comparacoes, tempo, tempo * 1000.0);
            mostrarComponentes(copia, num);

            printf("\nDeseja aplicar esta ordenacao à lista principal? (1-Sim / 0-Nao): ");
            char resp_apply[BUFFER_SIZE];
            if (fgets(resp_apply, sizeof(resp_apply), stdin) != NULL) {
                int r = (int)strtol(resp_apply, NULL, 10);
                if (r == 1) {
                    copiarComponentes(componentes, copia, num);
                    printf("Ordenacao aplicada.\n");
                    ordenadaPorNome = (alg == 1);
                } else {
                    printf("Ordenacao nao aplicada.\n");
                }
            } else {
                printf("Entrada falhou. Ordenacao nao aplicada.\n");
            }
            pausa_enter();
        }
        else if (opc == 5) {
            if (num == 0) {
                printf("\nNenhum componente cadastrado.\n");
                pausa_enter();
                continue;
            }
            if (!ordenadaPorNome) {
                printf("\nA busca binaria requer que a lista esteja ordenada por nome.\n");
                printf("Deseja ordenar por nome agora? (1-Sim / 0-Nao): ");
                char resp[BUFFER_SIZE];
                if (fgets(resp, sizeof(resp), stdin) == NULL) {
                    pausa_enter(); continue;
                }
                int r = (int)strtol(resp, NULL, 10);
                if (r == 1) {
                    long comps = 0;
                    clock_t start = clock();
                    bubbleSortNome(componentes, num, &comps);
                    clock_t end = clock();
                    double t = (double)(end - start) / (double)CLOCKS_PER_SEC;
                    printf("\nOrdenado por nome. Comparacoes: %ld. Tempo: %.6f s\n", comps, t);
                    ordenadaPorNome = true;
                    pausa_enter();
                } else {
                    printf("Busca binaria cancelada.\n");
                    pausa_enter();
                    continue;
                }
            }
            char nomeBusca[MAX_NOME];
            lerString("\nDigite o nome do componente-chave a buscar: ", nomeBusca, sizeof(nomeBusca));
            long compsBusca = 0;
            clock_t s = clock();
            int idx = buscaBinariaPorNome(componentes, num, nomeBusca, &compsBusca);
            clock_t e = clock();
            double tbusca = (double)(e - s) / (double)CLOCKS_PER_SEC;
            if (idx == -1) {
                printf("\nComponente '%s' nao encontrado (busca binaria). Comparacoes: %ld. Tempo: %.6f s\n",
                       nomeBusca, compsBusca, tbusca);
            } else {
                printf("\nComponente encontrado (indice %d):\n", idx);
                printf("Nome: %s\nTipo: %s\nPrioridade: %d\n", componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
                printf("Comparacoes na busca: %ld. Tempo: %.6f s\n", compsBusca, tbusca);
            }
            pausa_enter();
        }
        else {
            printf("\nOpcao invalida. Tente novamente.\n");
            pausa_enter();
        }
    } /* fim while */

    return 0;
}