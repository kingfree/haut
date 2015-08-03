#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/rand.h>
#include <openssl/pem.h>
#include <ctype.h>
#include <getopt.h>

static const char *optString = "i:o:edh?";

static const struct option options[] = {
    {"input", required_argument, NULL, 'i'},
    {"output", required_argument, NULL, 'o'},
    {"encrypt", no_argument, NULL, 'e'},
    {"decrypt", no_argument, NULL, 'd'},
    {"help", no_argument, NULL, 'h'},
    {NULL, no_argument, NULL, 0}};

void display_usage()
{
    printf(
        "用法: enc [选项] -i<输入文件> -o<输出文件>\n"
        "选项: -e 加密\n"
        "      -d 解密 (默认)\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    char *input_filename, *output_filename;
    int encrypt = 0;

    int opt;
    int index;

    while ((opt = getopt_long(argc, argv, optString, options, &index)) != -1) {
        switch (opt) {
        case 'i':
            input_filename = optarg;
            break;
        case 'o':
            output_filename = optarg;
            break;
        case 'e':
            encrypt = 1;
            break;
        case 'd':
            encrypt = 0;
            break;
        case 'h':
        case '?':
            display_usage();
            break;
        default:
            break;
        }
    }

    if (!input_filename || !output_filename) {
        display_usage();
    }
    return 0;
}
