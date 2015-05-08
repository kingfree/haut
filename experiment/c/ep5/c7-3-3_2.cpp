#include <stdio.h>
#include <algorithm>
using namespace std;
int main() {
    int n, i, a[20];
    scanf("%d", &n);
    for(i = 0; i < n; i++)
        scanf("%d", a + i);
    int *p = max_element(a, a + n);
    printf("%d %d\n", *p, p - a);
    return 0;
}
