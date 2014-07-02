/* problem.h
 * 题目数据库的增删改查
 */

#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include "db.h"

typedef struct problem {
  int id;
  char des[256];   // 题目描述
  char opt[4][64]; // 选项
  char ans;        // 答案
  char dif;        // 难度系数
  int cate_id;
} problem;

const char *problem_db_name = "problem.db";

FILE * problem_open();
int problem_close(FILE *db);

int problem_insert(FILE *db, problem *p); // by id
int problem_delete(FILE *db, problem *p); // by id
int problem_update(FILE *db, problem *p); // by id
int problem_select(FILE *db, problem *p); // by id

int problem_count(FILE *db);
int problem_max_id(FILE *db);

list * problem_search(FILE *db, char *search);

int problem_max_id(FILE *db);

#endif
