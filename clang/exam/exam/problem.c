/* problem.c
 * 题目数据库的增删改查
 */

#include <errno.h>

#include "problem.h"

FILE * problem_open()
{
  return db_open(problem_db_name, sizeof(problem));
}

int problem_close(FILE *db)
{
  return db_close(db);
}

int problem_insert(FILE *db, problem *p)
{
  p->id = problem_max_id(db) + 1;
  return db_insert(db, p);
}
