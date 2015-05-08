#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

const int BUSHI = 5;
const int RED = 1, BLUE = 2;
string bushiname[3][BUSHI] = {
  {"dragon", "ninja", "iceman", "lion", "wolf"},
  {"iceman", "lion", "wolf", "ninja", "dragon"},
  {"lion", "dragon", "ninja", "iceman", "wolf"}
};

class Bushi {};

class Shireibu {
  int seimei;
  int type;
  vector<Bushi> bushi[BUSHI];
  int status;
  int jikan;
  string name;

  void Print() {
    printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n", );
  }

  void Stop() {
    printf("%03d %s headquarter stops making warriors\n", jikan.c_str(), name);
  }

public:
  Shireibu(string namae, int first, int atti, int a[]) {
    name = namae;
    seimei = first;
    type = atti;
    status = 0;
    jikan = -1;
    for (int j = 0; j < BUSHI; j++) {
      bushi[find(bushiname[atti], bushiname[atti] + BUSHI, bushiname[0][j]) - bushiname[atti]] = a[j];
      strength[j] = a[j];
    }
    for (int j = 0; j < BUSHI; j++) {
      fprintf(stderr, "%s %d\n", bushiname[type][j].c_str(), bushi[j]);
    }
  }

  bool Step() {
    jikan += 1;
  }
};

int main() {
  int n;
  scanf("%d\n", &n);
  int M, a[BUSHI];
  for (int i = 1; i <= n; i++) {
    scanf("%d\n", &M);
    for (int j = 0; j < BUSHI; j++) {
      scanf("%d", &a[j]);
    }
    Shireibu red("red", M, RED, a);
    Shireibu blue("blue", M, BLUE, a);
    while (red.Step() || blue.Step())
      ;
  }
  return 0;
}