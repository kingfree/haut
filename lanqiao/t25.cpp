#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 30;
const int MAXM = 9 + 4 * 30 + 1;
char c[MAXM][MAXM];
int n;

void set(int i, int j) {
  c[i][j] = '$';
}

void fill_l(int l, int s, int t) { // 填充行
  for (int i = s + 2; i < t - 2; i++) {
    set(l, i);
  }
}

void fill_h(int h, int s, int t) { // 填充列
  for (int i = s + 2; i < t - 2; i++) {
    set(i, h);
  }
}

const char moto[10][10] = {
  "..$$$$$..",
  "..$...$..",
  "$$$.$.$$$",
  "$...$...$",
  "$.$$$$$.$",
  "$...$...$",
  "$$$.$.$$$",
  "..$...$..",
  "..$$$$$..",
}; // 9 * 9

void fill_moto(int s) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (moto[i][j] == '$') {
        set(s + i, s + j);
      }
    }
  }
}

void print(int s, int t) {
  for (int i = s; i < t; i++) {
    puts(c[i]);
  }
  printf("\n");
}

void solve() {
  int s = 0, t = 9 + 4 * n;
  for (int i = s; i < t + 1; i++) {
    fill(c[i] + s, c[i] + t, '.');
    c[i][t] = '\0';
  }
  for (; t - s > 9; s +=2, t -=2) {
    fill_l(s, s, t);
    fill_l(t - 1, s, t);
    fill_h(s, s, t);
    fill_h(t - 1, s, t);
    set(s + 2, s + 2); // 左上
    set(s + 2, s + 1);
    set(s + 1, s + 2);
    set(s + 2, t - 3); // 左下
    set(s + 2, t - 2);
    set(s + 1, t - 3);
    set(t - 3, s + 2); // 右上
    set(t - 3, s + 1);
    set(t - 2, s + 2);
    set(t - 3, t - 3); // 右下
    set(t - 3, t - 2);
    set(t - 2, t - 3);
  }
  fill_moto(s);
  print(0, 9 + 4 * n);
}

int main() {
  while (scanf("%d", &n) > 0) {
    n -= 1;
    solve();
  }
  return 0;
}
