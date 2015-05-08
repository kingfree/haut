#include <cstdio>

int main() {
  unsigned long long int n, s = 0;
#ifdef ONLINE_JUDGE
  scanf("%I64d", &n);
#else
  scanf("%lld", &n);
#endif
  if (n % 2 == 0) {
    s = n / 2 * (n + 1);
  } else {
    s = (n + 1) / 2 * n;
  }
#ifdef ONLINE_JUDGE
  printf("%I64d\n", s);
#else
  printf("%lld\n", s);
#endif
  return 0;
}
