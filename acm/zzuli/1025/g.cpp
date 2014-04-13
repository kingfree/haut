#include <cstdio>
#define BITS 0x20 
#define CROL(v, b) ((v << b) | (v >> (BITS - b)))
#define CROR(v, b) ((v >> b) | (v << (BITS - b)))

bool ok(int a) {
    int b;
    // fprintf(stderr, "\n%x: ", a);
    for (int i = 0; i < BITS; i += 2) {
        b = CROL(a, i);
        // fprintf(stderr, "(%d)%x ", i, b);
        if (0x00 <= b && b <= 0xff) return true;
    }
    return false;
}

int main() {
    // fprintf(stderr, "%zd\n", sizeof(int));
    int t;
    int n;
    scanf("%d", &t);
    while (t--) {
        scanf("%x", &n);
        printf("%s\n", ok(n) ? "YES" : "NO");
    }
    return 0;
}
