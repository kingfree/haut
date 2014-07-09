/* problem.h
 * 题目数据库的增删改查
 */

#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include "slist.h"
#include "list.h"

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

Problem *problem_new();

int problem_read_file(List *list);
int problem_write_file(List *list);

void *by_id(SList *item, void *data);
void *by_des(SList *item, void *data);
void *by_opt(SList *item, void *data);
void *by_difr(SList *item, void *data);
void *by_difs(SList *item, void *data);
void *by_tags(SList *item, void *data);
void *by_secs(SList *item, void *data);
void *by_mul(SList *item, void *data);

#endif
