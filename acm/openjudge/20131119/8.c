#include <stdio.h>
int sqr(int x) {
    return x * x;
}
int cub(int x) {
    return x * x * x;
}
void swap(int *a, int *b) {
    int t;
    t = *a, *a = *b, *b = t;
}
int main() {
    int x, y, i, m, n;
    while(scanf("%d%d", &m, &n) == 2) {
        x = y = 0;
        if(m > n)
            swap(&m, &n);
        for(i = m; i <= n; i++) {
            if(i % 2 == 0)
                x += sqr(i);
            else
                y += cub(i);
        }
        printf("%d %d\n", x, y);
    }
    return 0;
}
