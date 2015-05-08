#include <cstdio>
#include <iostream>
using namespace std;

const int MAXN = 22;
const int MAXM = 10010;
int res[MAXM];

int main() {
  int days[MAXN], n = 0, m = 0;
  while (scanf("%d", &days[n]), days[n] > 0) {
    m = max(m, days[n++]);
  }
  fprintf(stderr, "%d ", m);
  int k = 0;
  for (int i = 1; k <= m; i++) {
    for (int j = 1; j <= i; j++) {
      res[k + j] = res[k + j - 1] + i;
    }
    k += i;
  }
  for (int i = 0; i < 10; i++)
    fprintf(stderr, "%d ", res[i]);
  for (int i = 0; i < n; i++)
    printf("%d %d\n", days[i], res[days[i]]);
  return 0;
}
