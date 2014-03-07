#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

int n;
int l[10] = {0}, r[10] = {0};
int s = 0, t = 0;

bool C(int a, int b, int c) { // 判断是否数字都用过一遍
  s++;
  for (int i = 1; i <= 9; i++) {
    if (l[i] != 1) {
      return false;
    }
  }
  return true;
}

bool D(int x) { // 判断没有重复数字
  t++;
  for (; x; x /= 10) {
    if (x % 10 == 0 || l[x % 10]++ > 0) { // 不包括 0
      return false;
    }
  }
  return true;
}

void solve() {
  int res = 0;
  for (int left = 1; left < n; left++) { // 整数部分
    fill(l, l + 10, 0); // 标记置空
    if (D(left)) {
      copy(l, l + 10, r); // 备份标记
      for (int down = 1; down <= 9876; down++) { // 分母
        copy(r, r + 10, l); // 还原标记
        if (D(down)) {
          int up = (n - left) * down; // 分子
          if (D(up) && C(left, up, down)) {
            // fprintf(stderr, "%d+%d/%d\n", left, up, down);
            res += 1;
          }
        }
      }
    }
  }
  printf("%d\n", res);
  // fprintf(stderr, "%d %d\n", s, t);
}

int main() {
  while (scanf("%d", &n) > 0) {
    solve();
  }
  return 0;
}
