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
#include <openssl/sha.h>
#endif

#define BUFFSIZE 1024

void calc_hash(const char *filename)
{
    int n;
    unsigned char data[BUFFSIZE];
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("文件打开失败");
        return;
    }

    unsigned char md5[MD5_DIGEST_LENGTH];
    MD5_CTX md5_c;
    unsigned char sha1[SHA_DIGEST_LENGTH];
    SHA_CTX sha1_c;
    unsigned char sha512[SHA_DIGEST_LENGTH];
    SHA512_CTX sha512_c;

    MD5_Init(&md5_c);
    SHA1_Init(&sha1_c);
    SHA512_Init(&sha512_c);
    while ((n = fread(data, 1, BUFFSIZE, file)) != 0) {
        MD5_Update(&md5_c, data, n);
        SHA1_Update(&sha1_c, data, n);
        SHA512_Update(&sha512_c, data, n);
    }
    MD5_Final(md5, &md5_c);
    SHA1_Final(sha1, &sha1_c);
    SHA512_Final(sha512, &sha512_c);

    int i;
    printf("%s", filename);
    printf("\n%8s: ", "MD5");
    for (i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", md5[i]);
    printf("\n%8s: ", "SHA1");
    for (i = 0; i < SHA_DIGEST_LENGTH; i++) printf("%02x", sha1[i]);
    printf("\n%8s: ", "SHA512");
    for (i = 0; i < SHA_DIGEST_LENGTH; i++) printf("%02x", sha512[i]);
    printf("\n");

    fclose(file);
}

#ifndef USE_FDS
#define USE_FDS 15
#endif

int print_entry(const char *filepath, const struct stat *info,
        const int typeflag, struct FTW *pathinfo)
{
    if (typeflag == FTW_F) {
        calc_hash(filepath);
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
