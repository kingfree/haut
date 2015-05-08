#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

const int MAXN = 1000 + 1;
int m, n;
bool f[MAXN * MAXN];

int solve() {
  int l = m * n;
  f[0] = true;
  for (int i = 1; i < l; i++) {
    f[i] = false;
    if ((i >= m && f[i - m]) || (i >= n && f[i - n])) {
      f[i] = true;
    }
  }
  int ans = 0;
  for (int i = 0; i < l; i++) {
    if(i > ans && !f[i]) {
      ans = i;
    }
  }
  printf("%d\n", ans);
}

int main() {
  while (scanf("%d%d", &m, &n) > 0) {
    solve();
  }
  return 0;
}
