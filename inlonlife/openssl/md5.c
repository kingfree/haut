#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/queue.h>
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
        fprintf(stderr, "'%s' ", filename);
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

int print_entry(const char *file, const struct stat *st,
                const int type, struct FTW *path)
{
    if (type == FTW_F) {
        calc_hash(file);
    }

    return 0;
}

void do_nftw(const char *const dirpath)
{
    nftw(dirpath, print_entry, USE_FDS, FTW_PHYS);
}

void do_recuerse(const char *const dirpath)
{
    struct dirent *ent = NULL;
    DIR *dir;
    char path[BUFFSIZE];

    if ((dir = opendir(dirpath)) == NULL) return;

    while ((ent = readdir(dir)) != NULL) {
        snprintf(path, BUFFSIZE, "%s/%s", dirpath, ent->d_name);
        if (ent->d_type & DT_REG) {
            calc_hash(path);
        } else if (ent->d_type & DT_DIR) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                do_recuerse(path);
            }
        }
    }
    closedir(dir);
}

void do_stack(const char *const dirpath)
{
    struct dirent *ent = NULL;
    DIR *dir;
    char path[BUFFSIZE];

    SLIST_HEAD(slisthead, entry) head = SLIST_HEAD_INITIALIZER(head);
    struct entry {
        SLIST_ENTRY(entry) entries;
        char *dirname;
    } *p;

    SLIST_INIT(&head);

    p = malloc(sizeof(struct entry));
    p->dirname = strdup(dirpath);
    SLIST_INSERT_HEAD(&head, p, entries);

    while (!SLIST_EMPTY(&head)) {
        p = SLIST_FIRST(&head);
        fprintf(stderr, "{%s}\n", p->dirname);

        if ((dir = opendir(p->dirname)) == NULL) {
            perror("目录打开失败");
            goto end;
        }

        while ((ent = readdir(dir)) != NULL) {
            snprintf(path, BUFFSIZE, "%s/%s", p->dirname, ent->d_name);
            fprintf(stderr, "[%s]\n", path);
            if (ent->d_type & DT_REG) {
                calc_hash(path);
            } else if (ent->d_type & DT_DIR) {
                if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                    p = malloc(sizeof(struct entry));
                    p->dirname = strdup(path);
                    SLIST_INSERT_HEAD(&head, p, entries);
                }
            }
        }
        closedir(dir);

end:
        free(p->dirname);
        SLIST_REMOVE(&head, p, entry, entries);
        free(p);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) argv[argc++] = ".";

    if (argc > 2 && strcmp(argv[2], "-r") == 0) {
        printf("递归处理:  %s\n", argv[1]);
        do_recuerse(argv[1]);
    } else if (argc > 2 && strcmp(argv[2], "-s") == 0) {
        printf("非递归处理: %s\n", argv[1]);
        do_stack(argv[1]);
    } else {
        printf("调用函数: %s\n", argv[1]);
        do_nftw(argv[1]);
    }

    return 0;
}
