#include <stdio.h>
int f[50] = {0, 1, 1, 2};
int main() {
    int m, i;
    for(i = 3; i < 50; i++)
        f[i] = f[i - 1] + f[i - 2];
    while(scanf("%d", &m) == 1)
        printf("%d\n", f[m]);
    return 0;
}
