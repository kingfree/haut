/* ui.h
 * 终端用户屏幕交互
 */

#ifndef _UI_H_
#define _UI_H_

#include <stdbool.h>

#include "slist.h"
#include "problem.h"

void ui_login();

void ui_student();
void ui_teacher();
void ui_help();
void ui_about();

void ui_teacher_view();
void ui_teacher_insert();
void ui_teacher_delete();
void ui_teacher_update();
void ui_teacher_select();
void ui_teacher_generate();
void ui_teacher_score();

Problem *ui_select_id(PList *db);
void ui_edit_problem(Problem *p);

Problem *ui_input_problem();
void ui_output_problem(Problem *p, bool show_more);

void ui_output_count(PList *db);

char *dif2star(int dif);

#endif
