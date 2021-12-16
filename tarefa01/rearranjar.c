#include <stdio.h>

int find_index (int a, int line[], int n) {
    int i;
    for (i = 0; i <= n; i ++) {
        if (line[i] < a + 1 && line[i] > a - 1) 
            return i; // return index
    }
    return -1;
}

void invert (int a, int b, int line[], int n) {
    int i, elem, index_a, index_b, k;
    index_a = find_index(a, line, n);
    index_b = find_index(b, line, n);
    k = index_b;
     //going from the start to half of the gap, to invert just one time
    for (i = index_a; i <= (index_b + index_a)/2; i++){
        elem = line[i];
        line[i] = line[k];// putting the last element on the beggining
        line[k] = elem;//putting the begginer element on the end
        k--;
    }
}

void put_after (int a, int b, int line[], int n) {
    int i, index_a, index_b;
    index_a = find_index(a, line, n);
    index_b = find_index(b, line, n);
    for (i = index_a + 1; i <= index_b; i++) {
        line[i-1] = line[i]; //moving forward the elements before b
    }
    line[index_b] = a; //the person who waited for b, now is on the position of b
}

int main () {
    int n, m, i, changes, x1, y1, a;
    int line[10000]; //n max = 10000
    scanf ("%d", &n);
    scanf ("%d", &m);
    for (i = 1; i <= n; i++){ //creating the original line
        line[a] = i;
        a++;
    }
    for (i = 0; i < m; i++) { //applying the changes on the order of the line
        scanf ("%d %d %d", &changes, &x1, &y1);
        if (changes < 2) { //if changes = 1
            invert(x1, y1, line, n);
        }
        else {
            put_after(x1, y1, line, n);
        }  
    }
    for (i = 0; i < n; i++){ //printing each element of the array
        printf("%d ", line[i]); 
    }
    return 0;
}