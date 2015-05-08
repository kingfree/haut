#include <stdio.h>
int main() {
    int i;
    int count = 0, sum = 0;
    char c;
    for( ; ; ) {
        c = getchar();
        if('0' <= c && c <= '9') {
            count ++;
            sum += c - '0';
        } else
            break;
    }
    printf("%d %d\n", count, sum);
    return 0;
}

