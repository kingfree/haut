/* user.h
 * 用户类（学生和教师数据）
 */

#ifndef _USER_H_
#define _USER_H_

#include <stdbool.h>

typedef struct User {
    int id;
    char username[32];
    char passwd[64];
    bool teacher;
} User;

User *user_new();
void user_free(User *u);

int user_reg(User *u);
int user_login(User *u);

#endif
