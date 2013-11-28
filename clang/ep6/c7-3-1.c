#include <stdio.h>
int main() {
	int n, i, j, a[8][8], s = 0;
	scanf("%d", &n);
	for(i = 1; i <= n; i++)
		for(j = 1; j <= n; j++) {
			scanf("%d", a[i] + j);
			if(i != n && j != n && i + j != n + 1)
				s += a[i][j];
		}
	printf("%d\n", s);
	return 0;
}