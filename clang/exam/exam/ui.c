/* ui.c
 * 终端用户屏幕交互
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "db.h"
#include "problem.h"
#include "ui.h"

void cls() {
#ifdef WIN32
  system("cls");
#else
  system("clear");
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
      "   0 - 返回上一级\n"
    )) {
      // case 1: ui_teacher_view(); break;
      case 2: ui_teacher_insert(); break;
      // case 3: ui_teacher_delete(); break;
      // case 4: ui_teacher_update(); break;
      // case 5: ui_teacher_select(); break;
      // case 6: ui_teacher_generate(); break;
      // case 7: ui_teacher_score(); break;
      default: return; break;
    }
  }
}

void ui_teacher_insert()
{
  problem *p = ui_input_problem();
  FILE *db = problem_open();
  if (db == NULL) {
    perror("打开题目数据库失败");
    return;
  }
  if (problem_insert(db, p) != 0) {
    perror("插入题目失败");
    return;
  }
  if (problem_close(db) != 0) {
    perror("关闭题目数据库失败");
    return;
  }
}
