#include <cstdio>
#include <cstring>
#include <ios>
#include <string>
#include <algorithm>
using namespace std;

const int GAKUNEN = 4 + 1;

class Student {
  string name;
  int age;
  string id;
  int score[GAKUNEN], grades;

public:
  Student(char* namae, int nenrei, char* bangou) {
    name = namae;
    age = nenrei;
    id = bangou;
    fill_n(score, GAKUNEN, 0);
    grades = 0;
  }

  const char* getName() {
    return name.c_str();
  }

  const int getAge() {
    return age;
  }

  const char* getId() {
    return id.c_str();
  }

  bool setScore(int bunsu, int index = -1) {
    grades = index < 0 ? grades : index;
    if (grades >= GAKUNEN) {
      return false;
    }
    score[grades] = bunsu;
    return score[grades++] == bunsu;
  }

  int getAverage() {
    int sum = 0;
    for (int i = 0; i < grades; i++) {
      sum += score[i];
    }
    int ave = sum / grades;
    return ave;
  }
};

const int CHARN = 1 << 8;

char* strcrep(char *s, char from, char to) {
  for (int i = 0; s[i]; i++) {
    if (s[i] == ',') {
      s[i] = ' ';
    }
  }
  return s;
}

int main() {
  char s[CHARN];
  fgets(s, CHARN, stdin);
  strcrep(s, ',', ' ');
  // 暂时解决办法

  char namae[CHARN];
  int nenrei;
  char bangou[CHARN];
  int bunsu[4];
  sscanf(s, "%s %d %s %d %d %d %d", namae, &nenrei, bangou, &bunsu[0], &bunsu[1], &bunsu[2], &bunsu[3]);
  // fprintf(stderr, "[%s],%d,%s,%d,%d,%d,%d\n", namae, nenrei, bangou, bunsu[0], bunsu[1], bunsu[2], bunsu[3]);

  Student a(namae, nenrei, bangou);
  for (int i = 0; i < 4; i++) {
    a.setScore(bunsu[i]);
  }
  printf("%s,%d,%s,%d\n", a.getName(), a.getAge(), a.getId(), a.getAverage());
  return 0;
}
