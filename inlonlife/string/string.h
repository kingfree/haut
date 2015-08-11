#ifndef _KF_STRING_H_
#define _KF_STRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct string string;

string *string_new();
string *string_build(const char *fmt, ...);
string *string_init(string *s);

void string_free(string *s);

char *string_cstr(string *s);

size_t string_length(string *s);
size_t string_size(string *s);

long string_empty(string *s);

string *string_ncat(string *s, size_t n, const char *fmt, ...);
string *string_cat(string *s, const char *fmt, ...);

bool string_equals(string *s1, const char *s2);
long string_compare(string *s1, string *s2);

string *string_tolower(string *s);
string *string_toupper(string *s);

bool string_has(string *s, const char *find, ... /* bool icase */);
bool string_has_s(string *s, string *find, ... /* bool icase */);

bool string_has_prefix(string *s, const char *find, ... /* bool icase */);
bool string_has_prefix_s(string *s, string *find, ... /* bool icase */);

bool string_has_suffix(string *s, const char *find, ... /* bool icase */);
bool string_has_suffix_s(string *s, string *find, ... /* bool icase */);

long string_find(string *s, const char *find, ... /* bool icase */);
long string_find_s(string *s, string *find, ... /* bool icase */);

long string_replace(string *s, const char *find, const char *replace, ... /* bool icase */);
long string_replace_s(string *s, string *find, string *replace, ... /* bool icase */);

string *string_separate(string **s, const char *sep, ... /* bool icase */);
string *string_separate_s(string **s, string *sep, ... /* bool icase */);

char string_char_at(string *s, long index);

string *string_set_range(string *s, const char *rep, long from, long to);
string *string_set_range_s(string *s, string *rep, long from, long to);

string* string_substring_new(string *s, long from, long to);

long string_write_to_file(string *s, FILE *file);
long string_write_to_filename(string *s, const char *filename);

#endif
