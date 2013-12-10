#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;
const int N = 16;
char letters[N];
int value[N], target;
void p(int x) {
  putchar(value[x] + 'A' - 1);
}
void doit(int l) {
  int a, b, c, d, e;
  for(a = 0; a < l; a++)
    for(b = 0; b < l; b++) if(a != b)
      for(c = 0; c < l; c++) if(a != c && b != c)
        for(d = 0; d < l; d++) if(a != d && b != d && c != d)
          for(e = 0; e < l; e++) if(a != e && b != e && c != e && d != e)
            if(value[a] - pow(value[b], 2.0)
                + pow(value[c], 3.0)
                - pow(value[d], 4.0)
                + pow(value[e], 5.0) == target) {
              p(a), p(b), p(c), p(d), p(e);
              printf("\n");
              return ;
            }
  puts("no solution");
}
bool cmp(int a, int b) {
  return a > b;
}
int main() {
  int i;
  while(~scanf("%d %s\n", &target, letters)) {
    if(!target && !strcmp(letters, "END"))
      return 0;
    for(i = 0; letters[i]; i++)
      value[i] = letters[i] - 'A' + 1;
    sort(value, value + i, cmp);
    doit(i);
  }
  return 0;
}

