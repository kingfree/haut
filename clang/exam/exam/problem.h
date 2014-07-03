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
  char dif;        // 难度系数
  int  tag;        // 标签（知识点）
  int  chapter;    // 章
  int  section;    // 节
} Problem;

typedef SList PList;

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

#endif
