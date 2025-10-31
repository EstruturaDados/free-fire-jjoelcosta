/*
  inventario_mestre_compacto.c
  Versão mais enxuta do "Nível Mestre" - ordenações, contagem de comparações,
  medição de tempo e busca binária (fgets, até 20 componentes).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_COMPONENTES 20
#define MAX_NOME 30
#define MAX_TIPO 20
#define BUF 128

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade; /* 1..10 */
} Componente;

void trim(char *s){ size_t n=strlen(s); if(n && s[n-1]=='\n') s[n-1]=0; }
void pausa(){ char b[BUF]; printf("\nPressione Enter para continuar..."); fgets(b,BUF,stdin); }
int ci_cmp(const char *a,const char *b){ char A[MAX_NOME],B[MAX_NOME]; for(int i=0;a[i]&&i<MAX_NOME-1;i++) A[i]=tolower((unsigned char)a[i]); A[strlen(a)<'\0'?strlen(a):MAX_NOME-1]=0; for(int i=0;b[i]&&i<MAX_NOME-1;i++) B[i]=tolower((unsigned char)b[i]); B[strlen(b)<'\0'?strlen(b):MAX_NOME-1]=0; return strcmp(A,B); }

/* display */
void mostrar(Componente v[], int n){
    printf("\n--- COMPONENTES (%d) ---\n", n);
    printf("%-30s | %-15s | %s\n","NOME","TIPO","PRIORIDADE");
    printf("---------------------------------------------------------------\n");
    for(int i=0;i<n;i++) printf("%-30s | %-15s | %10d\n", v[i].nome, v[i].tipo, v[i].prioridade);
    printf("---------------------------------------------------------------\n");
}

/* sorts (contam comparacoes via ponteiro) */
void bubble_nome(Componente a[], int n, long *cmp){
    for(int i=0;i<n-1;i++){ bool t=false;
        for(int j=0;j<n-1-i;j++){
            (*cmp)++;
            if(ci_cmp(a[j].nome,a[j+1].nome)>0){ Componente tmp=a[j]; a[j]=a[j+1]; a[j+1]=tmp; t=true; }
        }
        if(!t) break;
    }
}
void insertion_tipo(Componente a[], int n, long *cmp){
    for(int i=1;i<n;i++){
        Componente key=a[i]; int j=i-1;
        while(j>=0){ (*cmp)++; if(strcmp(a[j].tipo,key.tipo)>0){ a[j+1]=a[j]; j--; } else break; }
        a[j+1]=key;
    }
}
void selection_prior(Componente a[], int n, long *cmp){
    for(int i=0;i<n-1;i++){
        int im=i;
        for(int j=i+1;j<n;j++){ (*cmp)++; if(a[j].prioridade>a[im].prioridade) im=j; }
        if(im!=i){ Componente tmp=a[i]; a[i]=a[im]; a[im]=tmp; }
    }
}

/* busca binaria case-insensitive (assume ordenado por nome) */
int busca_binaria(Componente a[], int n, const char *nome, long *cmp){
    int l=0,r=n-1;
    while(l<=r){
        int m=l+(r-l)/2;
        (*cmp)++;
        int c=ci_cmp(a[m].nome,nome);
        if(c==0) return m;
        else if(c<0) l=m+1;
        else r=m-1;
    }
    return -1;
}

/* medidor simples */
double medir(void (*alg)(Componente*,int,long*), Componente arr[], int n, long *cmp){
    clock_t s=clock();
    alg(arr,n,cmp);
    clock_t e=clock();
    return (double)(e-s)/CLOCKS_PER_SEC;
}

/* leitura */
void ler_str(const char *prompt, char *out, int size){
    char buf[BUF];
    while(1){
        printf("%s",prompt);
        if(!fgets(buf,BUF,stdin)){ buf[0]=0; }
        trim(buf);
        if(strlen(buf)==0) { printf("Entrada vazia. Tente novamente.\n"); continue; }
        strncpy(out,buf,size-1); out[size-1]=0; break;
    }
}
int ler_int(const char *prompt){
    char buf[BUF]; long v; char *e;
    while(1){
        printf("%s",prompt);
        if(!fgets(buf,BUF,stdin)) return 0;
        trim(buf);
        v=strtol(buf,&e,10);
        if(e==buf || *e) { printf("Numero invalido. Tente novamente.\n"); continue; }
        return (int)v;
    }
}

