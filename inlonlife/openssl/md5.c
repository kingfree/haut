#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <ftw.h>

#if defined(__APPLE__)
#define COMMON_DIGEST_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
#else
#include <openssl/md5.h>
#endif

#define BUFFSIZE 1024

void calc_md5(const char *filename)
{
    unsigned char c[MD5_DIGEST_LENGTH];

    FILE *file = fopen(filename, "rb");
    MD5_CTX md5;
    int bytes;
    unsigned char data[BUFFSIZE];

    if (file == NULL) {
        perror("文件打开失败");
        return;
    }

    MD5_Init(&md5);
    while ((bytes = fread(data, 1, BUFFSIZE, file)) != 0)
        MD5_Update(&md5, data, bytes);
    MD5_Final(c, &md5);

    int i;
    for (i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
    printf(" %s\n", filename);

    fclose(file);
}

#ifndef USE_FDS
#define USE_FDS 15
#endif

int print_entry(const char *filepath, const struct stat *info,
        const int typeflag, struct FTW *pathinfo)
{
    if (typeflag == FTW_F) {
        calc_md5(filepath);
    }

    return 0;
}

int print_directory_tree(const char *const dirpath)
{
    int result;

    if (dirpath == NULL || *dirpath == '\0') return errno = EINVAL;

    result = nftw(dirpath, print_entry, USE_FDS, FTW_PHYS);
    if (result >= 0) errno = result;

    return errno;
}

int main(int argc, char *argv[])
{
    if (argc < 2) argv[argc++] = ".";
    for (int i = 1; i < argc; i++) {
        if (print_directory_tree(argv[i])) {
            perror("目录打开失败");
            exit(1);
        }
    }

    return 0;
}
