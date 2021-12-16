#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 400

typedef struct No {
    int v;
    struct No *prox;
} No;
typedef No * p_no;

typedef struct {
    p_no *adjacencia;
    int n;
    int atributos[MAX][6]; //numero de atributos dos vertices: 6
    char nome[MAX][30]; //nome dos vertices
} Grafo;
typedef Grafo * p_grafo;

p_grafo criar_grafo(int n) {
    int i;
    p_grafo g = malloc(sizeof(Grafo));
    g->n = n;
    g->adjacencia = malloc(n * sizeof(p_no));
    for (i = 0; i < n; i++)
        g->adjacencia[i] = NULL;
    return g;
}

void libera_lista(p_no lista) {
    if (lista != NULL) {
        libera_lista(lista->prox);
        free(lista);
    }
}

void destroi_grafo(p_grafo g) {
    int i;
    for (i = 0; i < g->n; i++)
        libera_lista(g->adjacencia[i]);
    free(g->adjacencia);
    free(g);
}

p_no insere_na_lista(p_no lista, int n) {
    p_no novo = malloc(sizeof(No));
    novo->v = n;
    novo->prox = lista;
    return novo;
}

void insere_aresta(p_grafo g, int u, int n) {
    g->adjacencia[u] = insere_na_lista(g->adjacencia[u], n);
    g->adjacencia[n] = insere_na_lista(g->adjacencia[n], u);
}

void cmp_atributos(p_grafo G, int atrib[], int n, int v1){
    int i, j = 0, falta = 0;
    for (i = 0; i < v1; i++){
        while(j != 6 && falta <= 2){
            //comparando atributos da cela com os do personagem
            if (atrib[j] - G->atributos[i][j] < 0)
                falta += abs (atrib[j] - G->atributos[i][j]);
            j++;
        }
        // se em duas rodadas o personagem consegue ficar
        // com o minimo de atributos possiveis, então insere-se aresta
        if (falta <= 2)
            insere_aresta(G, i, n);
        j = 0;
        falta = 0;
    }
}

int imprime(p_grafo g, int v2) {
    p_no t;
    int i, cont = 0, imprimiu = 0;
    for (i = 0; i < g->n; i++){
        for (t = g->adjacencia[i]; t != NULL; t = t->prox)
            cont++; //contando numero de elementos na lista
        if (cont == v2){
            printf("%s\n", g->nome[i]); 
            imprimiu = 1;
        }
        cont = 0;
    }
    return imprimiu;
}

int main(){
    int v1, v2, atrib[6], i = 0, j = 0;
    p_grafo entr_cela;
    entr_cela = criar_grafo(MAX); 
    scanf("%d", &v1);

    while(i != v1){
        scanf("%s", entr_cela->nome[i]);
        while (j != 6){
            scanf("%d", &entr_cela->atributos[i][j]);
            j++;
        }
        j = 0;
        i++;
    }

    scanf("%d", &v2);

    while(i != v1 + v2){
        scanf("%s", entr_cela->nome[i]);
        while (j != 6){
            scanf("%d", &atrib[j]);
            j++;
        }
        cmp_atributos(entr_cela, atrib, i, v1);
        j = 0;
        i++;
    }

    if (!imprime(entr_cela, v2))
        printf("Impossivel terminar em duas rodadas.");
    destroi_grafo(entr_cela);
    return 0;
}

