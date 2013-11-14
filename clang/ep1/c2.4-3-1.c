#include <stdio.h>
#include <math.h>
int main() {
    int n, i;
    printf("n (n > 0): ");
    scanf("%d", &n);
    for(i=0; i<=n; i++)
        printf("3^%d = %.0lf\n", i, pow(3, i));
    return 0;
}
