#include <cstdio>
#include <algorithm>
using namespace std;

const int MAXN = (int) 5e4 + 1;
int a[MAXN];
int n;

void solve() {
  int res = 0;
  int chi, ooi;
  for (int i = 1; i <= n; i++) {
    chi = n, ooi = 1;
    for (int j = i; j <= n; j++) {
      chi = min(chi, a[j]);
      ooi = max(ooi, a[j]);
      if (ooi - chi == j - i) {
        res += 1;
      }
    }
  }
  printf("%d\n", res);
}

int main() {
  while (scanf("%d", &n) > 0) {
    for (int i = 1; i <= n; i++) {
      scanf("%d", &a[i]);
    }
    solve();
  }
}
