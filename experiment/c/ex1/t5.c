#include <stdio.h>

#define inputi(x) ({ \
    printf("请输入 " #x ": "); \
    scanf("%i", &x); \
})

int Ack(int m, int n)
{
    if (m == 0) {
        return n + 1;
    } else if (n == 0) {
        return Ack(m - 1, 1);
    } else if (m > 0 && n > 0) {
        return Ack(m - 1, Ack(m, n - 1));
    } else {
        return 1;
    }
}

int main()
{
    int m, n;
    inputi(m);
    inputi(n);
    printf("Ackerman(%d, %d) = %d\n", m, n, Ack(m, n));
    return 0;
}
