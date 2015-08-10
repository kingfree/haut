#include <stdio.h>

#include "string.h"

int main()
{
    string *s = string_new();

    string_cat(s, "hello");

    printf("字符串: %s\n", string_cstr(s));

    printf("字符串长度: %ld\n", string_length(s));

    printf("%d\n", string_has(s, "e"));
    printf("%d\n", string_has(s, "E", true));
    printf("%d\n", string_has(s, "0", false));

    printf("%s\n", string_cstr(string_set_range(s, "ラブライブ", 2, 3)));
    printf("字符串长度: %ld\n", string_length(s));

    return 0;
}
