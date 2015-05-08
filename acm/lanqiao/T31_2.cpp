
int x, y, q; // a(x+qb)+b(y-qa)=c
void gcd(int a, int b) {
  if (b) {
    gcd(b, a % b);
    swap(x, y);
    y -= a / b * x;
  } else {
    x = 1;
    y = 0;
    q = a;
  }
}
