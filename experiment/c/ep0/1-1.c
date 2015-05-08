#include <stdio.h>
int main(void) {
    int n;
    int factoral(int n);
    scanf("%d", &n);
    printf("%d\n", factoral(n));
    return 0;
}
int factoral(int n) {
    int i, fact = 1;
    for(i = 1; i <= n; i++)
        fact = fact * i;
    return fact;
}
