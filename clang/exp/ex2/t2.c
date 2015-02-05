#include <stdio.h>
#include <string.h>

const char *week[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

int getindex(const char **list, size_t n, const char *key)
{
    int i = 0;
    for (i = 0; i < n; i++) {
        if (strcmp(list[i], key) == 0) {
            return i + 1;
        }
    }
    return -1;
}

int main()
{
    char s[16];
    scanf("%s", s);
    printf("%d\n", getindex(week, 7, s));
    return 0;
}
