#include <stdio.h>
#include <stdlib.h>

#define N 12

int (*a[N]) (int, char const**);

int main(int argc, char const* argv[]) {
  printf("%d\n", atoi("ao"));
  a[0] = main;
  return 0;
}
