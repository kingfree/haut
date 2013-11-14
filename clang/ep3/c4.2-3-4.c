#include <stdio.h>
int main() {
    int n, m, i;
    int a, b, c;
    scanf("%d %d", &m, &n);
    for(i = m; i <= n; i++) {
        a = i % 10;
        b = i / 10 % 10;
        c = i / 100;
        if(a*a*a + b*b*b + c*c*c == i)
            printf("%d\n", i);
    }
    return 0;
}
