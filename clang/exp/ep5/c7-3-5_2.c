#include <stdio.h>
#include <stdlib.h>
void swap(int *a, int *b) {
    int t;
    t = *a, *a = *b, *b = t;
}
int main() {
    int n, a[100], i, j, p;
    scanf("%d", &n);
    for(i = 0; i < n; i++)
        printf("%d ", a[i] = rand() % 1000);
    printf("\n");

    for(i = 0; i < n - 1; i++) {
        p = i;
        for(j = i + 1; j < n; j++)
            if(a[p] > a[j])
                p = j;
        if(p != i)
            swap(&a[i], &a[p]);
    }

    for(i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}
