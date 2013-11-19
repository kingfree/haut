#include <stdio.h>
int main() {
    int a, b, c;
    scanf("%d%d%d", &a, &b, &c);
    if(a == b && b == c)
        printf("yes\n");
    else if(a + b > c && b + c > a && a + c > b)
        printf("no\n");
    else
        printf("not a triangle");
    return 0;
}
