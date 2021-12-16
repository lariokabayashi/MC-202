#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define infinite 1000000

typedef struct No {
    int v;
    float dist;
    struct No *prox;
} No;
typedef No * p_no;

typedef struct {
    p_no *adjacencia;
    int n; // numero de cidades 
    float fator_c[MAX]; //fator centralidade das cidades
    int pop[MAX]; // população dos vertices (cidades)
    char nome[MAX][30]; //nome dos vertices (cidades)
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
float distancia(p_grafo g, int u, int v){
    p_no t;
    if (u == v)
        return 0; //a dist de uma cidade a si mesma é 0
    for (t = g->adjacencia[u]; t != NULL; t = t->prox){
        //achando a dist de v na lista de adjacencia
        if (t->v == v)
            return t->dist;
    }
    return infinite;
}

void insere_dist(p_grafo g, float dist, int u, int v){
    // insere distância na lista de adjacência
    p_no t;
    for (t = g->adjacencia[u]; t != NULL; t = t->prox){
        if (t->v == v)
            t->dist = dist;
    }
}

void Floyd_Warshall(p_grafo G){
    // implementação do algoritmo Floyd Warshall
    int k, i, j;
    float dist;
    for (k = 0; k < G->n; k++)
        for (i = 0; i < G->n; i++)
            if (i != k && distancia(G, i, k) < infinite){ 
                for(j = 0; j < G->n; j++){
                    if (distancia(G, i, j) > distancia(G, i, k) + distancia(G, k, j)){
                        dist = distancia(G, i, k) + distancia(G, k, j);
                        insere_dist(G, dist, i, j);
                    }
                }
            }
}

p_no insere_na_lista(p_no lista, int n, float dist) {
    p_no novo = malloc(sizeof(No));
    novo->v = n;
    novo->dist = dist;
    novo->prox = lista;
    return novo;
}

void criar_lista_adj(p_grafo g){
    int i, n;
    // cria lista de adjacẽncia inicializando as distâncias
    // como infinito, pois não sabemos se u tem ligação com v 
    for (i = 0; i < g-> n; i++)
        for (n = 0; n < g-> n; n++){
            if (i == n)
                g->adjacencia[i] = insere_na_lista(g->adjacencia[i], n, 0);
            else
                g->adjacencia[i] = insere_na_lista(g->adjacencia[i], n, infinite);
        }
}

void insere_aresta(p_grafo g, int u, int n, float dist) {
    insere_dist(g, dist, u, n);
    insere_dist(g, dist, n, u);
}

int encontra_i (p_grafo G, char nome[]){
    int i;
    for (i = 0; i < G->n; i++)
        if (strcmp(G->nome[i], nome) == 0)
            return i;
    return -1;
}

float fator_c (p_grafo g, int u){
    //calcula fator de centralidade
    int v;
    float d, C = 0, P = 0;
    p_no t;
    for (t = g->adjacencia[u]; t != NULL; t = t->prox){
        if (t->dist < infinite){
            d = t->dist;
            v = t->v;
            C += g->pop[v]*d;
            P += g->pop[v];
        }
    }
    return C/(float)P;
}
int alcance_cidade(p_grafo g, int u){
    int PopT = 0, p = 0, i = 0;
    p_no t; 

    // calculando pop total
    while(i != g->n){
        PopT += g->pop[i];
        i++;
    }

    // calculando pop que a cidade alcança
    for (t = g->adjacencia[u]; t != NULL; t = t->prox)
        if (t->dist < infinite)
            p += g->pop[t->v];

    if (p >= PopT/2)
        return 1;

    return -1;
}

int eh_repetido(int lista_pos[], int pos){
    int i;
    for (i = 0; lista_pos[i] != infinite; i++){
        if (lista_pos[i] == pos){
            return 1;
        }
    }
    return -1;
}

void imprime(p_grafo g){
    int i, ordem[MAX], pos, j, k;
    float min = infinite;
    //inicializando elem de ordem[i] com infinito
    for (i = 0; i < g->n; i++)
        ordem[i] = infinite;

    // ordenando as cidades por fator de centralidade
    for (j = 0; j < g->n; j++){
        for (i = 0; i < g->n; i++){
            if (g->fator_c[i] < min && eh_repetido(ordem, i) != 1){
                min = g->fator_c[i];
                pos = i;
            }
            else if (g->fator_c[i] == min && eh_repetido(ordem, i) != 1){
                // ordenar por ordem alfabetica
                if (strcmp(g->nome[i], g->nome[pos]) < 0){
                    min = g->fator_c[i];
                    pos = i;
                }
            }
        }
        // lista com cidades ordenadas
        ordem[j] = pos;
        min = infinite;
    }
    for (i = 0; i < g->n; i++){
        if (alcance_cidade(g, ordem[i]) == 1){
            k = ordem[i];
            printf("%s %.2f\n", g->nome[k], g->fator_c[k]*0.99999996);
        }    
    }
}

int main(){
    int i = 0, n, c1, c2, u, j = 0;
    float dist;
    char cidade1[50], cidade2[50];
    p_grafo estado;
    scanf("%d", &n);
    estado = criar_grafo(n);
    while (i != n){
        scanf("%s %d", estado->nome[i], &estado->pop[i]);
        i++;
    }

    criar_lista_adj(estado);

    while(scanf("%s", cidade1) != EOF){
        scanf ("%s %f", cidade2, &dist);
        c1 = encontra_i(estado, cidade1);
        c2 = encontra_i(estado, cidade2);
        if (c1 == c2)
            dist = 0;
        insere_aresta(estado, c1, c2, dist);
        j++;
    }

    Floyd_Warshall(estado);
    
    for (i = 0; i < estado->n; i++){
        u = encontra_i(estado, estado->nome[i]);
        estado->fator_c[u] = fator_c(estado, u);
    }
    imprime(estado);
    destroi_grafo(estado);
    return 0;
}