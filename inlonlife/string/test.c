#include <stdio.h>
#include <assert.h>

#include "string.h"

int main()
{
    string *s = string_new();

    string_cat(s, "%sworld", "hello");

    string *t = string_build("hello%s", "world");

    printf("字符串: %s\n", string_cstr(s));
    printf("字符串长度: %ld\n", string_length(s));
    printf("字符串: %s\n", string_cstr(t));

    assert(string_compare(s, t) == 0);

    printf("含有前缀 'hell': %s\n", string_has_prefix(s, "hell") ? "是" : "否");
    printf("含有前缀 'ell': %s\n", string_has_prefix(s, "helll") ? "是" : "否");

    assert(string_has_prefix(s, "hell"));

    printf("含有后缀 'orld': %s\n", string_has_suffix(s, "orld") ? "是" : "否");
    printf("含有后缀 'orl': %s\n", string_has_suffix(s, "orl") ? "是" : "否");

    printf("%d\n", string_has(s, "e"));
    printf("%d\n", string_has(s, "E", true));
    printf("%d\n", string_has(s, "0", false));

    printf("替换 'o' 到 '蛤蛤' 共 %ld 次\n", string_replace(s, "o", "蛤蛤"));
    printf("字符串: %s\n", string_cstr(s));
    printf("字符串长度: %ld\n", string_length(s));

    string_set_range(s, "ラブライブ", 2, 3);
    printf("字符串: %s\n", string_cstr(s));
    printf("字符串长度: %ld\n", string_length(s));

    assert(string_equals(s, "heラブライブ蛤蛤w蛤蛤rld"));

    return 0;
}
