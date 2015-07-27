#include "string.h"

string *stringncat(string *s1, const char *s2, size_t n)
{
    if (s1->len + n + 1 >= s1->size) {
        s1->size += n + 1 > 128 ? n + 1 : 128;
        if (s1->data == NULL) {
            s1->data = malloc(s1->size);
        } else {
            s1->data = realloc(s1->data, s1->size);
        }
        if (s1->data == NULL) return NULL;
    }
    // char *res = strncat(s1->data + s1->len, s2, n);
    char *p = s1->data + s1->len;
    for (size_t i = 0; i < n; i++) *p++ = s2[i];
    // printf("%ld %ld %s %ld\n", s1->len, s1->size, s1->data, n);
    s1->len += n;
    s1->data[s1->len] = '\0';  // strncat() 会自动加 '\0'
    return s1;
    // return res == s1->data ? s1 : NULL;
}

void stringfree(string *s)
{
    if (s->data) free(s->data);
    s->data = NULL;
    s->len = s->size = 0;
}
