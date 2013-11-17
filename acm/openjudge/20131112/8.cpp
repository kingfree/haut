#include <cstdio>
#include <set>
#include <algorithm>
#define p(x) fprintf(stderr, #x " = %d\n", x)
#define N 1000
using namespace std;
void pf(int *s, int *l) {
    printf("%d", l - s);
    for(int *i = s; i != l; i++)
        printf(" %d", *i);
    printf("\n");
}
int main() {
    int n, m, *l;
    int a[N], b[N], c[N];
    scanf("%d%d", &n, &m);
    for(int i = 0; i< n; i++)
        scanf("%d", a + i);
    sort(a, a + n);
    for(int i = 0; i< m; i++)
        scanf("%d", b + i);
    sort(b, b + m);
    // pf(a, a + n), pf(b, b + m);
    l = set_union(a, a + n, b, b + m, c);
    pf(c, l);
    l = set_intersection(a, a + n, b, b + m, c);
    pf(c, l);
    l = set_difference(a, a + n, b, b + m, c);
    pf(c, l);
    return 0;
}