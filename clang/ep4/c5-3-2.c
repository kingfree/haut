#include <stdio.h>
int even(int n) {
    return n % 2 ? 1 : 0;
}
int main() {
    int s = 0, n = 1;
    while(n > 0) {
        scanf("%d", &n);
        if(even(n))
            s += n;
    }
    printf("%d\n", s);
    return 0;
}
