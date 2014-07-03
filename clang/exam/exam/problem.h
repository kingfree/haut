/* problem.h
 * 题目数据库的增删改查
 */

#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include "slist.h"

typedef struct Problem {
  int  id;
  char des[256];   // 题目描述
  char opt[4][64]; // 选项
  char ans;        // 答案
  short dif;       // 难度系数
  short  tag;      // 标签（知识点）
  short  chapter;  // 章
  short  section;  // 节
} Problem;

typedef struct PList {
    SList *slist;
    int max_id;
    int count;
} PList;

PList *plist_new();

static char *problem_db_name = "problem.db";

int problem_read_list(PList *db);
int problem_write_list(PList *db);

int problem_insert(PList *db, Problem *p); // by id
int problem_delete(PList *db, Problem *p); // by id
int problem_update(PList *db, Problem *p); // by id
int problem_select(PList *db, Problem *p); // by id

int problem_count(PList *db);
int problem_max_id(PList *db);

PList *problem_search(PList *db, char *search);

int problem_max_id(PList *db);

char *dif2star(int dif);

#endif
