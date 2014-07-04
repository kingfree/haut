/* problem.c
 * 题目数据库的增删改查
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "algorithm.h"
#include "file.h"
#include "problem.h"

extern char *problem_db_name;

Problem *problem_new()
{
  Problem *p = (Problem *) malloc(sizeof(Problem));
  p->id = 0;
  p->ans = 'A';
  p->dif = 0;
  p->tag = 0;
  p->chapter = 0;
  p->section = 0;
  return p;
}

PList *plist_new()
{
  PList *pl = (PList *) malloc(sizeof(PList));
  pl->slist = NULL;
  pl->max_id = 0;
  pl->count = 0;
  return pl;
}

int cmp_id(const SList *item1, const SList *item2, void *max_id)
{
  Problem *a = (Problem *) item1->userdata;
  Problem *b = (Problem *) item2->userdata;
  // fprintf(stderr, "%p: %i (%i)\n", max_id, max_id, *max_id);
  // int c = max((int) *max_id, max(a->id, b->id));
  // *max_id = c;
  // fprintf(stderr, "[%d %d]\n", a->id, b->id);
  return (a->id - b->id);
}

int problem_read_list(PList *db)
/* 返回成功读取的条目数 */
{
  FILE *file = fopen(problem_db_name, "rb");
  if (file == NULL) {
    if ((file = fopen(problem_db_name, "wb")) == NULL) {
      return -1;
    }
    return 0;
  }
  int n = file_data_count(file, sizeof(Problem));
  rewind(file);
  Problem *p;
  int i = 0;
  for (i = 0; i < n; i++) {
    // fseek(file, sizeof(Problem) * i, SEEK_SET);
    p = problem_new();
    if (fread(p, sizeof(Problem), 1, file) != 1) {
      goto end;
    }
    // fprintf(stderr, "pid %d %s (%d)\n", p->id, p->des, ftell(file));
    if (problem_insert(db, p) < 0) {
      goto end;
    }
  }
end:
  fclose(file);
  problem_restore(db);
  return db->count;
}

void problem_restore(PList *db)
{
  db->slist = slist_sort(db->slist, cmp_id, &db->max_id);
  db->count = slist_length(db->slist);
  if (db->count <= 0) {
    db->max_id = 0;
  } else {
    SList *s = slist_nth(db->slist, db->count);
    Problem *p = (Problem *) s->userdata;
    db->max_id = p->id;
  }
}

void *problem_write(SList *item, void *file)
{
  Problem *p = (Problem *) item->userdata;
  // fprintf(stderr, "pid %d %s (%d)\n", p->id, p->des, ftell(file));
  fwrite(p, sizeof(Problem), 1, file);
  return NULL;
}

int problem_write_list(PList *db)
{
  FILE *file = fopen(problem_db_name, "wb");
  if (file == NULL) {
      return -1;
  }
  rewind(file);
  slist_foreach(db->slist, problem_write, file);
  fclose(file);
  return 0;
}

int problem_insert(PList *db, Problem *p)
{
  db->slist = slist_cons(slist_box(p), db->slist);
  db->count++;
  db->max_id++;
  return 0;
}

char *dif2star(int dif) {
  char *s = calloc(16, sizeof(char));
  char c[8][4] = {""};
  int i;
  for (i = 0; i < dif / 2; i++)
    strcpy(c[i], "★");
  if ((i = dif % 2) == 1)
    strcpy(c[dif / 2 + 1], "☆");
  sprintf(s, "%s%s%s%s%s", c[0], c[1], c[2], c[3], c[4]);
  return s;
}
