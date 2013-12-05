#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void swap(int *a, int *b) {
	int t;
	t = *a, *a = *b, *b = t;
}
void sort(int *a, int n) {
	int i, j, k;
	for(i = 0; i < n - 1; i++) {
		k = i;
		for(j = i + 1; j < n; j++)
			if(a[j] < a[k])
				k = j;
		swap(a + i, a + k);
	}
}
int main() {
	int a[10000], n, i;
	scanf("%d", &n);
	srand(time(0) + n);
	for(i = 0; i < n; i++)
		printf("%d ", a[i] = rand() % (n * 10));
	printf("\n");
	sort(a, n);
	for(i = 0; i < n; i++)
		printf("%d ", a[i]);
	printf("\n");
	return 0;
}
