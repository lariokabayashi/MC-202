#include <stdio.h>
# define MAX 1000

void read_array(double M[MAX][15], int n) {
    int i, j;
    for (i = 0; i < n; i++){
        for (j = 0; j < 12; j++){
            scanf("%lf", &M[i][j]);
        }
    }
}

double calc_percent(double value, double v1, double v2, double v3, double v4){
    // calc percentage based on the taxes v1, v2, v3, v4
    double percent;
    if (value < v1){
        percent = 0;
    }
    else if (value >= v1 && value < v2){
        percent = ((value - v1)*7.5)/100;
    }
    else if (value >= v2 && value < v3){
        percent = ((value - v2) * 15)/100 + ((v2 - v1)*7.5)/100;
    }
    else if (value >= v3 && value < v4){
        percent = ((value - v3) * 22.5)/100 + ((v2 - v1)*7.5)/100 + ((v3 - v2)*15)/100; 
    }
    else{
        percent = ((value - v4) * 27.5)/100 + ((v2 - v1)*7.5)/100 + ((v3 - v2)*15)/100 + ((v4 - v3)*22.5)/100; 
    }
    return percent;
}

double sum_array(double M[MAX][15], int i){ //sum array of the line i 
    int j;
    double sum = 0;
    for (j = 0; j < 12; j++){
        sum += M[i][j];
    }
    return sum;
}

double calc_anual_tax(double M[MAX][15], int n, double abat){
    double basis_of_calc, sum;
    sum = sum_array(M,n);
    basis_of_calc = (sum - abat); 
    //return anual tax
    return calc_percent(basis_of_calc, 18000, 26400, 36000, 44400);
}
void print_array(double M[MAX][15], int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 15; j++){
            printf("%.2lf\t", M[i][j]);
        }
        printf("\n");
    }
}

int main(){
    int n, i, j;
    // column = 12 months + 3 parameters (Retido Devido Ajuste)
    double salary [MAX][15], abat;
    scanf("%d", &n);
    read_array(salary, n); //transform input in array
    for (i = 0; i < n; i++){
        scanf ("%lf", &abat); // calc anual tax first because the array will change 
        salary[i][13] = (calc_anual_tax(salary, i, abat));
    }
    for (i = 0; i < n; i++){
            for (j = 0; j < 12; j++){
                // now the array will contain retained values
                salary[i][j] = calc_percent(salary[i][j], 1499.15, 2246.75, 2995.7, 3743.19);
            }
        }
        for (i = 0; i < n; i++){
            salary[i][12] = sum_array(salary, i); // total retained
            // anual adjust = anual tax - total retained
            salary[i][14] = salary[i][13] - salary[i][12];         }
    printf("Jan Fev Mar Abr Mai Jun Jul Ago Set Out Nov Dez Retido Devido Ajuste\n");
    print_array(salary, n);
    return 0;
}


