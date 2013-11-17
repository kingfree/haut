#include <stdio.h>
#include <string.h>
#define p(x) fprintf(stderr, #x " = %d\n", x)
#define N 10000
char p[N];

int chai(int n) {
    int i, s = 0;
    for(i = 2; i < n / 2; i++)
        if(p[i] && p[n - i])
            s++;
    return s;
}

int main() {
    int i, j, n;

    memset(p, 1, N);
    for(i = 2; i < N; i++) if(p[i])
        for(j = i + i; j < N; j += i)
            p[j] = 0;

    while(scanf("%d", &n), n)
        printf("%d\n", chai(n));
    return 0;
}