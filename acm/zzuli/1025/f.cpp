#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 1000 + 10;
struct node {
    int d, t;
    bool operator< (const node& y) const {
        return d < y.d;
    }
} v[MAXN];

int main() {
    int n;
    while(scanf("%d", &n) > 0) {
        for (int i = 1; i <= n; i++) {
            scanf("%d", &v[i].d);
            v[i].t = i;
        }
        sort(v + 1, v + n + 1);
        for (int i = 1; i < n; i++) {
            printf("%d ", v[i].t);
        }
        printf("%d\n", v[n].t);
    }
    return 0;
}

