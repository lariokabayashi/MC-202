#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct No {
    float reais;
    char nome[21];
    int nivel;
    struct No *esq, *dir; /* *pai */
} No;
typedef No * p_no;

typedef struct No_f {
    char dado[21];
    struct No_f *prox;
} No_f;
typedef struct No_f * p_no_f;

p_no inserir(p_no raiz, float chave, char nome[], char recrutador[], int true) {

    // Insere elemento na raiz, adicionando o Nó como filho do respectivo recrutador

    p_no novo;
    // Quando true == 0 é porque estamos adicionando o criador do esquema
    // então temos que criar a p_no raiz
    if (raiz == NULL && true == 0) {
        novo = malloc(sizeof(No));
        novo->esq = novo->dir = NULL;
        novo->reais = chave;
        strcpy(novo->nome, nome);
        return novo;
    }
    if (raiz == NULL && true == -1)
        return raiz;

    // Percorrendo a raiz e procurando o recrutador pra add recrutado
    if (strcmp(raiz->nome, recrutador) == 0){
        if (raiz->esq == NULL)
            raiz->esq = inserir(raiz->esq, chave, nome, recrutador, 0);
        else if (raiz->dir == NULL)
            raiz->dir = inserir(raiz->dir, chave, nome, recrutador, 0);
    }
    // Percorrendo a raiz recursivamente
    else{
        raiz->esq = inserir(raiz->esq, chave, nome, recrutador, -1);
        raiz->dir = inserir(raiz->dir, chave, nome, recrutador, -1);
    }
    return raiz;
}

p_no procurar_no(p_no raiz, char x[]) {

    // Encontra e retorna Nó de nó->nome == x

    p_no esq;
    if (raiz == NULL || strcmp(x, raiz->nome) == 0)
        return raiz;
    esq = procurar_no(raiz->esq, x);
    if (esq != NULL)
        return esq;
    return procurar_no(raiz->dir, x);
}

p_no acha_recrutador(p_no raiz, char x[], p_no antes) {

    // Encontra o nó de recrutador de um recrutado x

    p_no esq;
    if (raiz == NULL) 
        return raiz;
    if (strcmp(x, raiz->nome) == 0)
        return antes;
    esq = acha_recrutador(raiz->esq, x, raiz);
    if (esq != NULL)
        return esq;
    return acha_recrutador(raiz->dir, x, raiz);
}

void pagamento(char recrutador[], char recrutado[], p_no raiz, float taxa, int i){
    
    // Realiza o pagamento de um filho para um pai

    p_no no_recrutador = procurar_no(raiz, recrutador);
    p_no no_recrutado;

    if (i == 0){
        // Quando i = 0 tanto o filho dir quanto o filho esq devem pagar o pai
        no_recrutador->reais += taxa*no_recrutador->dir->reais + taxa*no_recrutador->esq->reais;
        no_recrutador->esq->reais -= taxa*no_recrutador->esq->reais;
        no_recrutador->dir->reais -= taxa*no_recrutador->dir->reais;
    }
    else{
        no_recrutado = procurar_no(raiz, recrutado);
        no_recrutador-> reais += taxa*no_recrutado->reais;
        no_recrutado->reais -= taxa*no_recrutado->reais;
    }
}

p_no pagam_aux(char recrutador[], p_no raiz){

    // Faz todos os pagamentos necessários percorrendo a àrvore de baixo pra cima

    int i = 0;
    float taxa = 0.06;
    char recrutado[21] = {'0'};

    while(strcmp(recrutado, raiz->nome) != 0 && taxa >= 0.009){
        if (i == 0)
            pagamento(recrutador, recrutado, raiz, taxa, i);
        else{
            //novo recrutador é quem recrutou o atual recrutado
            strcpy(recrutador, acha_recrutador(raiz, recrutado, NULL)->nome);
            pagamento(recrutador, recrutado, raiz, taxa, i);
        }
        i++;
        taxa -= 0.01;
        //recrutador vira recrutado
        strcpy(recrutado, recrutador);
    }
    return raiz;
}

// As funções de baixo são funções padrão para manipular a ED fila
// A fila é necessária para a implementação do percurso em largura


typedef struct {
    p_no_f ini, fim;
} Fila;
typedef Fila * p_fila;

p_fila criar_fila() {
    p_fila f;
    f = malloc(sizeof(Fila));
    f->ini = NULL;
    f->fim = NULL;
    return f;
}

void enfileirar(p_fila f, p_no raiz) {
    p_no_f novo;
    novo = malloc(sizeof(No));
    strcpy(novo->dado, raiz->nome);
    novo->prox = NULL;
    if (f->ini == NULL)
        f->ini = novo;
    else
        f->fim->prox = novo;
    f->fim = novo;
}

void desenfileirar(p_fila f) {
    p_no_f primeiro = f->ini;
    f->ini = f->ini->prox;
    if (f->ini == NULL)
        f->fim = NULL;
    free(primeiro);
}

void destruir_lista(p_no_f lista) {
    if (lista != NULL) {
        destruir_lista(lista->prox);
        free(lista);
    }
}

void destruir_fila(p_fila f) {
    destruir_lista(f->ini);
    free(f);
}

void percurso_em_largura(p_no raiz) {

    // Para imprimir a saída deve implementar um percurso por largura
    // Assim, imprimimos os elementos de cada Nível da pirâmide

    p_fila f;
    p_no no_atual;
    int nivel_ant = -1, i = 0;
    f = criar_fila();
    enfileirar(f, raiz);
    while(f->ini != NULL) {
        if (raiz != NULL) {
            no_atual = procurar_no(raiz, f->ini->dado);

            if (nivel_ant != no_atual->nivel)
                printf("\nNivel %d: [%s %.2f]", no_atual->nivel, no_atual->nome, no_atual->reais*1.000001); 
            else
                printf(" [%s %.2f]", no_atual->nome, no_atual->reais); 

            nivel_ant = no_atual->nivel;
            desenfileirar(f);

            if (no_atual->esq != NULL)
                enfileirar(f, no_atual->esq);
            if (no_atual->dir != NULL)
                enfileirar(f, no_atual->dir);
        }
        i++;
    }
    destruir_fila(f);
    printf("\n");
}

void destruir_arvore(p_no arv){
    if (arv != NULL){
        destruir_arvore(arv->esq);
        destruir_arvore(arv->dir);
        free(arv);
    }
}

int main(){
    int i = 0;
    float invest_ini;
    char topo[5], recruta[8], nome[21] , recrutador[21], recrutado[21];
    p_no raiz;
    
    while (scanf("%f", &invest_ini) != EOF){
        if (i == 0){
            scanf("%s %s %f", topo, nome, &invest_ini);
            raiz = inserir(NULL, invest_ini, nome, recrutador, 0);
            procurar_no(raiz, nome)->nivel = 1;
        }
        else{
            scanf("%s %s %s %f", recrutador, recruta, recrutado, &invest_ini);
            procurar_no(raiz, recrutador)->reais -= invest_ini*0.1;
            invest_ini += invest_ini*0.1;
            raiz = inserir(raiz, invest_ini, recrutado, recrutador, -1);
            procurar_no(raiz, recrutado)->nivel = (procurar_no(raiz, recrutador)->nivel + 1);
            
            //quando recrutador consegue recrutar duas pessoas ele é pagado
            if (procurar_no(raiz, recrutador)->dir != NULL && procurar_no(raiz, recrutador)->esq != NULL)
                raiz = pagam_aux(recrutador, raiz);
        }
        percurso_em_largura(raiz);
        i++;
    }
    destruir_arvore(raiz);
    return 0;
}

