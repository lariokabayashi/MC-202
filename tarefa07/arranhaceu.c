#include <stdio.h>
#include <stdlib.h>

int procura_repetido(int p, int v, int k, int *seq){
    int j;
    if (p < k){ //pos < n
        for(j = 0; j <= p; j++){ //vendo se há igual na linha
            if (seq[j] == v){
                return -1;
            }
        }
    }
    else { //para pos > n
        for (j = 1; j <= p%k; j++){
            if (v == seq[p-j]){
                return -1;
            }
        }
        for (j = 1; j <= (p/k); j++){//vendo se há igual na coluna
            if (seq[p-j*k] == v){
                return -1;
            }
        }
        
    }
    return 0;
}

int predios_vistos_OD(int tipo, int v, int k, int i, int j, int lim, int *seq){
    int w, y, viu = 1, nao_viu = 0;
    for (w = 1; w <= lim; w++){
        nao_viu = 0;
        for (y = 0; y != w; y += 1){
            if (w == lim){ // nesse caso vamos estar comparando se v é maior do que os termos anteriores

                if (tipo == 0){
                    if (v < seq[i*k + y]){// y representa a coluna
                        nao_viu += 1;
                    }
                }
                else {
                    if (v < seq[y*k + j]){// y representa a linha
                        nao_viu += 1;
                    }
                }
            } 
            else { //vendo se os termos anteriores são maiores que seus anteriores (se isso acontece, ele foi visto)

                if (tipo == 0){//tipo 0 é examinar os prédios vistos na linha
                    if (seq[i*k + w] < seq[i*k + y]){ //  w = 1,2,3... w representa a coluna
                        nao_viu += 1;
                    }
                }
                else if (tipo == 1) {//prédios vistos na coluna
                    if (seq[w*k + j] < seq[y*k + j]){ //  w = 1,2,3... w representa a linha
                        nao_viu += 1;
                    }
                }
            }
        }
        if (nao_viu == 0){
            viu += 1;
        }
    }
    return viu;
}

int predios_vistos_OI(int tipo, int v, int k, int i, int j, int p, int lim, int *seq){
    int w, y, viu = 1, nao_viu = 0;
    seq[p] = v; //para não ter que dividir o código em casos
    for (w = 0; w < lim; w++){
        nao_viu = 0;
        for (y = w+1; y <= lim; y += 1){
           
            if (tipo == 0){// y representa a coluna
                if (seq[i*k + w] < seq[i*k + y]){ //  w = 1,2,3... w representa a coluna
                    nao_viu += 1;
                }
            }
            else { // y representa a linha
                if (seq[w*k + j] < seq[y*k + j]){ //  w = 1,2,3... w representa a linha
                    nao_viu += 1;
                }
            }
        }
        if (nao_viu == 0){
            viu += 1;
        }
    }
    return viu;
}

int ver_arranhaceu(int *seq, int **grade, int p, int v, int k){
    int i = p/k, j = p % k, viu_l1, viu_l2, viu_c1, viu_c2;
    int pista_l_1 = grade[i+1][0], pista_l_2 = grade[i+1][k+1], pista_c_1 = grade[0][j+1], pista_c_2 = grade[k+1][j+1];
   
    viu_l1 = predios_vistos_OD(0, v, k, i, j, p%k, seq);
    if (viu_l1 > pista_l_1){ 
        return -1;//ver se prédios vistos já ultrapassa o limite, ai descarta esse v
    }
   
    if ((p+1)%k == 0){//quando termina uma linha é possivel contabilizar os predios vistos na ordem indireta
        viu_l2 = predios_vistos_OI(0, v, k, i, j, p, p%k, seq);
        if (viu_l1 != pista_l_1 || viu_l2 != pista_l_2){
            seq[p] = 0;
            return -1;
        }
    }
    
    viu_c1 = predios_vistos_OD(1, v, k, i, j, p/k, seq);
    if (viu_c1 > pista_c_1 ){ //ver se prédios vistos na coluna já ultrapassa o limite, ai descarta esse v
        return -1;
    }
   
    if (i == k-1){
        //quando estamos na última linha é possivel contabilizar os predios vistos na coluna em OI
        viu_c2 = predios_vistos_OI(1, v, k, i, j, p, p/k, seq);
        if (viu_c1 != pista_c_1 || viu_c2 != pista_c_2){
            return -1;
        }
    }
    return 0;
} 

void imprime_matriz(int *seq, int **M, int n){
    int i, j, p;
    for (p = 0; p < n*n; p++){
        i = p/n;
        j = p%n;
        M[i+1][j+1] = seq[p];
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++){
            printf("%d ", M[i+1][j+1]);
        }
        printf("\n");
    }
}

void sem_repeticaoR(int *seq, int **grade, int n, int k, int i) {
    int v;
    if (i == k*k) {
        imprime_matriz(seq, grade, k);
        return;
    }
    for (v = 1; v <= n; v++) {
        if (procura_repetido(i, v, k, seq) == 0) {
            if (ver_arranhaceu(seq, grade, i, v, k) == 0){
                seq[i] = v;
                sem_repeticaoR(seq, grade, n, k, i+1);
            }
        }
    }
}

void sem_repeticao(int n, int **grade) {
    int tam = n*n;
    int *seq = calloc(tam, sizeof(int));
    sem_repeticaoR(seq, grade, n, n, 0);
    free(seq);
}

int main(){
    int n, i, j, **grade;
    scanf("%d", &n);
    grade = calloc(n+2, sizeof(int*));

    for (i = 0; i < n+2; i++){
        grade[i] = calloc(n+2, sizeof(int));
        for (j = 0; j < n+2; j++){
            scanf("%d", &grade[i][j]);
        }
    }

    sem_repeticao(n, grade);
    
    for (i = 0; i < n+2; i++){
        free(grade[i]);
    }
    free(grade);
    return 0;
}
