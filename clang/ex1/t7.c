#include <stdio.h>

#define inputi(x) ({ \
    printf("请输入 " #x ": "); \
    scanf("%i", &x); \
})

int f(int n)
{
    if (n > 1) {
        return f(n - 2) + f(n - 1);
    } else {
        return n;
    }
}

int main()
{
    int n;
    inputi(n);
    printf("fib(%d) = %d\n", n, f(n));
    return 0;
}
