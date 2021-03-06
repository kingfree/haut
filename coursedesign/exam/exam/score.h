﻿/* score.h
 * 成绩类
 */

#ifndef _SCORE_H_
#define _SCORE_H_

#include <time.h>

#include "user.h"
#include "paper.h"

typedef struct Score {
    int id;
    int user_id;
    char username[128];
    int paper_id;
    int paper_count;
    int right, now;
    time_t date;
    char answer[256];
} Score;

Score *score_new(User *u, Paper *p);

int score_read_list(List *list);
int score_write_file(List *list);

int score_did(Score *s, char c, char ans);

void *by_user_id(SList *item, void *data);

#endif
