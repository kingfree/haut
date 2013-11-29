#include <stdio.h>
#include <string.h>
int main() {
	int i, d, l;
	char c, s[100];
	scanf("%c\n", &c);
	scanf("%s", s);
	l = strlen(s);
	d = -1;
	for(i = 0; i < l; ++i)
		if(s[i] == c)
			d = i;
	if(d < 0)
		printf("Not Found\n");
	else
		printf("%d\n", d);
	return 0;
}