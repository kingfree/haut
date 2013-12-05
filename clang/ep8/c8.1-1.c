#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
	int n, i;
	char min[80], s[80];
	scanf("%d\n", &n);
	gets(min);
	for(i = 1; i < n; i++) {
		gets(s);
		if(strcmp(min, s) > 0)
			strcpy(min, s);
	}
	puts(min);
	return 0;
}
