#include <stdio.h>
#include <stdlib.h>
#include <math.h>
double sqr(double x) {
    return x * x;
}
double dist(double x1, double y1, double x2, double y2) {
    return sqrt(sqr(x1 - x2) + sqr(y1 - y2));
}
int main() {
    double a, b, c, d;
    printf("Input (x1, y1): ");
    scanf("%lf%lf", &a, &b);
    printf("Input (x2, y2): ");
    scanf("%lf%lf", &c, &d);
    printf("Distance = %.2lf\n", dist(a, b, c, d));
    return 0;
}

