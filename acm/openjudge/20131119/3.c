#include <stdio.h>
int Year(int y) {
    return y % 100 == 0 ? y % 400 == 0 : y % 4 == 0;
}
int main() {
    int y;
    while(scanf("%d", &y) == 1)
        printf("%c\n", Year(y) ? 'R' : 'P');
    return 0;
}
