#include <stdio.h>
#include <stdlib.h>
void swap(int *a, int *b) {
    int t;
    t = *a, *a = *b, *b = t;
}
int main() {
    int n, a[20], i;
    int max = 0, max_i;
    int min = 0x7ffffff, min_i;
    scanf("%d", &n);
    for(i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        if(a[i] > max)
            max = a[max_i = i];
    }
    swap(&a[max_i], &a[n]);
    for(i = 1; i <= n; i++) {
        if(a[i] < min)
            min = a[min_i = i];
    }
    swap(&a[min_i], &a[1]);
    for(i = 1; i < n; i++)
        printf("%d ", a[i]);
    printf("%d\n", a[n]);
    return 0;
}
