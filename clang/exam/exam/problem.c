/* problem.c
 * 题目数据库的增删改查
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "problem.h"

PList *problem_read_list()
{
  FILE *file = fopen(problem_db_name, "r+");
  if (file == NULL) {
    return NULL;
  }
  Problem p;
}

int problem_write_list(PList *db)
{
  return db_close(db);
}

int problem_insert(PList *db, problem *p)
{
  p->id = problem_max_id(db) + 1;
  return db_insert(db, p);
}
