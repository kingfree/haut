#include <cstdio>
#include <cmath>
#include <climits>

typedef long long int int64;
int64 f1, f2, f3;
int64 n, m, p, s;

// fn = (((1+\sqrt5)/2)^n-((1-\sqrt5)/2)^n)/\sqrt{5}
// s = (f(n)+f(n+1))%f(m)+f(m)-1

inline int64 fn(int64 n)
{
	long double nn = n;
	long double sqrt5 = sqrt(5.0);
	long double a = pow((1 + sqrt5) / 2, nn);
	long double b = pow((1 - sqrt5) / 2, nn);
	long double s = (a - b) / sqrt5;
	// fprintf(stderr, "%lf %lf %lf %lf \n", sqrt5, a, b, s);
	return (int64)s;
}

int main()
{
	// fprintf(stderr, "%lld\n", LONG_LONG_MAX);
	scanf("%lld %lld %lld", &n, &m, &p);
	f1 = fn(n);
	f2 = fn(n + 1);
	f3 = fn(m);
	// fprintf(stderr, "%lld\n", f1);
	// fprintf(stderr, "%lld\n", f2);
	// fprintf(stderr, "%lld\n", f3);
	s = (f1 % f3 % p + f2 % f3 % p + f3 % p - 1) % f3 % p;
	printf("%lld\n", s);
	return 0;
}
