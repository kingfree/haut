#include <stdio.h>
int main() {
    int i, n;
    double height, distance;
    scanf("%lf", &height);
    scanf("%d", &n);
    distance = height;
    for(i = 1; i <= n; i++) {
        distance += height;
        height *= 0.5;
    }
    printf("%.1lf %.1lf\n", distance, height);
    return 0;
}
