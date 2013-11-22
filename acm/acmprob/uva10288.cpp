#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef long long int int64;
int64 gcd(int64 a, int64 b) {
	return b ? gcd(b, a % b) : a;
}
int main() {
	int n, k;
	int64 p, q, s, t, m;
	// 计算：n \Sum_{i=1}^n (1 / i)
	while(~scanf("%d", &n)) {
		p = q = 1;
		k = 0;
		for(int i = 2; i <= n; i++) {
			s = p / gcd(p, i) * i;
			q = s / p * q + s / i;
			t = gcd(p = s, q);
			p /= t, q /= t;
			k += (int) (q / p), q %= p;
		}
		k *= n;
		q *= (int64) n;
		k += (int) (q / p), q %= p;
		t = gcd(p, q);
		p /= t, q /= t;
		s = (int) (log(p) / log(10) + 1);
		m = t = (int) (log(k) / log(10) + 2);
		if(q) {
			while(t--) printf(" ");
			printf("%lld\n", q);
			printf("%d ", k);
			while(s--) printf("-");
			printf("\n");
			while(m--) printf(" ");
			printf("%lld\n", p);
		} else {
			printf("%d\n", k);
		}
	}
	return 0;
}