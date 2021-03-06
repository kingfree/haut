#include <stdio.h>
int C[16] = {0};
void push(int k, int v) {
    C[k] = (C[k] << 1) + v;
}
int main() {
    int A[] = {0, 0, 1, 1};
    int B[] = {0, 1, 0, 1};
    int a, b, i;
    for(i = 0; i < 4; i++) {
        a = A[i], b = B[i];
        push(0x0, 0);         //  FALSE
        push(0x1, a & b);     // A  &  B
        push(0x2, a & !b);    // A  & !B
        push(0x3, a);         //    A
        push(0x4, !a & b);    // !A &  B
        push(0x5, b);         //    B
        push(0x6, !(a == b)); // A (+) B
        push(0x7, a | b);     // A  |  B
        push(0x8, !(a | b));  // ! (A|B)
        push(0x9, a == b);    // A  =  B
        push(0xA, !b);        //   !B
        push(0xB, a | !b);    // A  | !B
        push(0xC, !a);        //   !A
        push(0xD, !a | b);    // A ==> B
        push(0xE, !(a & b));  // ! (A&B)
        push(0xF, 1);         //  TRUE
    }
    for(i = 0; i < 16; i++) {
        printf("%X ", C[i]);
    }
    return 0;
}
