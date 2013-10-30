#include <stdio.h>
int main() {
	double sal, rate, tax, ded;
	scanf("%lf", &sal);
	if(sal <= 3500)
		rate = 0, ded = 0;
	else if(sal <= 5000)
		rate = 3, ded = 0;
	else if(sal <= 8000)
		rate = 10, ded = 105;
	else if(sal <= 12500)
		rate = 20, ded = 555;
	else if(sal <= 38500)
		rate = 25, ded = 1005;
	else if(sal <= 58500)
		rate = 30, ded = 2755;
	else if(sal <= 83500)
		rate = 35, ded = 5505;
	else
		rate = 45, ded = 13505;
	tax = rate / 100.0 * (sal - 3500) - ded;
	printf("%.2lf\n", tax);
	return 0;
}
