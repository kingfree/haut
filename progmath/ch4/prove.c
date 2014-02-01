#include <stdio.h>
void prove(int n) {
    // 数学归纳法
    int k;
    printf("现在开始证明P(%d)成立。\n", n);
    k = 0;
    printf("根据[步骤1]得出P(%d)成立。\n", k);
    while(k < n) {
        printf("根据[步骤2]可以说“若P(%d)成立，则P(%d)也成立”。\n", k, k + 1);
        printf("因此可以说“P(%d)是成立的”。\n", k + 1);
        k = k + 1;
    }
    printf("证明结束。\n");
}
int main() {
    int n;
    scanf("%d", &n);
    prove(n);
    return 0;
}
