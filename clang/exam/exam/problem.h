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

Problem *problem_new();
void problem_free(Problem *p);
PList *plist_new();
void problem_slist_free(void *s);
void plist_free(PList *db);

int problem_read_file(PList *db, const char *filename);
int problem_write_file(PList *db, const char *filename);

void problem_restore(PList *db);

int problem_insert(PList *db, Problem *p);

int problem_count(PList *db);
int problem_max_id(PList *db);

PList *problem_search(PList *db, char *search);

void *by_id(SList *item, void *data);
void *by_des(SList *item, void *data);
void *by_opt(SList *item, void *data);
void *by_dif(SList *item, void *data);
void *by_tag(SList *item, void *data);
void *by_sec(SList *item, void *data);
void *by_mul(SList *item, void *data);

#endif
