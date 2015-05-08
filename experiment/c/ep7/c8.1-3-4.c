#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int search(int *list, int n, int x) {
	int i;
	for(i = 0; i < n; i++)
		if(list[i] == x)
			return i;
	return -1;
}
int main() {
	int a[1 << 10], n, i, k, x;
	scanf("%d", &n);
	srand(time(0) + n);
	for(i = 0; i < n; i++)
		printf("%d ", a[i] = rand() % (n * 10));
	printf("\n²éÕÒ: ");
	scanf("%d", &x);
	if((k = search(a, n, x)) < 0)
		printf("Not Found.\n");
	else
		printf("Found at %d.\n", k);
	return 0;
}
