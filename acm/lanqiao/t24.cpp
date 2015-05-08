#include <cstdio>

int a, b, c;

int gcd(int a, int b) {
  return b ? gcd(b, a % b) : a;
}

void solve() {
  int d = 0;
  d = a / gcd(a, b) * b;
  d = c / gcd(c, d) * d;
  printf("%d\n", d);
}

int main() {
  while (scanf("%d%d%d", &a, &b, &c) > 0) {
    solve();
  }
  return 0;
}
