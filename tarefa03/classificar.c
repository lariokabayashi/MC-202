#include <stdio.h>
#include <string.h>
int find_i(char full_name[], int n){
    int i;
    for (i = 0; i < n; i++){
        if (full_name[i] == '_'){
            return i;
        }
    }
    return -1;
}

void sep_first_name(char full_name[], char first_name[], int i){
    int j;  
    for (j = 0; j < i; j++){
        first_name[j] = full_name[j];
    }
    first_name[j] = '\0';
}

void print_str(char name[]) {
    int i = 0;
    while (name[i] != '\0')
    {
        printf("%c", name[i]);
        i ++;
    }
    printf("\n");
}

int find_repeated_names(int list[], int j){
    int i;
    for (i = 0; i < 100; i++){
        if (list[i] == j){
            return 0;
        }
    }
    return -1;
}
void organize_list(char list[100][30], int n){
    int i, j, min, x = 0;
    char aux[30];
    for (i = 0; i < n; i++){
        x = 0;
        min = i;
        for(j = i + 1; j < n; j++){
            x = 0;
            while (list[min][x] == list[j][x]){
                x++;
            } 
            if(list[min][x] > list[j][x]){
                    min = j;
            }
        }
        if (min != i){
                strcpy(aux, list[min]);
                strcpy(list[min], list[i]);
                strcpy(list[i], aux);
        }
    }
}

void find_pairs(int n, char list[100][30]){
    int i, j, index_1, index_2,repeat_names[100], r = 0;
    char name_1[30], name_2[30];
    for(i = 0; i < 100; i++){
        repeat_names[i] = 0;
    }
    for (i = 0; i < n; i++){
        int k = 0;
        index_1 = find_i(list[i], 30);
        name_1[index_1] = 0;
        sep_first_name(list[i], name_1, index_1);
        for (j = i + 1; j < n; j++){
            if (find_repeated_names(repeat_names, j) != 0){
                index_2 = find_i(list[j], 30);
                name_2[index_2] = 0;
                sep_first_name(list[j], name_2, index_2);
                if (strcmp(name_1, name_2) == 0){
                    if (k == 0){
                        print_str(list[i]);
                        print_str(list[j]);
                        repeat_names[r] = j;
                        r += 1;
                        k += 1;
                    }
                    else { 
                        print_str(list[j]);
                        repeat_names[r] = j;
                        r += 1;
                    }
                }
            }
        }
    }
}

int main(){
    int n, i;
    char type[10], list[100][30];
    scanf("%d ", &n);
    scanf("%s ", type);
    for (i = 0; i < n; i++){
        scanf("%s ", list[i]);
    }
    organize_list(list, n);
    find_pairs(n, list);
    return 0;
}