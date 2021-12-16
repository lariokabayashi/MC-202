#include <stdio.h>
#include <stdlib.h>

int count (int n, int *id, char c, char *segment){
    int i, count = 0;
    for (i = n; i > -1; i--){
        if (segment[i] == c){
            count++;
        }
    }
    return (count-1);
}
void ID(int n, int *id, char *segment){
    int i;
    for (i = 0; i < n; i++){
        id[i] = count(i, id, segment[i], segment);
    }
}

char transcr(int n, char segment, char dna[] , char rna[]){
    int j = 0;
    for (j = 0; j < 13; j++){
        if (segment == dna[j]){
        return rna[j];
        }
    }
    return rna[j];
}

void print_str(char name[]) {
    int i = 0;
    while (name[i] != '\0'){
        printf("%c  ", name[i]);
        i ++;
    }
}

void print_output(int n, char *segment,char *transcript, int *id){
    int i;
    printf("ID:     ");
    for (i = 0; i < n; i++){
        printf("%d  ", id[i]);
    }
    printf("\n");
    printf("DNA:    ");
    print_str(segment);
    printf("\n        ");
    for (i = 0; i < n; i++){
        printf("|  ");
    }
    printf("\n");
    printf("RNA:    ");
    print_str(transcript);
}

int main(){
    char *segment, dna[14], rna[14], *transcript; 
    int n, i, *id;
    scanf("%s", dna);
    scanf("%s", rna);
    scanf("%d", &n);
    segment = calloc((n+1), sizeof(char));
    id = calloc(n, sizeof(int));
    transcript = calloc((n+1), sizeof(char));
    scanf("%s", segment);
    for (i = 0; i < n; i++){
        transcript[i] = transcr(n, segment[i], dna, rna);
    }
    ID(n, id, segment);
    print_output(n, segment, transcript, id);
    free (segment);
    free (id);
    free (transcript);
    return 0;
}

