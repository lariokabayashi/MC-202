#include <stdio.h>
#include <stdlib.h>
#include "minecraft.h"

int main(){
    int m, n, s, i, j, **altitudes, qtdDiamante = 0, qtdOuro = 0, qtdFerro = 0, qtdBlocos = 0;
    Bloco ***mundo;
    double t, t_total;
    scanf("%d %d", &m, &n);
    scanf("%d", &s);
    scanf("%lf", &t);
    altitudes = calcularAltitudes(m, n, s);
    mundo = criarMundo(m, n, altitudes, s);
    t_total = explorarMundo (mundo, m, n, altitudes, t, &qtdDiamante, &qtdOuro, &qtdFerro, &qtdBlocos);
    printf("Total de Blocos: %d\n", qtdBlocos);
    printf("Tempo total: %.2lfs\n", t_total);
    printf("Diamantes: %d\n", qtdDiamante);
    printf("Ouros: %d\n", qtdOuro);
    printf("Ferros: %d\n", qtdFerro);
    for (i = 0; i < m; i++){
        for (j = 0; j < n; j++){
            free(mundo[i][j]);
        }
        free(altitudes[i]);
        free(mundo[i]);
    }
    free(altitudes);
    free(mundo);
    return 0;
}

int **calcularAltitudes(int m, int n, int seed){
    int i, j, **altitude;
    altitude = calloc(m, sizeof(int*));
    for (i = 0; i < m; i++){
        altitude[i] = calloc(n, sizeof(int));
        for (j = 0; j < n; j++){
            altitude[i][j] = ((seed * (202 + i + j) + 12345 + i + j) % 256);
        }
    }
    return altitude;
}

Bloco ***criarMundo(int m, int n, int **altitudes, int seed){
    int i, j, y;
    Bloco ***mundo;
    mundo = calloc(m, sizeof(int**)); //troque para calloc dps
    for (i = 0; i < m; i++){
        mundo[i] = calloc(n, sizeof(int*));
        for (j = 0; j < n; j++){
            mundo[i][j] = calloc(256, sizeof(int));
            for (y = 0; y < 256; y++){
                if (y > altitudes[i][j]){
                    mundo[i][j][y].tipo = 21;
                }
                else{
                    mundo[i][j][y].tipo = ((seed * (202 + i + j + y) + i + j + y) % 33);
                }
            }
        }
    }
    return mundo;
}

double explorarMundo(
    Bloco ***mundo, int m, int n, int **altitudes, double tempoPorBloco,
    int *qtdDiamante, int *qtdOuro, int *qtdFerro, int *qtdBlocos){
    int i, j, y;
    double tempo_total;
    for (i = 0; i < m; i++){
        for (j = 0; j < n; j++){
            for (y = 0; y < 256; y++){
                if (mundo[i][j][y].tipo <= 20){
                    *qtdBlocos += 1;
                }
                if (mundo[i][j][y].tipo == 0){
                    *qtdDiamante += 1;
                }
                else if (mundo[i][j][y].tipo == 1 || mundo[i][j][y].tipo == 2){
                    *qtdOuro += 1;
                }
                else if (mundo[i][j][y].tipo == 3 || mundo[i][j][y].tipo == 4 || mundo[i][j][y].tipo == 5){
                    *qtdFerro += 1;
                }
            }
        }
    }
    tempo_total = *qtdBlocos*tempoPorBloco;
    return tempo_total;
}

