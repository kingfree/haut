#include <stdio.h>

int sum(int array[], int size) {
    int k = 0;
    int s = 0;
    /* M(0) and k == 0 */
    while(k < size) {
        /* M(k) and k < size */
        s += array[k];
        /* M(k+1) and k < size */
        k += 1;
        /* M(k) and k <= size */
    }
    /* M(size) and k == size */
    return s;
}

int main() {
    int n, a[1<<10], i;
    scanf("%d", &n);
    for(i = 0; i < n; i++) {
        a[i] = i + 1;
    }
    printf("%d\n", sum(a, n));
    return 0;
}