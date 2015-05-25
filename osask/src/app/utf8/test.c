#include <stdio.h>

int main()
{
    int codepoint;
    int size = 0, index = 0, out_index = 0;
    unsigned char c;
    unsigned char *utf8 = (unsigned char *)"编码测试\nhello!おはよう";
    printf("%s\n", utf8);

    /* first calculate the size of the target string */
    c = utf8[index++];
    while(c) {
        if((c & 0x80) == 0) {
            index += 0;
        } else if((c & 0xe0) == 0xe0) {
            index += 2;
        } else {
            index += 1;
        }
        size += 1;
        c = utf8[index++];
    }

    index = 0;

    c = utf8[index++];
    while(c)
    {
        if((c & 0x80) == 0) {
            codepoint = c;
        } else if((c & 0xe0) == 0xe0) {
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

        printf("%.4X\n", codepoint);
    }
    return 0;
}
