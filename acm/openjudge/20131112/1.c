#include <stdio.h>
int main() {
    int n, m;
    while(scanf("%d%d", &n, &m) == 2)
        printf("%s\n", m > n / 2 + (n & 1) ? "YES" : "NO");
    return 0;
}