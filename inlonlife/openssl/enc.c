#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <pwd.h>
#include <unistd.h>

#include <openssl/evp.h>

static const char *optString = "c:i:o:edh?";

static const struct option options[] = {
    {"input", required_argument, NULL, 'i'},
    {"output", required_argument, NULL, 'o'},
    {"encrypt", no_argument, NULL, 'e'},
    {"decrypt", no_argument, NULL, 'd'},
    {"help", no_argument, NULL, 'h'},
    {NULL, no_argument, NULL, 0}};

#define default_chipher "aes-128-cbc"

void display_usage()
{
    printf(
        "用法: enc [选项] -i<输入文件> -o<输出文件>\n"
        "选项: -e       加密\n"
        "      -d       解密 (默认)\n"
        "      -c<编码> 加解密算法，默认为 " default_chipher "\n");
    exit(1);
}

#define BUFFSIZE 1024
unsigned char inbuf[BUFFSIZE], outbuf[BUFFSIZE + EVP_MAX_BLOCK_LENGTH];

int main(int argc, char *argv[])
{
    int ret = 0;
    char *infile = NULL, *outfile = NULL;
    int enc = 1;
    char *select_cipher = default_chipher;
    char *str = NULL;
    int in = -1, out = -1;
    EVP_CIPHER_CTX ctx;
    const EVP_CIPHER *cipher = NULL;
    const EVP_MD *dgst = NULL;
    unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
    int inl, outl;

    int opt;
    int index;

    while ((opt = getopt_long(argc, argv, optString, options, &index)) != -1) {
        switch (opt) {
        case 'c':
            select_cipher = optarg;
            break;
        case 'i':
            infile = optarg;
            break;
        case 'o':
            outfile = optarg;
            break;
        case 'e':
            enc = 1;
            break;
        case 'd':
            enc = 0;
            break;
        case 'h':
        case '?':
            display_usage();
            break;
        default:
            break;
        }
    }

    cipher = EVP_get_cipherbyname(select_cipher);
    if (!cipher) {
        fprintf(stderr, "%s 算法无效\n", select_cipher);
        goto end;
    }

    if (dgst == NULL)
        dgst = EVP_md5();

    if (!infile) {
        fflush(stdin);
        in = dup(STDIN_FILENO);
    } else
        in = open(infile, O_RDONLY);
    if (in < 0) {
        fprintf(stderr, "打开输入文件 %s 失败\n", infile);
        goto end;
    }

    for (;;) {
        str = getpass("输入密码: ");
        if (str) break;
        fprintf(stderr, "密码输入失败\n");
    }
    out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (out < 0) {
        fprintf(stderr, "打开输出文件 %s 失败\n", outfile);
        goto end;
    }

    unsigned char *sptr = NULL;

    if (!EVP_BytesToKey(cipher, dgst, sptr, (unsigned char *)str, strlen(str),
                        1, key, iv)) {
        fprintf(stderr, "密码转换失败\n");
        goto end;
    }

    EVP_CIPHER_CTX_init(&ctx);

    if (!EVP_CipherInit_ex(&ctx, cipher, NULL, NULL, NULL, enc)) {
        fprintf(stderr, "设置加密算法 %s 失败\n", EVP_CIPHER_name(cipher));
        goto end;
    }

    while((inl = read(in, inbuf, BUFFSIZE)) > 0) {
        if (inl <= 0) break;
        if (!EVP_CipherUpdate(&ctx, outbuf, &outl, inbuf, inl)) {
            fprintf(stderr, "%s失败\n", enc ? "加密" : "解密");
            goto end;
        }
        write(out, outbuf, outl);
    }
    if (!EVP_CipherFinal_ex(&ctx, outbuf, &outl)) {
        fprintf(stderr, "%s失败\n", enc ? "加密" : "解密");
        goto end;
    }
    write(out, outbuf, outl);

end:
    if (in >= 0) close(in);
    if (out >= 0) close(out);
    // EVP_CIPHER_CTX_cleanup(&ctx);

    return ret;
}
