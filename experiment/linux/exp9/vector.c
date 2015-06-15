#include "vector.h"

int int_sum(int a[], int len)
{
    int sum = 0;
    int i;
    for (i = 0; i < len; i++) {
        sum += a[i];
    }
    return sum;
}
