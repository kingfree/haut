#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXS = 1000 + 1;
char op[MAXS], ed[MAXS];

void exchange(int i) {
  if (op[i] == '*') {
    op[i] = 'o';
  } else if (op[i] == 'o') {
    op[i] = '*';
  }
}

void solve() {
  int res = 0;
  int l = strlen(op);
  for (int i = 0; i < l; i++) {
    if (op[i] != ed[i]) {
      exchange(i);
      exchange(i + 1);
      res += 1;
    }
  }
  printf("%d\n", res);
}

int main() {
  scanf("%s\n", op);
  scanf("%s\n", ed);
  solve();
}
