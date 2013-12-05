#include <stdio.h>
#include <string.h>
char *strmcpy(char *s, char const *t, size_t m) {
	return strcpy(s, t + m);
}
int main() {
	int m;
	char s[1 << 10], t[1 << 10];
	gets(t);
	scanf("%d", &m);
	puts(strmcpy(s, t, m - 1));
	return 0;
}
