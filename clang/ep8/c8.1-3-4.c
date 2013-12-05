#include <stdio.h>
#include <string.h>
int is_palindrome(char const *s) {
	int i, l = strlen(s);
	for(i = 0; i < l; i++)
		if(s[i] != s[l - i - 1])
			return 0;
	return i;
}
int main() {
	char s[1 << 10];
	while(gets(s) != NULL)
		puts(is_palindrome(s) ? "YES" : "NO");
	return 0;
}
