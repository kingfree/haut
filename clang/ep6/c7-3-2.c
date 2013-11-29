#include <stdio.h>
int main() {
	int n, a[10][10], i, j;
	scanf("%d", &n);
	for(i = 1; i <= n; i++)
		for(j = 1; j <= i; j++)
			a[i][j] = i * j;
	printf("%c", '*');
	for(j = 1; j <= n; j++)
		printf("%4d", j);
	printf("\n");
	for(i = 1; i <= n; i++) {
		printf("%d", i);
		for(j = 1; j <= i; j++)
			printf("%4d", a[i][j]);
		printf("\n");
	}
	return 0;
}