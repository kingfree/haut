#include <stdio.h>

#define inputi(x) ({ \
    printf("请输入 " #x ": "); \
    scanf("%i", &x); \
})

void dectobin(int n)
{
    if (n > 0) {
        dectobin(n / 2);
        printf("%d", n % 2);
    }
}

int main()
{
    int n;
    inputi(n);
    dectobin(n);
    printf("\n");
    return 0;
}
