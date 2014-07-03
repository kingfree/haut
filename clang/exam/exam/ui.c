/* ui.c
 * 终端用户屏幕交互
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>

#include "slist.h"
#include "problem.h"
#include "ui.h"

void cls() {
#ifdef WIN32
  system("cls");
#else
  system("clear");
#endif
}

void pause() {
#ifdef WIN32
  system("pause");
#else
  fprintf(stderr, "键入回车以继续...\n");
  getchar();
#endif
}

int input_option(char *menu)
{
  cls();
  printf("欢迎使用 %s v%s\n"
    "%s", NAME, VERSION, menu);
  printf("请输入你的选项: ");
  int x;
  scanf("%i", &x);
  getchar();
  return x;
}

void ui_login()
{
  while (true) {
    switch (input_option(
      "   1 - 以学生身份登录\n"
      "   2 - 以教师身份登录\n"
      "   3 - 使用帮助\n"
      "   4 - 关于\n"
      "   0 - 退出系统\n"
    )) {
      // case 1: ui_student(); break;
      case 2: ui_teacher(); break;
      // case 3: ui_help(); break;
      // case 4: ui_about(); break;
      default: return; break;
    }
  }
}

void ui_teacher()
{
  while (true) {
    switch (input_option(
      "   1 - 浏览试题\n"
      "   2 - 添加试题（增）\n"
      "   3 - 删除试题（删）\n"
      "   4 - 修改试题（改）\n"
      "   5 - 查询试题（查）\n"
      "   6 - 智能组卷\n"
      "   7 - 成绩分析\n"
      "   9 - 返回上一级\n"
      "   0 - 退出系统\n"
    )) {
      case 1: ui_teacher_view(); break;
      case 2: ui_teacher_insert(); break;
      // case 3: ui_teacher_delete(); break;
      // case 4: ui_teacher_update(); break;
      // case 5: ui_teacher_select(); break;
      // case 6: ui_teacher_generate(); break;
      // case 7: ui_teacher_score(); break;
      case 9: return; break;
      default: exit(0); break;
    }
  }
}

void *ui_each_problem_show(SList *item, void *userdata)
{
  Problem *p = item->userdata;
  // fprintf(stderr, "(%p)->%p\n", item, p);
  printf("%3d. %s\n"
    "     [A]. %s\t[B]. %s\t[C]. %s\t[D]. %s\n"
    "", p->id, p->des,
    p->opt[0], p->opt[1], p->opt[2], p->opt[3]);
  printf("  答案: %c\t难度: %10s\t标签: %i\t章节: %i.%i\n",
    p->ans, dif2star(p->dif), p->tag, p->chapter, p->section);
  return NULL;
}

void ui_teacher_view()
{
  PList *db, dbc;
  int n;
  if ((n = problem_read_list(db = &dbc)) < 0) {
    perror("读取题目数据库失败");
    return;
  }
  printf("数据库中有 %d 条记录，最大编号为 %d\n", db->count, db->max_id);
  slist_foreach(db->slist, ui_each_problem_show, NULL);
  pause();
}

Problem *ui_input_problem()
{
  // int  id;
  // char des[256];   // 题目描述
  // char opt[4][64]; // 选项
  // char ans;        // 答案
  // char dif;        // 难度系数
  // int  tag;        // 标签（知识点）
  // int  chapter;    // 章
  // int  section;    // 节
  int i = 0;
  Problem *p = malloc(sizeof(Problem));

  printf("题目描述: ");
  scanf("%s", p->des);

  for (i = 'A'; i <= 'D'; i++) {
    printf("[选项 %c]: ", i);
    scanf("%s", p->opt[i - 'A']);
  }

  printf("正确答案字母序号: ");
  while (scanf("%c", &p->ans), p->ans = toupper(p->ans), !('A' <= p->ans && p->ans <= 'D'));

  printf("题目难度(1--10): ");
  scanf("%hi", &p->dif);

  printf("知识点标签(数字编号): ");
  scanf("%hi", &p->tag);

  printf("知识点章节(章.节): ");
  scanf("%hi.%hi", &p->chapter, &p->section);

  return p;
}

void ui_output_problem(Problem *p, bool show_more)
{
  printf("%3d. %s\n"
    "[A]. %s\n"
    "[B]. %s\n"
    "[C]. %s\n"
    "[D]. %s\n"
    "", p->id, p->des,
    p->opt[0], p->opt[1], p->opt[2], p->opt[3]);
  if (show_more) {
    // printf("%p\n", p);
    printf("答案: %c\t", p->ans);
    printf("难度: %s\t", dif2star(p->dif));
    printf("标签: %i\t", p->tag);
    printf("章节: %i.%i\n", p->chapter, p->section);
  }
}

void ui_teacher_insert()
{
  PList *db, dbc;
  int n;
  if ((n = problem_read_list(db = &dbc)) < 0) {
    perror("读取题目数据库失败");
    return;
  }
  printf("数据库中有 %d 条记录，最大编号为 %d\n", db->count, db->max_id);
  Problem *p = ui_input_problem();
  p->id = db->max_id + 1;
  ui_output_problem(p, true);
  if (problem_insert(db, p) < 0) {
    perror("插入题目失败");
    return;
  }
  if (problem_write_list(db) < 0) {
    perror("写入题目数据库失败");
    return;
  }
  pause();
}
