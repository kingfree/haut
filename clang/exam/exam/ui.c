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

static char *NAME = "标准化考试系统";
static char *VERSION = "0.0.8";

static char *problem_db_name = "problem.db";

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
      case 0: exit(0); break;
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
      case 0: exit(0); break;
      default: return; break;
    }
  }
}

void *ui_each_problem_show(SList *item, void *userdata)
{
  Problem *p = (Problem *) item->userdata;
  // fprintf(stderr, "(%p)->%p\n", item, p);
  printf("%d. %s\n"
    "    [A]. %s\t[B]. %s\t[C]. %s\t[D]. %s\n"
    "", p->id, p->des,
    p->opt[0], p->opt[1], p->opt[2], p->opt[3]);
  printf("\t 答案: %c\t 难度: %-10s\t 标签: %i\t 章节: %i.%i\n",
    p->ans, dif2star(p->dif), p->tag, p->chapter, p->section);
  return NULL;
}

void ui_teacher_view()
{
  PList *db = plist_new();
  int n;
  if ((n = problem_read_file(db, problem_db_name)) < 0) {
    perror("读取题目数据库失败");
    return;
  }
  printf("数据库中有 %d 条记录，最大编号为 %d\n", db->count, db->max_id);
  slist_foreach(db->slist, ui_each_problem_show, NULL);
  pause();
}

char ui_input_ans()
{
  char c = ' ';
  while (scanf("%c", &c), c = toupper(c), !('A' <= c && c <= 'D'));
  return c;
}

Problem *ui_input_problem()
{
  int i = 0;
  Problem *p = malloc(sizeof(Problem));

  printf("请输入题目相关信息: \n");

  printf("题目描述: ");
  scanf("%s", p->des);

  for (i = 'A'; i <= 'D'; i++) {
    printf("[选项 %c]: ", i);
    scanf("%s", p->opt[i - 'A']);
  }

  printf("正确答案字母序号: ");
  p->ans = ui_input_ans();

  printf("题目难度(1--10): ");
  while(scanf("%hi", &p->dif) != 1);

  printf("知识点标签(数字编号): ");
  while(scanf("%hi", &p->tag) != 1);

  printf("知识点章节(章.节): ");
  while(scanf("%hi.%hi", &p->chapter, &p->section) != 2);

  return p;
}

void ui_output_problem(Problem *p, bool show_more)
{
  printf("%d. %s\n"
    "    [A]. %s\n"
    "    [B]. %s\n"
    "    [C]. %s\n"
    "    [D]. %s\n"
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
  PList *db = plist_new();
  int n;
  if ((n = problem_read_file(db, problem_db_name)) < 0) {
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
  if (problem_write_file(db, problem_db_name) < 0) {
    perror("写入题目数据库失败");
    return;
  }
  pause();
}
