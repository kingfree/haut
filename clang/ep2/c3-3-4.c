#include <stdio.h>
int main() {
    int i, c;
    double t;
    for(i = 0; i < 5; i++) {
        printf("[1] ƻ��\n[2] ��\n[3] ����\n[4] ����\n[0] �˳�\n�����룺");
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
