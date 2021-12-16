#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    float valor;
    int codigo;
    char loja[15];
    struct No *esq, *dir; 
} No;
typedef No * p_no;

p_no inserir(p_no raiz, char loja[], int cod, float valor) {
    p_no novo;

    if (raiz == NULL) {
        novo = malloc(sizeof(No));
        novo->esq = novo->dir = NULL;
        novo->codigo = cod;
        strcpy(novo->loja, loja);
        novo->valor = valor;
        return novo;
    }
    if (valor < raiz->valor)
        raiz->esq = inserir(raiz->esq, loja, cod, valor);
    else
        raiz->dir = inserir(raiz->dir, loja, cod, valor);
    return raiz;
}

void remover_sucessor(p_no raiz) {
    p_no min = raiz->dir; /*será o mínimo da subárvore direita*/
    p_no pai = raiz; /*será o pai de min*/
    while (min->esq != NULL) {
        pai = min;
        min = min->esq;
    }
    if (pai->esq == min)
        pai->esq = min->dir;
    else
        pai->dir = min->dir;
    raiz->codigo = min->codigo;
    raiz->valor = min->valor;
    strcpy(raiz->loja, min->loja);
}

p_no buscar(p_no raiz, int cod, float valor) {
    if (raiz == NULL)
        return raiz;
    if (cod == raiz->codigo && raiz->valor <= valor*1.1)
    //se o no atual é o procurado
        return raiz;
    if (valor < raiz->valor)
        return buscar(raiz->esq, cod, valor);
    else
        return buscar(raiz->dir, cod, valor);
}

p_no proc_repetido (char loja[], float valor, p_no raiz){
    if (raiz == NULL)
        return raiz;
    if (strcmp(raiz->loja, loja) == 0)
        //se a lojá já está na arv repetidos, então ela é repetida
        return raiz;
    if (valor < raiz->valor)
        return proc_repetido(loja, valor, raiz->esq);
    else
        return proc_repetido(loja, valor, raiz->dir);
}

p_no remover_rec(p_no raiz, float valor, char loja[]) {
    if (raiz == NULL)
        return NULL;
    if (valor < raiz->valor)
        raiz->esq = remover_rec(raiz->esq, valor, loja);
    else if (valor > raiz->valor)
        raiz->dir = remover_rec(raiz->dir, valor, loja);
    else if (raiz->esq == NULL && raiz-> dir == NULL && strcmp(loja, raiz->loja) == 0)
        //quando há só um nó na arvore
        return NULL; 
    else if (raiz->esq == NULL && strcmp(loja, raiz->loja) == 0)
        return raiz->dir;
    else if (raiz->dir == NULL && strcmp(loja, raiz->loja) == 0)
        return raiz->esq;
    else
        remover_sucessor(raiz);
    return raiz;
}

p_no buscar_aux(p_no raiz, int cod, float valor, p_no repetidos) {
    // Estamos buscando todos as lojas que tenham o codigo e o valor almejado
    p_no aux = raiz;
    p_no prod = buscar(raiz, cod, valor);
    while(prod != NULL){
        if (proc_repetido(prod ->loja, valor, repetidos) == NULL)
            repetidos = inserir(repetidos, prod->loja, prod->codigo, prod->valor); // as lojas encontradas são inseridas na arv das lojas já printadas
        aux = remover_rec(aux, prod->valor, prod->loja); //as lojas já encontradas vão sendo removidas da raiz
        prod = buscar(aux, cod, valor);
    }
    return repetidos;
}

int comparador(char c1[], char c2[]){
    //vê se c1 vem primeiro do que c2 na ordem alfabetica
    int i = 0;
    while (c1[i] == c2[i])
        i++;
    if (c1[i] < c2[i])
        return 1; // se c1 vem primeiro retornamos 1
    return -1; //se c2 vem primeiro retornamos -1
}

p_no procurar_no(p_no raiz, char loja[]) {
    p_no esq;
    if (raiz == NULL)
        return raiz;
    if (strcmp(raiz->loja, loja) != 0 && comparador(raiz->loja, loja) == 1)
        return raiz;
    esq = procurar_no(raiz->esq, loja);
    if (esq != NULL)
        return esq;
    return procurar_no(raiz->dir, loja);
}

void imprime_ofertas(p_no raiz){
    //imprimir em ordem alfabetica
    p_no no_min;
    char min[15];
    printf("\nOFERTAS para %d:\n", raiz->codigo);
    while (raiz != NULL){
        no_min = raiz;
        strcpy(min, raiz->loja);
        while (procurar_no(raiz, min) != NULL){ //quando a função retorna null é porque o raiz->loja é o primeiro na ordem alfabetica
            no_min = procurar_no(raiz, min);
            strcpy(min, no_min->loja);
        }
        printf("%s %d %.2f\n", no_min->loja, no_min->codigo, no_min->valor);
        raiz = remover_rec(raiz, no_min->valor, no_min->loja);
    }
}

void destruir_arvore(p_no arv){
    if (arv != NULL){
        destruir_arvore(arv->esq);
        destruir_arvore(arv->dir);
        free(arv);
    }
}

int main(){
    int i = 0, n, m, cod;
    char loja[15];
    float val;
    p_no raiz = NULL, repetidos ;

    scanf("%d", &m);
    while (i != m){
        scanf("%s %d %f", loja, &cod, &val);
        raiz = inserir(raiz, loja, cod, val);
        i++;
    }

    i = 0;
    scanf("%d", &n);
    while(i != n){
        repetidos = NULL;
        scanf("%d %f", &cod, &val);
        repetidos = buscar_aux(raiz, cod, val, repetidos);
        if (repetidos == NULL){
            printf("\nOFERTAS para %d:\n", cod);
            printf("nenhuma boa oferta foi encontrada");
        }
        else
            imprime_ofertas(repetidos);
        i++;
    }
    destruir_arvore(raiz);
    destruir_arvore(repetidos);
    return 0;
}