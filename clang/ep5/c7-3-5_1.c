#include <stdio.h>
#include <stdlib.h>
void swap(int *a, int *b) {
    int t;
    t = *a, *a = *b, *b = t;
}
int main() {
    int n, a[100], i, j;
    scanf("%d", &n);
    for(i = 0; i < n; i++)
        printf("%d ", a[i] = rand() % 1000);
    printf("\n");

    for(i = n - 1; i >= 0; i--)
        for(j = 0; j < i; j++)
            if(a[j] > a[j + 1])
                swap(&a[j], &a[j + 1]);

    for(i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}
