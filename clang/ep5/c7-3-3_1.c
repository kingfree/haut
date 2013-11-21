#include <stdio.h>
int main() {
    int n, i, j, p, a[20];
    scanf("%d", &n);
    for(i = 0; i < n; i++)
        scanf("%d", a + i);
    p = a[j = 0];
    for(i = 1; i < n; i++)
        if(p < a[i])
            p = a[j = i];
    printf("%d %d\n", p, j);
    return 0;
}
