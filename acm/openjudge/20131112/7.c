#include <stdio.h>
double f(double x) {
    return 2 * x*x*x - 4 * x*x + 3 * x - 6;
}
int main() {
    double a = -10, b = 10, e;
    double l = f(a),r = f(b),m;
    scanf("%lf", &e);
    do {
        m = f( (a + b) / 2);
        if(m == 0) break;
        if(m * l < 0)
            b = (a + b) / 2;
        else if(m * r<0)
            a = (a + b) / 2;
    } while(b - a > e);
    printf("%.8lf\n", (a + b) / 2);
    return 0;
} 