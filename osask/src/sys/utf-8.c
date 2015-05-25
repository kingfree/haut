#include "bootpack.h"

int utf8len(const char* s)
{
    int size = 0, index = 0;
    unsigned char *utf8 = (unsigned char *)s;
    unsigned char c;
    c = utf8[index++];
    while (c) {
        if ((c & 0x80) == 0) {
            index += 0;
        } else if ((c & 0xe0) == 0xe0) {
            index += 2;
        } else {
            index += 1;
        }
        size += 1;
        c = utf8[index++];
    }
    return size;
}

int* utf8str(const char* s)
{
    int codepoint;
    int size = 0, index = 0, out_index = 0;
    unsigned char *utf8 = (unsigned char*)s;
    unsigned char c;
    
    memman_t* memman = (memman_t*)MEMMAN_ADDR;
    int* unicode = (int*)memman_alloc_4k(memman, (utf8len(s) + 1) * sizeof(int));

    c = utf8[index++];
    while (c) {
        if ((c & 0x80) == 0) {
            codepoint = c;
        } else if ((c & 0xe0) == 0xe0) {
            codepoint = (c & 0x1F) << 12;
            c = utf8[index++];
            codepoint |= (c & 0x3F) << 6;
            c = utf8[index++];
            codepoint |= (c & 0x3F);
        } else {
            codepoint = (c & 0x3F) << 6;
            c = utf8[index++];
            codepoint |= (c & 0x3F);
        }
        c = utf8[index++];
        unicode[out_index++] = codepoint;
    }
    unicode[out_index] = 0;
    return unicode;
}

char* utf8char(const char* s, int* code)
{
    int codepoint;
    int size = 0, index = 0;
    unsigned char *utf8 = (unsigned char*)s;
    unsigned char c;

    c = utf8[index++];
    while (c) {
        if ((c & 0x80) == 0) {
            codepoint = c;
        } else if ((c & 0xe0) == 0xe0) {
            codepoint = (c & 0x1F) << 12;
            c = utf8[index++];
            codepoint |= (c & 0x3F) << 6;
            c = utf8[index++];
            codepoint |= (c & 0x3F);
        } else {
            codepoint = (c & 0x3F) << 6;
            c = utf8[index++];
            codepoint |= (c & 0x3F);
        }
        c = utf8[index++];
        *code = codepoint;
        return &utf8[index - 1];
    }
    return 0;
}
