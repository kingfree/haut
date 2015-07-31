#include <stdio.h>

#if defined(__APPLE__)
#  define COMMON_DIGEST_FOR_OPENSSL
#  include <CommonCrypto/CommonDigest.h>
#else
#  include <openssl/md5.h>
#endif

#define BUFFSIZE 1024

int main(int argc, char *argv[])
{
    unsigned char c[MD5_DIGEST_LENGTH];
    char *filename = "md5.c";
    if (argc > 1) filename = argv[1];

    FILE *file = fopen(filename, "rb");
    MD5_CTX md5;
    int bytes;
    unsigned char data[BUFFSIZE];

    if (file == NULL) {
        printf("%s can't be opened.\n", filename);
        return 0;
    }

    MD5_Init(&md5);
    while ((bytes = fread(data, 1, BUFFSIZE, file)) != 0)
        MD5_Update(&md5, data, bytes);
    MD5_Final(c, &md5);

    int i;
    for (i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
    printf(" %s\n", filename);

    fclose(file);

    return 0;
}
