#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    int dado;
    struct No *prox;
} No;
typedef struct No * p_no;

typedef struct Pessoa{
    int num;
    char profissao[26];
    char vacina[13];
}Pessoa;

p_no cria_lista(){
    return NULL;
}

p_no inverter_lista(p_no lista) {
    p_no atual, ant, invertida = NULL;
    atual = lista;
    while (atual != NULL) {
        ant = atual;
        atual = ant->prox;
        ant->prox = invertida;
        invertida = ant;
    }
    return invertida;
}

p_no concatenar_lista(p_no primeira , p_no segunda) {
    if (primeira == NULL)
        return segunda;
    primeira ->prox = concatenar_lista(primeira ->prox, segunda);
    return primeira;
}

p_no adicionar_elemento(p_no lista, int x) {
    p_no novo;
    novo = malloc(sizeof(No));
    novo->dado = x;
    novo->prox = lista;
    return novo;
}

p_no adiciona_no_inicio(p_no lista, int x){
    p_no novo;
    novo = malloc(sizeof(No));
    novo -> dado = x;
    novo = concatenar_lista(lista, novo);
    return novo;
}

p_no add_depois_lanterna(p_no lista, int x, int elem) {
    p_no nova;
    nova = malloc(sizeof(No));
    while(lista != NULL && lista->dado != x){
        //copia lista até ela chegar na pessoa com a lanterna
        nova = adicionar_elemento(nova, lista->dado);
        lista = lista->prox;
    }
    nova = adicionar_elemento(nova, elem);
    nova = inverter_lista(nova);
    return concatenar_lista(nova, lista);//junta lista nova com o restante da fila
}

int encontrar_cliente( Pessoa clientes[], int n){
    int k;
    for (k = 0; clientes[k].num != 0; k++){
        if (clientes[k].num == n)
            return k;
    }
    return 0;
}

Pessoa *ordenar_clientes(p_no lista, Pessoa lista_clientes[], int i, Pessoa *clientes_ord){
    // coloca a lista de clientes na ordem da lista ligada
    int n, w = 0;
    while (lista != NULL){
        if (lista->dado != 0){
            n = encontrar_cliente(lista_clientes, lista->dado);
            clientes_ord[w] = lista_clientes[n];
            w++;
        }
        lista = lista->prox;
    }
    return clientes_ord;
}

p_no remover_num(p_no lista, int x){
    p_no nova;
    nova = malloc(sizeof(No));
    lista = inverter_lista(lista);
    while (lista != NULL && lista->dado != x){
        // copia na lista nova os elementos da lista, até chegar no que deve ser removido
        if (lista->dado != 0)
            nova = adicionar_elemento(nova, lista->dado);

        lista = lista->prox;
    }
    lista = inverter_lista(lista->prox);
    return concatenar_lista(lista, nova);//depois junta a lista nova com o restante da lista original
}

Pessoa *remove_de_array(int k, Pessoa *clientes, int i, int x){
    int w;
    for (w = k; w < i; w++)
        clientes[w] = clientes[w+1];

    clientes[i - x].num = 0; //para indicar que esse vetor está vazio agora
    return clientes;
}

p_no atras_do_fiscal(p_no lista, Pessoa *clientes_ord, int i){
    int k, x = 0;
    for (k = 0; k < i; k++){
        if (strcmp(clientes_ord[k].profissao, "fiscal") == 0){
            while (strcmp(clientes_ord[k + 1].vacina, "nao-vacinado") == 0){
                //enquanto os que estao atras do fiscal não estão vacinados
                //removemos o numero da lista e da lista de clientes
                lista = remover_num(lista, clientes_ord[k + 1].num);
                clientes_ord = remove_de_array(k + 1, clientes_ord, i, x);
                x += 1;
            }
        }
    }
    return lista;
}

void imprime (p_no lista, Pessoa clientes[]) {
    p_no atual;
    int i;
    for (atual = lista; atual != NULL; atual = atual->prox){
        if (atual->dado != 0){
            for (i = 0; i < 100; i++){
                if (clientes[i].num == atual->dado){
                    printf ("%d %s %s\n", atual->dado, clientes[i].profissao, clientes[i].vacina);
                    break;
                }
            }
        }
    }
}

void destruir_lista(p_no lista) {
    if (lista != NULL) {
        destruir_lista(lista->prox);
        free(lista);
    }
}

int verifica_privilegio(char prof[]){
    if (strcmp(prof, "coach") == 0 || strcmp(prof, "ex-bbb") == 0
    || strcmp(prof, "filho-politico") == 0 
    || strcmp(prof, "herdeiro") == 0 || strcmp(prof, "youtuber-financas") == 0)
        return 1;
    return -1;
}

int main() {
    int n, lanterna = 0, i = 0;
    Pessoa lista_clientes[100], *clientes_ord;
    p_no lista;
    lista = cria_lista();
    scanf("%d", &n);

    while (scanf("%d", &n) != EOF){
        Pessoa cliente;
        cliente.num = n;
        if (n == 10431)
            printf("x");
        scanf("%s", cliente.profissao);
        scanf("%s", cliente.vacina);

        if (strcmp(lista_clientes[i-1].profissao, "fiscal") == 0 && strcmp(cliente.vacina, "nao-vacinado") == 0 && verifica_privilegio(cliente.profissao) == -1)
            continue; //não faça nada

        else{
            lista_clientes[i] = cliente;
            if (lanterna == 0 || strcmp(cliente.profissao, "funcionario") == 0)
                lista = adicionar_elemento(lista, n); //add no final da lista
            
            else if (verifica_privilegio(cliente.profissao) == 1)
                lista = adiciona_no_inicio(lista, n);
            
            else
                lista = add_depois_lanterna(lista, lanterna, n);
            lanterna = n;
            i++;
        }
        scanf("%d", &n);
    }

    clientes_ord = calloc(i, sizeof(Pessoa));
    lista = inverter_lista(lista); 
    clientes_ord = ordenar_clientes(lista, lista_clientes, i, clientes_ord);
    lista = atras_do_fiscal(lista, clientes_ord, i);
    imprime(lista, lista_clientes); /*(em ordem reversa de inserção)*/
    destruir_lista(lista);
    free(clientes_ord);
    return 0;
}
//teste 10 falhando, teste 10 avalia se os criterios do fiscal tao sendo atendidos, 9.0