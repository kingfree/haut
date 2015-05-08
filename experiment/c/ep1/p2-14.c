#include <stdio.h>
int main() {
    int n, i;
    float s = 0;
    scanf("%d", &n);
    for(i = 1; i <= 2*n-1; i += 2)
        s += 1.0 / i;
    printf("%.4f\n", s);
    return 0;
}

