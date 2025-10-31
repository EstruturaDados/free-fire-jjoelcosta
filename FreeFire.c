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