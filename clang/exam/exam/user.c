/* user.c
* 用户类（学生和教师数据）
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>

#include "file.h"
#include "user.h"

static char *user_list_name = "user.list";

User *user_new()
{
    User *u = (User *)malloc(sizeof(User));
    assert(u);
    u->id = 0;
    memset(u->username, 0, sizeof(u->username));
    memset(u->passwd, 0, sizeof(u->passwd));
    u->teacher = false;
    return u;
}

void user_free(User *u)
{
    free(u);
}

int user_init()
{
    User u = {
        .id = 1,
        .username = "root",
        .passwd = "root",
        .teacher = true
    };
    if (user_reg(&u) < 0) {
        return -1;
    }
    return 0;
}

int user_reg(User *u)
{
    FILE *file = fopen(user_list_name, "ab");
    if (file == NULL) {
        return -1;
    }
    int n = file_data_count(file, sizeof(User));
    u->id = n + 1;
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return -1;
    }
    if (fwrite(u, sizeof(User), 1, file) != 1) {
        fclose(file);
        return -1;
    }
    if (fclose(file) != 0) {
        return -1;
    }
    fprintf(stderr, "%d %s %s %d\n", u->id, u->username, u->passwd, u->teacher);
    return u->id;
}

int user_login(User *u)
{
    FILE *file = fopen(user_list_name, "rb");
    if (file == NULL) {
        if (user_init() < 0) {
            return -1;
        }
        file = fopen(user_list_name, "rb");
        if (file == NULL) {
            return -1;
        }
    } else if (file_data_count(file, sizeof(User)) < 1) {
        fclose(file);
        if (user_init() < 0) {
            return -1;
        }
    }
    User v;
    rewind(file);
    while (!feof(file)) {
        fread(&v, sizeof(v), 1, file);
        //fprintf(stderr, "%d %s %s %d\n", v.id, v.username, v.passwd, v.teacher);
        if (strcmp(v.username, u->username) == 0) {
            if (strcmp(v.passwd, u->passwd) == 0) {
                u->id = v.id;
                u->teacher = v.teacher;
                goto end;
            }
            errno = EACCES;
            return -2;
        }
    }
end:
    if (fclose(file) == EOF) {
        return -1;
    }
    return u->id;
}
