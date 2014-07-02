/* problem.h
 * 题目数据库的增删改查
 */

#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include "db.h"

typedef struct problem {
  int id;
  char des[256];
  char opt[4][128];
  char ans;
} problem;

const char *problem_db_name = "problem.db";

FILE * problem_open();
int problem_insert(FILE *db, problem p);
int problem_close(FILE *db);

#endif
