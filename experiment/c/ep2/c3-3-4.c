#include <stdio.h>
int main() {
    int i, c;
    double t;
    for(i = 0; i < 5; i++) {
        printf("[1] Æ»¹û\n[2] Àæ\n[3] éÙ×Ó\n[4] ÆÏÌÑ\n[0] ÍË³ö\nÇëÊäÈë£º");
        scanf("%d", &c);
        if(c == 0) break;
        t = 0;
        switch(c) {
            case 1: t = 3.00; break;
            case 2: t = 2.50; break;
            case 3: t = 4.10; break;
            case 4: t = 10.2; break;
        }
        printf("%.1lf\n", t);
    }
    return 0;
}
