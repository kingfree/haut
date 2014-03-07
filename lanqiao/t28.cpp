#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = 100 * 100 + 1;
int a[MAXN];
int n, k = 0;

void solve() {
  sort(a, a + k);
  int br = 0, co = 0;
  for (int i = 0; i < k; i++) {
    if (a[i] == a[i + 1]) {
      co = i;
    } else if (a[i] + 1 != a[i + 1]) {
      br = i;
    } else if (co && br) {
      printf("%d %d\n", a[br] + 1, a[co]);
      return ;
    }
  }
}

int main() {
  scanf("%d\n", &n);
  while (scanf("%d", &a[k++]) == 1)
    ;
  solve();
}
