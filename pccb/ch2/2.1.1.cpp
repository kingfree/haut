/* 递归 */
#include <cstdio>

int fact(int n) {
  return n ? n * fact(n - 1) : 1;
}

int fib1(int n) {
  return n < 2 ? n : fib1(n - 1) + fib1(n - 2);
}

const int MAXN = (int) 1e6;
int memo[MAXN + 1] = {0, 1};

int fib(int n) {
  return n < 2 || memo[n] ? memo[n] : memo[n] = fib(n - 1) + fib(n - 2);
}

int main() {
  int n;
  scanf("%d", &n);
  printf("fact(%d) = %d\n", n, fact(n));
  scanf("%d", &n);
  printf("fib(%d) = %d\n", n, fib(n));
  return 0;
}
