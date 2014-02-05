#include <stdio.h>

void hanoi(int n, char x, char y, char z) {
    if(n > 0) {
        hanoi(n - 1, x, z, y);
        printf("%c->%c, ", x, y);
        hanoi(n - 1, z, y, x);
    }
}

int main() {
    int n;
    scanf("%d", &n);
    hanoi(n, 'A', 'B', 'C');
    return 0;
}
