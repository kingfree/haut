#include <stdlib.h>
#include <string.h>

typedef struct string {
    char *data;
    size_t len;
    size_t size;
} string;

string *stringncat(string *s1, const char *s2, size_t n);
void stringfree(string *s);

