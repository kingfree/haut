#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void swap(int *a, int *b) {
	int t;
	t = *a, *a = *b, *b = t;
}
void bubble(int *a, int n) {
	int i, j;
	for(i = 1; i < n; i++)
		for(j = 0; j < n - i; j++)
			if(a[j] > a[j + 1])
				swap(a + j, a + j + 1);
}
int main() {
	int a[10000], n, i;
	scanf("%d", &n);
	srand(time(0) + n);
	for(i = 0; i < n; i++)
		printf("%d ", a[i] = rand() % (n * 10));
	printf("\n");
	bubble(a, n);
	for(i = 0; i < n; i++)
		printf("%d ", a[i]);
	printf("\n");
	return 0;
}
