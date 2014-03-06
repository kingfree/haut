#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

int n;
int s = 0, t = 0;

bool C(int a, int b, int c) {
  s++;
  int l[10] = {0};
  for (; a; a /= 10) l[a % 10]++;
  for (; b; b /= 10) l[b % 10]++;
  for (; c; c /= 10) l[c % 10]++;
  if (l[0] > 0) {
    return false;
  }
  for (int i = 1; i <= 9; i++) {
    if (l[i] != 1) {
      return false;
    }
  }
  return true;
}

bool D(int x) {
  t++;
  int l[10] = {0};
  for(; x; x /= 10) {
    if(x % 10 == 0 || l[x % 10]++ > 0) {
      return false;
    }
  }
  return true;
}

void solve() {
  int res = 0;
  for (int left = 1; left < n; left++) if (D(left)) {
    for (int down = 1; down <= 9876; down++) if (D(down)) {
      int up = (n - left) * down;
      if (D(up) && C(left, up, down)) {
        // fprintf(stderr, "%d+%d/%d\n", left, up, down);
        res += 1;
      }
    }
  }
  printf("%d\n", res);
  fprintf(stderr, "%d %d\n", s, t);
}

int main() {
  while (scanf("%d", &n) > 0) {
    solve();
  }
  return 0;
}
