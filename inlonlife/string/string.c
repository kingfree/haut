#include "string.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define BUFFSIZE 128

struct string {
    char *data;
    size_t len;
    size_t size;
};

string *string_new()
{
    string *s = malloc(sizeof(string));
    return string_init(s);
}

string *string_init(string *s)
{
    if (!s) return NULL;
    s->len = 0;
    s->size = 0;
    s->data = NULL;
    return s;
}

char *string_cstr(string *s)
{
    if (!s) return NULL;
    return s->data;
}

size_t string_length(string *s)
{
    if (!s) return 0;
    return s->len;
}

long string_empty(string *s)
{
    if (!s) return 1;
    return string_length(s) == 0;
}

void string_free(string *s)
{
    if (!s) return;
    if (s->data) free(s->data);
    s->data = NULL;
    s->len = s->size = 0;
}

static string *string_larger(string *s, size_t len)
{
    if (!s) return NULL;
    len ++;
    if (s->len + len >= s->size) {
        s->size += len > BUFFSIZE ? len : BUFFSIZE;
        if (s->data == NULL) {
            s->data = malloc(s->size);
        } else {
            s->data = realloc(s->data, s->size);
        }
        if (s->data == NULL) return NULL;
    }
    return s;
}

string *string_ncat(string *s, size_t n, const char *fmt, ...)
{
    if (!s) return NULL;
    if (string_larger(s, n) == NULL) return NULL;

    va_list p;
    va_start(p, fmt);
    s->len += vsnprintf(s->data + s->len, n, fmt, p);
    va_end(p);

    return s;
}

string *string_cat(string *s, const char *fmt, ...)
{
    if (!s) return NULL;
    va_list p;
    va_start(p, fmt);
    return string_ncat(s, strlen(fmt) * 6, fmt, p);
}

long string_compare(string *s1, string *s2)
{
    if (s1 == NULL && s2 == NULL) return 0;
    if (s1 == NULL) return 1;
    if (s2 == NULL) return -1;
    return strcmp(s1->data, s2->data);
}

string *string_tolower(string *s)
{
    if (!s) return NULL;
    for (size_t i = 0; i < s->len; i++) {
        if (s->data[i] >= 'A' && s->data[i] <= 'Z') {
            s->data[i] += 32;
        }
    }
    return s;
}

string *string_toupper(string *s)
{
    if (!s) return NULL;
    for (size_t i = 0; i < s->len; i++) {
        if (s->data[i] >= 'a' && s->data[i] <= 'z') {
            s->data[i] -= 32;
        }
    }
    return s;
}

#define bool_icase \
    bool icase = false; \
    va_list p; \
    va_start(p, find); \
    long tmpicase = va_arg(p, long); \
    if (tmpicase == true) icase = true; \
    va_end(p);

bool string_has(string *s, const char *find, ... /* bool icase */)
{
    if (!s || !find) return 0;

    bool_icase;

    return string_find(s, find, icase) >= 0;
}

bool string_has_s(string *s, string *find, ... /* bool icase */)
{
    if (!s || !find) return 0;

    bool_icase;

    return string_find(s, find->data, icase) >= 0;
}

bool string_has_prefix(string *s, const char *find, ... /* bool icase */)
{
    if (!s || !find) return 0;

    bool_icase;

    return string_find(s, find, icase) == 0;
}

bool string_has_prefix_s(string *s, string *find, ... /* bool icase */);

bool string_has_suffix(string *s, const char *find, ... /* bool icase */);
bool string_has_suffix_s(string *s, string *find, ... /* bool icase */);

long string_find(string *s, const char *find, ... /* bool icase */)
{
    if (!s || !find) return 0;

    bool_icase;

    char *index;

    if (icase) {
        index = strcasestr(s->data, find);
    } else {
        index = strstr(s->data, find);
    }

    return index ? index - s->data : -1;
}

long string_find_s(string *s, string *find, ... /* bool icase */)
{
    if (!s || !find) return 0;

    bool_icase;

    char *index;

    if (icase) {
        index = strcasestr(s->data, find->data);
    } else {
        index = strstr(s->data, find->data);
    }

    return index ? index - s->data : -1;
}

long string_replace(string *s, const char *find, const char *replace, ... /* bool icase */);
long string_replace_s(string *s, string *find, string *replace, ... /* bool icase */);

string *string_separate(string **s, const char *sep, ... /* bool icase */);
string *string_separate_s(string **s, string *sep, ... /* bool icase */);

char string_char_at(string *s, long index);

string *string_set_range(string *s, const char *rep, long from, long to)
{
    if (!s || !rep) return NULL;

    if (from < 0 || from >= (long)s->len || to < 0 || to >= (long)s->len) return NULL;
    if (from > to) return NULL;

    long l = strlen(rep);
    long len = l - (to - from + 1);

    if (!string_larger(s, len)) return s;

    long i;

    for (i = to + len; i >= to; i--) {
        s->data[i + len] = s->data[i];
    }

    for (i = 0; i < l; i++) {
        s->data[i + from] = rep[i];
    }

    s->len += len;

    return s;
}

string *string_set_range_s(string *s, string *rep, long from, long to)
{
    if (!s || !rep) return NULL;

    return string_set_range(s, rep->data, from, to);
}


string* string_substring_new(string *s, long from, long to);

long string_write_to_file(string *s, FILE *file);
long string_write_to_filename(string *s, const char *filename);

