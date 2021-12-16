#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 4783

typedef struct No{
    char chave[50]; // Palavrea
    int freq; // Frequência da palavra
    struct No * prox;
} No;
typedef No * p_no;

typedef struct {
    p_no vetor[MAX];
} Hash;
typedef Hash * p_hash;

p_hash criar_hash(){
    int i;
    p_hash t;
    t = malloc(sizeof(Hash));
    for (i = 0; i < MAX; i++){
        t->vetor[i] = NULL;
    }
    return t;
}

void ajusta_caracteres(char c[]){
    // Deixa só os caracteres válidos na str, de acordo com os critérios da tarefa
    int i = 0, j = 0;
    char nova[50];
    for (i = 0; i < strlen(c); i++){
        if ((c[i] >= 'a' && c[i] <= 'z') || (c[i] >= 'A' && c[i] <= 'Z')) {
            nova[j] = tolower(c[i]);
            j++;
        }
        else if (isalnum(c[i]))
            nova[0] = '\0';
    }
    nova[j] = '\0';
    strcpy(c, nova);
}

int hash(char chave[]) {
    // Função Hash
    int i, n = 0;
    for (i = 0; i < strlen(chave); i++)
        n = (256 * n + chave[i]) % MAX;
    return n;
}

int Eh_stop_word(char c[], char stop_word[600][50], int m){
    // Verifica se char c eh Stop Word
    int i = 0;
    while (i != m){
        if (strcmp(c, stop_word[i]) == 0)
            return 1;
        i++;
    }
    return 0;
}

p_no inserir_lista(p_no lista, char x[]) {
    p_no novo;
    novo = malloc(sizeof(No));
    strcpy(novo->chave, x);
    novo->freq = 1;
    novo->prox = lista;
    return novo;
}

int buscar(char chave[], p_hash t){
    // Busca se palavra (chave) já existe na p_hash, e se sim aumentamos a freuqẽncia da palavra
    int i, var = 0;
    for (i = 0; i < MAX; i++){
        if (t->vetor[i] != NULL && strcmp(t->vetor[i]->chave, chave) == 0){
            t->vetor[i]->freq++;
            return 1;
        }
    }
    return var;
}

void insere(p_hash t, char chave[]) {
    int n = hash(chave), var;
    var = buscar(chave, t);
    // Quando var == 0, quer dizer que essa palavra(chave) ainda não foi adicionada na p_hash
    if (var == 0 && t->vetor[n] != NULL)
        // Procuramos o lugar que vamos add a palavra nova
        while (t->vetor[n] != NULL)
            n++;
    if (var == 0)
        //  Inserimos palavra nova
        t->vetor[n] = inserir_lista(t->vetor[n], chave);
}

int elimina_str_descartadas(p_hash t, p_hash nova_t, char stop_word[600][50], int m){
    // Elimina do hash listas que são Null
    int i, k = 0;
    for (i = 0; i < MAX; i++)
        if (t->vetor[i] != NULL)
            nova_t->vetor[k++] = t->vetor[i];
    return k;
}

void troca(p_no *a, p_no *b) {
    p_no t = *a;
    *a = *b;
    *b = t;
}

#define F_ESQ(i) (2*i+1) /*Filho esquerdo de i*/
#define F_DIR(i) (2*i+2) /*Filho direito de i*/

void desce_no_heap(p_hash t, int n, int k) {
    int maior_filho;
    if (F_ESQ(k) < n) {
        maior_filho = F_ESQ(k);
        if (F_DIR(k) < n)
            if (t->vetor[F_ESQ(k)]->freq < t->vetor[F_DIR(k)]->freq || (t->vetor[F_ESQ(k)]->freq == t->vetor[F_DIR(k)]->freq &&
            strcmp(t->vetor[F_ESQ(k)]->chave, t->vetor[F_DIR(k)]->chave) > 0))
                maior_filho = F_DIR(k);
        if (t->vetor[k]->freq < t->vetor[maior_filho]->freq) {
            troca(&t->vetor[k], &t->vetor[maior_filho]);
            desce_no_heap(t, n, maior_filho);
        }
        else if (t->vetor[k]->freq == t->vetor[maior_filho]->freq){
            if (strcmp(t->vetor[k]->chave, t->vetor[maior_filho]->chave) > 0){
                troca(&t->vetor[k], &t->vetor[maior_filho]);
                desce_no_heap(t, n, maior_filho);
            }
        }
    }
}

void heapsort(p_hash t, int n) {
    // Ordena p_hash por frequência
    int k;
    for (k = n/2; k >= 0; k--) /* transforma em heap */
        desce_no_heap(t, n, k);
    while (n > 1) { /* extrai o máximo */
        troca(&t->vetor[0], &t->vetor[n - 1]);
        n--;
        desce_no_heap(t, n, 0);
    }
}

void imprime_palavras(p_hash t, int n){
    // Imprime 50 palavras mais frequentes
    int i = 1;
    while (i <= 50){
        if (t->vetor[n-i] != NULL)
        printf("%s %d\n", t->vetor[n-i]->chave, t->vetor[n-i]->freq);
        i++;
    }
}

void destruir_lista(p_no lista) {
    if (lista != NULL) {
        destruir_lista(lista->prox);
        free(lista);
    }
}

void destruir_hash(p_hash t){
    int i;
    for (i = 0; i < MAX; i++)
        destruir_lista(t->vetor[i]);
    free(t);
}

int main(){
    int n, m, i = 0;
    char stop_word[600][50], word[50];

    p_hash nuvem, nuvem_50;
    nuvem = criar_hash();
    nuvem_50 = criar_hash();

    scanf("%d", &n);
    scanf("%d ", &m);

    while (i != m){
        scanf("%s ", stop_word[i]);
        i++;
    }
    while (scanf("%s", word) != EOF){
        ajusta_caracteres(word);
        if (strlen(word) > 1 && !Eh_stop_word(word, stop_word, m))
            insere(nuvem, word);
    }
    // Elimina do hash p_no que são NULL
    int tam = elimina_str_descartadas(nuvem, nuvem_50, stop_word, m);
    // Ordena por frequência
    heapsort(nuvem_50, tam);
    //Imprime as 50 palavras mais frequentes
    imprime_palavras(nuvem_50, tam);
    destruir_hash(nuvem);
    free(nuvem_50);
    return 0;
}
