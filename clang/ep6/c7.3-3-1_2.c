#include <stdio.h>
#include <string.h>
int main() {
	char *d;
	char c, s[100];
	scanf("%c\n", &c);
	scanf("%s", s);
	d = strchr(s, c);
	if(d == NULL)
		printf("Not Found\n");
	else
		printf("%d\n", d - s);
	return 0;
}