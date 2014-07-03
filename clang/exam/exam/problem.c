/* problem.c
 * 题目数据库的增删改查
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "file.h"
#include "problem.h"

int problem_read_list(PList *db)
/* 返回成功读取的条目数 */
{
  db = malloc(sizeof(PList));
  FILE *file = fopen(problem_db_name, "rb+");
  if (file == NULL) {
    if ((file = fopen(problem_db_name, "wb+")) == NULL) {
      return -1;
    }
    return 0;
  }
  int n = file_data_count(file, sizeof(Problem));
  fprintf(stderr, "%d\n", n);
  rewind(file);
  Problem *p = malloc(sizeof(Problem));
  int i = 0;
  for (i = 0; i < n; i++) {
    if (fread(p, sizeof(Problem), 1, file) != 1) {
      goto end;
    }
    if (problem_insert(db, p) < 0) {
      goto end;
    }
  }
end:
  fclose(file);
  db = slist_reverse(db);
  return i;
}

int problem_write_list(PList *db)
{
  return 0;
}

int problem_insert(PList *db, Problem *p)
{
  db = slist_cons(slist_box(p), db);
  return 0;
}
