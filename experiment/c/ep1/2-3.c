/* clac the temp. */
#include <stdio.h>
int main(void) {
    int F, C;
    F = 100;
    C = 5 * (F - 32) / 9;
    printf("fahr=%d, celsius=%d\n", F, C);
    return 0;
}
