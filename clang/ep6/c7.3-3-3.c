#include <stdio.h>
#include <string.h>
int main() {
	char s[1 << 10];
	int i, l;
//	fprintf(stderr, "%d\n", sizeof(s));
//	for(i = 'A'; i <= 'Z'; i++)
//		fprintf(stderr, "(%c)[%c] ", i, 'A' + (25 - (i - 'A')));
	gets(s);
	l = strlen(s);
	for(i = 0; i < l; i++)
		if(s[i] >= 'A' && s[i] <= 'Z')
			s[i] = 'A' + (25 - (s[i] - 'A'));
	printf(s);
	return 0;
}