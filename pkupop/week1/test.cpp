#include <cstdio>
#include <iostream>
using namespace std;

int main() {
  printf("%d\n", 34 & 27);
  printf("%x\n",-12 >> 2 );
  printf("%d\n",26 | 14);
  printf("%d\n",18 ^ 22);
  int * p = new int[12];
  printf("%d\n", sizeof(p[0]));
  int a = 3, b = 5;
  int & r = a;
  r = b;
  b = 7;
  cout << a << endl;
  return 0;
}
