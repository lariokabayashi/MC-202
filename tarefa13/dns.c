#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Cor {VERMELHO , PRETO};

typedef struct No {
    char codigo[21]; //end_site ou IP
    char dominio[101]; 
    int n_vezes;
    enum Cor cor;
    struct No *esq, *dir;
} No;
typedef No * p_no;

int ehVermelho(p_no x) {
    if (x == NULL)
        return 0;
    return x->cor == VERMELHO;
}
int ehPreto(p_no x) {
    if (x == NULL)
        return 1;
    return x->cor == PRETO;
}

p_no rotaciona_para_esquerda(p_no raiz) {
    p_no x = raiz->dir;
    raiz->dir = x->esq;
    x->esq = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return x;
}

p_no rotaciona_para_direita(p_no raiz) {
    p_no x = raiz->esq;
    raiz->esq = x->dir;
    x->dir = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return x;
}

void sobe_vermelho(p_no raiz) {
    raiz->cor = VERMELHO;
    raiz->esq->cor = PRETO;
    raiz->dir->cor = PRETO;
}
p_no corrige_arv(p_no raiz){
    if (ehVermelho(raiz->dir) && ehPreto(raiz->esq))
        raiz = rotaciona_para_esquerda(raiz);
    if (ehVermelho(raiz->esq) && ehVermelho(raiz->esq->esq))
        raiz = rotaciona_para_direita(raiz);
    if (ehVermelho(raiz->esq) && ehVermelho(raiz->dir))
        sobe_vermelho(raiz);
    return raiz;
}

p_no inserir_rec_site(p_no raiz, char codigo[], char dominio[]) {
    p_no novo;
    if (raiz == NULL) {
        novo = malloc(sizeof(No));
        novo->esq = novo->dir = NULL;
        strcpy(novo->codigo, codigo);
        strcpy(novo->dominio, dominio);
        novo->cor = VERMELHO;
        return novo;
    }
    if (strcmp(dominio, raiz->dominio) < 0) //criterio de comparação é o dominio
        raiz->esq = inserir_rec_site(raiz->esq, codigo, dominio);
    else
        raiz->dir = inserir_rec_site(raiz->dir, codigo, dominio);
   
    raiz = corrige_arv(raiz);
   
    return raiz;
}

p_no inserir_rec_usuario(p_no raiz, char codigo[], char dominio[]) {
    p_no novo;
    if (raiz == NULL) {
        novo = malloc(sizeof(No));
        novo->esq = novo->dir = NULL;
        strcpy(novo->codigo, codigo);
        novo->n_vezes = 1;
        novo->cor = VERMELHO;
        return novo;
    }
    if (strcmp(codigo, raiz->codigo) < 0) //criterio de comparação é o IP
        raiz->esq = inserir_rec_usuario(raiz->esq, codigo, dominio);
    else
        raiz->dir = inserir_rec_usuario(raiz->dir, codigo, dominio);
    
    raiz = corrige_arv(raiz);

    return raiz;
}

p_no inserir(p_no raiz, char codigo[], char dominio[], int tipo) {
    if (tipo == 1) //arv de sites
        raiz = inserir_rec_site(raiz, codigo, dominio);
    else //tipo = 2: arv de usuarios
        raiz = inserir_rec_usuario(raiz, codigo, dominio);
    raiz->cor = PRETO;
    return raiz;
}

p_no buscar_site(p_no raiz, char dominio[]) {
    while (raiz != NULL && strcmp(dominio, raiz->dominio) != 0)
        if (strcmp(dominio, raiz->dominio) < 0) //na arv de sites criterio é o dominio
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
    return raiz;
}

p_no buscar_usuario(p_no raiz, char codigo[]) {
    while (raiz != NULL && strcmp(codigo, raiz->codigo) != 0)
        if (strcmp(codigo, raiz->codigo) < 0) // na arv de usuarios criterio é o IP
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
    return raiz;
}

void destruir_arvore(p_no arv){
    if (arv != NULL){
        destruir_arvore(arv->esq);
        destruir_arvore(arv->dir);
        free(arv);
    }
}

int main(){
    int u, n, m, i = 0;
    char dominio[101], GET[4], FROM[5], end_site[20], IP[20];
    p_no sites = NULL, site_buscado, arv_usuarios = NULL, usuario;
    scanf("%d", &u);
    scanf("%d", &n);
    while (i != n){
        scanf("%s %s", dominio, end_site);
        sites = inserir(sites, end_site, dominio, 1);
        i++;
    }

    scanf("%d", &m);
    i = 0;
    while (i != m){
        scanf("%s %s %s %s", GET, dominio, FROM, IP);
        site_buscado = buscar_site(sites, dominio);
        usuario = buscar_usuario(arv_usuarios, IP);
        
        if (usuario == NULL){
            arv_usuarios = inserir(arv_usuarios, IP, dominio, 2);
            usuario = buscar_usuario(arv_usuarios, IP);
        }
        else
            usuario->n_vezes += 1;

        //saída
        if (usuario->n_vezes > u)
            printf("FORBIDDEN %s FROM %s\n", dominio, IP);
        else if (site_buscado == NULL)
            printf("NOTFOUND %s FROM %s\n", dominio, IP);
        else
            printf("ACCEPTED %s FROM %s RESPOND %s\n", dominio, IP, site_buscado->codigo);
        i++;
    }
    destruir_arvore(sites);
    destruir_arvore(arv_usuarios);
    return 0;
}
