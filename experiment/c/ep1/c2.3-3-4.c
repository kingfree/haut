#include <stdio.h>
int main() {
    int i, j = 1, n;
    double s = 1;
    printf("n (n > 0): ");
    scanf("%d", &n);
    for(i=2; i<=n; i++) {
        j += 3;
        if(i % 2 == 0)
            s -= 1.0 / j;
        else
            s += 1.0 / j;
    }
    printf("Sum = %.3lf\n", s);
    return 0;
}
