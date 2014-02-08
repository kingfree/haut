#include <stdio.h>
#include <time.h>
#define MAX 4263116
long long p[MAX+10], num;
long long i, n;
int isprime(long long k) {
    for(i = 1; i < num && p[i]*p[i] <= k; i++)
        if(k % p[i] == 0)
            return 0;
    return 1;
}
int main() {
    double start, finish;
    start = clock();
    p[0] = 2;
    num = n = 1;
    while(num <= MAX) {
        n += 2;
        if(isprime(n)) {
            p[num++] = n;
            if(num % 10000 == 0)
                fprintf(stderr, ".");
        }
    }
    finish = clock();
	fprintf(stderr, "%.0lf s\n", (finish - start) / CLOCKS_PER_SEC);
    printf("%lld\n", p[MAX-1]); //72621257
    return 0;
}