/* programa principal, enxuto */
int main(void){
    Componente lista[MAX_COMPONENTES];
    int n=0; bool ordenadaPorNome=false;
    for(;;){
        printf("\n=== MONTAGEM TOWER - NÍVEL MESTRE (ENXUTO) ===\n");
        printf("Componentes: %d/%d | Ordenada por nome: %s\n", n, MAX_COMPONENTES, ordenadaPorNome?"SIM":"NAO");
        printf("1-Add 2-Rem 3-List 4-Ordenar 5-Buscar 0-Sair\nEscolha: ");
        char opt[BUF]; if(!fgets(opt,BUF,stdin)) break; trim(opt); if(strlen(opt)==0) continue;
        int opc = (int)strtol(opt,NULL,10);
        if(opc==0) { printf("Saindo...\n"); break; }
        if(opc==1){
            if(n>=MAX_COMPONENTES){ printf("Limite atingido.\n"); pausa(); continue; }
            Componente c; ler_str("Nome: ", c.nome, MAX_NOME); ler_str("Tipo: ", c.tipo, MAX_TIPO);
            int p; while((p=ler_int("Prioridade (1..10): "))<1 || p>10) printf("Valor entre 1 e 10.\n");
            c.prioridade=p;
            /* checa duplicata (ci) */
            int idx=-1;
            for(int i=0;i<n;i++) if(ci_cmp(lista[i].nome,c.nome)==0){ idx=i; break; }
            if(idx!=-1){
                printf("Componente existe. Substituir? 1-Sim/0-Nao: ");
                char r[BUF]; fgets(r,BUF,stdin); if(strtol(r,NULL,10)==1){ lista[idx]=c; ordenadaPorNome=false; printf("Substituido.\n"); }
                else printf("Cancelado.\n");
            } else { lista[n++]=c; ordenadaPorNome=false; printf("Adicionado.\n"); }
            pausa();
        } else if(opc==2){
            if(n==0){ printf("Nada a remover.\n"); pausa(); continue; }
            char nome[MAX_NOME]; ler_str("Nome a remover: ", nome, MAX_NOME);
            int idx=-1; for(int i=0;i<n;i++) if(ci_cmp(lista[i].nome,nome)==0){ idx=i; break; }
            if(idx==-1) printf("Nao encontrado.\n"); else { for(int j=idx;j<n-1;j++) lista[j]=lista[j+1]; n--; ordenadaPorNome=false; printf("Removido.\n"); }
            pausa();
        } else if(opc==3){
            mostrar(lista,n); pausa();
        } else if(opc==4){
            if(n==0){ printf("Nada para ordenar.\n"); pausa(); continue; }
            printf("1-Bubble(nome) 2-Insertion(tipo) 3-Selection(prioridade)\nEscolha: ");
            char a[BUF]; if(!fgets(a,BUF,stdin)) continue; int alg=strtol(a,NULL,10);
            if(alg<1||alg>3){ printf("Opcao invalida.\n"); pausa(); continue; }
            Componente copia[MAX_COMPONENTES]; memcpy(copia,lista,sizeof(Componente)*n);
            long cmp=0; double t=0;
            if(alg==1) t = medir(bubble_nome, copia, n, &cmp);
            else if(alg==2) t = medir(insertion_tipo, copia, n, &cmp);
            else t = medir(selection_prior, copia, n, &cmp);
            printf("Ordenacao feita. Comparacoes: %ld Tempo: %.6f s\n", cmp, t);
            mostrar(copia,n);
            printf("Aplicar ordenacao? 1-Sim/0-Nao: "); char r[BUF]; fgets(r,BUF,stdin);
            if(strtol(r,NULL,10)==1){ memcpy(lista,copia,sizeof(Componente)*n); ordenadaPorNome = (alg==1); printf("Aplicada.\n"); }
            else printf("Nao aplicada.\n");
            pausa();
        } else if(opc==5){
            if(n==0){ printf("Vazio.\n"); pausa(); continue; }
            if(!ordenadaPorNome){
                printf("Lista precisa estar ordenada por nome. Ordenar agora? 1-Sim/0-Nao: ");
                char r[BUF]; fgets(r,BUF,stdin); if(strtol(r,NULL,10)==1){ long c=0; clock_t s=clock(); bubble_nome(lista,n,&c); clock_t e=clock(); printf("Ordenado. cmp=%ld t=%.6f s\n", c, (double)(e-s)/CLOCKS_PER_SEC); ordenadaPorNome=true; } else { printf("Busca cancelada.\n"); pausa(); continue; }
            }
            char chave[MAX_NOME]; ler_str("Nome a buscar: ", chave, MAX_NOME);
            long cmp=0; clock_t s=clock(); int idx = busca_binaria(lista,n,chave,&cmp); clock_t e=clock();
            if(idx==-1) printf("Nao encontrado. cmp=%ld tempo=%.6f s\n", cmp, (double)(e-s)/CLOCKS_PER_SEC);
            else { printf("Encontrado idx=%d\nNome: %s\nTipo: %s\nPrioridade: %d\n", idx, lista[idx].nome, lista[idx].tipo, lista[idx].prioridade); printf("cmp=%ld tempo=%.6f s\n", cmp, (double)(e-s)/CLOCKS_PER_SEC); }
            pausa();
        } else {
            printf("Opcao invalida.\n"); pausa();
        }
    }
    return 0;
}