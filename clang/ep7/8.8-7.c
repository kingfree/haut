#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
	int i, a[10], *j, s;
	srand(time(0));
	for(i = 0; i < 10; i++)
		printf("%d ", a[i] = rand() % 100);
	printf("\n");
	for(s = i = 0; i < 10; i++)
		s += a[i];
	printf("%d\n", s);
	for(s = 0, j = a; j <= a + 9; j++)
		s += *j;
	printf("%d\n", s);
	return 0;
}
