#include <stdio.h>
int countdigit(int number, int digit) {
    int s = 0;
    while(number> 0) {
        s += number % 10 == digit;
        number /= 10;
    }
    return s;
}
int main() {
    int n;
    scanf("%d", &n);
    printf("%d\n", countdigit(n, 2));
    return 0;
}
