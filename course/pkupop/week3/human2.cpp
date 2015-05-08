#include <iostream>
using namespace std;
class Sample{
public:
  int v;
  Sample() { };
  Sample(int n):v(n) { };
  Sample(const Sample & x) { v = 2 + x.v ; }
};
Sample PrintAndDouble( Sample o) {
  cout << o.v;
  o.v = 2 * o.v;
  return o;
}
int main()  {
  Sample a(5);
  Sample b = a;
  Sample c = PrintAndDouble( b );
  cout << endl;
  cout << c.v << endl;
  Sample d;
  d = a;
  cout << d.v ;
}