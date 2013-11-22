#include <stdio.h>
#include <math.h>
inline double sqr(double x) {
    return x * x;
}
double dist(double x1, double y1, double x2, double y2) {
    return sqrt(sqr(x1 - x2) + sqr(y1 - y2));
}
int main() {
    double a, b, c, d;
    while(scanf("%lf%lf%lf%lf", &a, &b, &c, &d) == 4)
        printf("%.2lf\n", dist(a, b, c, d));
    return 0;
}
