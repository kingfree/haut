#include <stdio.h>
#include "vector.h"

int main(int argc, char* argv[])
{
    int n = 10;
    int a[10];
    int i;

    for (i = 0; i < n; i++) {
        a[i] = i;
    }

    int s = int_sum(a, n);
    printf("%d\n", s);

    return 0;
}
