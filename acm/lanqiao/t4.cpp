#include <cstdio>
#include <cmath>

const int MAXN = 1000000 + 1;
int F[MAXN] = {0, 1};
int n;

const int M = 10007;

void solve() {
  for (int i = 2; i <= n; i++) {
    F[i] = (F[i - 1] + F[i - 2]) % M;
  }
  printf("%d\n", F[n]);
}

int main() {
  scanf("%d", &n);
  solve();
  return 0;
}
