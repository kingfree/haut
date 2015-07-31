#include <stdio.h>
#include <openssl/md5.h>

int main(int argc, char *argv[])
{
    unsigned char c[MD5_DIGEST_LENGTH];
    char *filename = "md5.c";
    if (argc > 1) filename = argv[1];

    FILE *inFile = fopen(filename, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    if (inFile == NULL) {
        printf("%s can't be opened.\n", filename);
        return 0;
    }

    MD5_Init(&mdContext);
    while ((bytes = fread(data, 1, 1024, inFile)) != 0)
        MD5_Update(&mdContext, data, bytes);
    MD5_Final(c, &mdContext);

    int i;
    for (i = 0; i < MD5_DIGEST_LENGTH; i++) printf("%02x", c[i]);
    printf(" %s\n", filename);

    fclose(inFile);

    return 0;
}
