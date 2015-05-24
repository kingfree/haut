#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define CODE_SIZE 0x10000
#define CODEPOINT 32

char codemap[CODE_SIZE][CODEPOINT];

int main(int argc, char const* argv[])
{
    // fprintf(stderr, "%ld\n", sizeof(codemap) / 1024);

    if (argc < 2) {
        printf("用法: %s <输入十六进制文件> <输出二进制文件>\n\n", argv[0]);
        printf("示例: %s unifont-7.0.06.hex unifont.bin\n\n", argv[0]);
        exit(1);
    }

    FILE* fin = fopen(argv[1], "r");
    if (!fin) {
        fprintf(stderr, "打开输入文件失败: %s\n", strerror(errno));
        exit(1);
    }

    int line;
    char s[80];
    char* p;
    char c[4] = { 0 };
    char x = 0;
    int i, j;
    int n;
    unsigned int d;

    n = 0;
    while (fscanf(fin, "%X:%s\n", &line, s) == 2) {
        // printf("%d:%s\n", line, s);
        for (i = 0, p = s; p[0]; i++) {
            c[0] = p[0];
            c[1] = p[1];
            c[2] = 0;
            d = strtoul(c, NULL, 16);
            x = (char)d;
            codemap[line][i] = x;
            //fprintf(stderr, "[%s]%X(%c)\n", c, (int)x, x);
            // fprintf(stderr, "%.2X", d);
            p += 2;
        }
        n++;
        // fprintf(stderr, "%d\n", i);
        // fprintf(stderr, "\n");
        // if (line > 100) break;
    }
    printf("读入 %d 个位图字符，最大到 %d 。\n", n, line);

    // for (i = 0; i < 40; i++) {
    //     fprintf(stderr, "%.4X:", i);
    //     for (j = 0; j < CODEPOINT; j++) {
    //         d = codemap[i][j];
    //         fprintf(stderr, "%.2X", d);
    //     }
    //     fprintf(stderr, "\n");
    // }

    FILE* bin = fopen(argv[2], "w");
    if (!bin) {
        fprintf(stderr, "打开输出文件失败: %s\n", strerror(errno));
        exit(1);
    }

    n = fwrite(codemap, sizeof(char), sizeof(codemap), bin);
    if (n < sizeof(codemap)) {
        fprintf(stderr, "写出失败: %s\n", strerror(errno));
        exit(1);
    }
    printf("写出 %d 个位图字符，计 %d 个字节（%dKB）。\n", n / (int)sizeof(char) / CODEPOINT, n, n / 1024);

    fclose(fin);
    fclose(bin);

    return 0;
}
