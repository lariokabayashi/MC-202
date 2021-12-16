#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct navio{
    char nome[20];
    char operacao[11];
    char tipo[20];
    int num_merc;
    int mudanca;
}navio;

typedef struct No {
    char dado[20];
    struct No *prox;
} No;
typedef struct No * p_no;


typedef struct fila { 
    p_no ini, fim;
    int tam;
} fila;
typedef struct fila * p_fila;

typedef struct {
    p_no topo;
    int tam;
} Pilha;
typedef Pilha * p_pilha;

p_fila criar_fila() {
    p_fila f;
    f = malloc(sizeof(No));
    f->ini = NULL;
    f->fim = NULL;
    f->tam = 0;
    return f;
}
p_pilha criar_pilha(){
    p_pilha p;
    p = malloc(sizeof(Pilha));
    p->topo = NULL;
    p->tam = 0;
    return p;
}

void enfileira(p_fila f, char nome[]) {
    p_no novo;
    novo = malloc(sizeof(No));
    strcpy(novo->dado, nome);
    novo->prox = NULL;
    if (f->ini == NULL)
        f->ini = novo;
    else{
        f->fim->prox = novo;
    }
    f->fim = novo;
    f->tam += 1;
}
void desenfileira(p_fila f) {
    p_no primeiro = f->ini;
    f->ini = f->ini->prox;
    if (f->ini == NULL)
        f->fim = NULL;
    free(primeiro); // Para eliminar corretamente todas as variáveis no valgrind
    f->tam -= 1;
}

int empilhar(p_pilha pilha, int num_merc, char tipo[20], int cap_doca) {
    int n = 0;
    while (n != num_merc && pilha->tam != cap_doca){
        p_no novo = malloc(sizeof(No));
        strcpy(novo->dado, tipo);
        novo->prox = pilha->topo;
        pilha->topo = novo;
        pilha->tam += 1;
        n++;
    }
    return n;
}
int desimpilhar(p_pilha pilha, int num_merc, char tipo[]){
    int n = 0;
    while (n != num_merc && pilha->topo != NULL){ 
        if (strcmp(pilha->topo->dado, tipo) == 0){
            p_no primeiro = pilha->topo;
            pilha->topo = pilha->topo->prox;
            free(primeiro); // Para eliminar corretamente todas as variáveis no valgrind
            n++;
            pilha->tam -= 1;
        }
        else
            return n;
    }
    return n;
}

int encontra_doca(char tipo[], p_pilha *pilha, int n_docas){
    // vê se tem doca com o produto que o navio precisa ser carregado
    int i;
    for (i = 0; i < n_docas; i++){
        if (pilha[i] == NULL)
            continue;
        else if (pilha[i]->tam == 0)
            continue;
        else if (strcmp(pilha[i]->topo->dado, tipo) == 0)
            return i;
    }
    return -1;
}
int doca_disp(p_pilha *pilhas, int n_docas, int cap){
    // vê se tem doca com espaço disponivel
    int i = 0;
    if (pilhas[0] == NULL)
        return 0;
    else{
        while (i != n_docas){
            if (pilhas[i]->tam == cap)
                i += 1;
            else
                return i;
        }   
    }
    return -1;
}

int encontre_navio(navio *seq_navios, char nome[], int num_navios){
    // procuramos o index do primeiro da fila de navios
    //no array com info dos navios 
    int i = 0;
    for (i = 0; i < num_navios; i++){
        if (strcmp(seq_navios[i].nome, nome) == 0)
            return i;
    }
    return -1;
}
int fila_esvaziavel(p_no fila, navio *seq_navios, int num_navios){
    // ve se tem como esvaziar a fila
    //para isso temos que ter pelo menos uma mudança
    int n;
    p_no aux = fila;
    while(aux != NULL){
        n = encontre_navio(seq_navios, aux->dado, num_navios); 
        if (seq_navios[n].mudanca != -1){
            return 0;
        }
        aux = aux->prox;
    }
    return -1;
}

void destruir_lista(p_no lista) {
    if (lista != NULL) {
        destruir_lista(lista->prox);
        free(lista);
    }
}

void destruir_fila(p_fila f) {
    destruir_lista(f->ini);
    free(f);
}

void destruir_pilha(p_pilha p){
    destruir_lista(p->topo);
    free(p);
}


int main(){
    int n_docas, cap, n_navios, d, n, i, cont = 0;
    p_fila fila_navios;
    p_pilha *docas;
    navio *seq_navios;
    fila_navios = criar_fila();
    scanf("%d", &n_docas);
    docas = calloc(n_docas, sizeof(p_pilha));
    for (i = 0; i < n_docas; i++){
        docas[i] =  criar_pilha();
    }
    scanf("%d", &cap);
    scanf("%d", &n_navios);
    seq_navios = calloc(n_navios, sizeof(navio));
    
    i = 0;
    while (i != n_navios){
        navio n;
        scanf("%s %s %s %d", n.nome, n.operacao, n.tipo, &n.num_merc);
        n.mudanca = 0;
        seq_navios[i] = n;
        enfileira(fila_navios, n.nome);
        i++;
    }

    while (fila_navios->tam != 0){
        i = encontre_navio(seq_navios, fila_navios->ini->dado, n_navios);
        if (strcmp(seq_navios[i].operacao, "carrega") == 0){
            d = encontra_doca(seq_navios[i].tipo, docas, n_docas);
            if (d != -1)
                n = desimpilhar(docas[d], seq_navios[i].num_merc, seq_navios[i].tipo);
        }
        else{ //descarregar
            d  = doca_disp(docas, n_docas, cap);
            if (d != -1)
                n = empilhar(docas[d], seq_navios[i].num_merc, seq_navios[i].tipo, cap);
        }

        if (d != -1){
            if (n != seq_navios[i].num_merc){
                seq_navios[i].num_merc -= n;
                seq_navios[i].mudanca = 0;
                enfileira(fila_navios, seq_navios[i].nome);
            }
            printf("%s %s %s doca: %d conteineres: %d\n", seq_navios[i].nome, seq_navios[i].operacao, seq_navios[i].tipo, d, n);
            cont = 0; //como houve uma mudança na fila de navios o contador volta a ser zero
        }
        else{ //d = -1 não há ou doca disp ou não encontra doca cm mercadoria
            enfileira(fila_navios, seq_navios[i].nome);
            seq_navios[i].mudanca = -1;
        }
        desenfileira(fila_navios);

        //checando se é esvaziavel
        if (fila_navios->tam > 0 && fila_esvaziavel(fila_navios->ini, seq_navios, n_navios) == -1){
            //esperar eles entrarem nas funções novamente e ver se não acontece nada
            if (cont > fila_navios->tam){ //tds os navios terão entrado na função quando o contador se igualar ao tam da fila
                printf("ALERTA: impossivel esvaziar fila, restam %d navios.", fila_navios->tam);
                while (fila_navios->tam!= 0){
                    desenfileira(fila_navios);
                }
                return 0;
            }
            else
                cont += 1;
        }
    }

    // Liberando as estruturas filas e pilhas, como também as listas e vetores que os compõem
    for (i = 0; i < n_docas; i++)
        destruir_pilha(docas[i]);
    free(docas);

    destruir_fila(fila_navios);
    free(seq_navios);

    return 0;
}