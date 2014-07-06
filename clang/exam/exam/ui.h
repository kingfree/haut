/* ui.h
 * 终端用户屏幕交互
 */

#ifndef _UI_H_
#define _UI_H_

#include <stdbool.h>

#include "slist.h"
#include "problem.h"
#include "paper.h"

void cls();
void pause();
char *dif2star(int dif);

void ui_login();

void ui_student();
void ui_teacher();
void ui_help();
void ui_about();

void ui_student_test();
void ui_student_score();

void ui_teacher_view();
void ui_teacher_insert();
void ui_teacher_delete();
void ui_teacher_update();
void ui_teacher_select();
void ui_teacher_generate();
void ui_teacher_score();

Problem *ui_select_id(PList *db);
int ui_select_des(PList *db);
int ui_select_opt(PList *db);
int ui_select_dif(PList *db);
int ui_select_tag(PList *db);
int ui_select_sec(PList *db);
int ui_select_mul(PList *db);
int ui_select_output(PList *db, SListCallback *find, void *matchdata);

Problem *ui_input_problem();
void ui_edit_problem(Problem *p);
void ui_output_problem(Problem *p, bool show_more);
void *ui_each_problem_show(SList *item, void *userdata);
void ui_output_count(PList *db);

int ui_input_number();
char ui_input_ans();

void ui_paper_list();
int ui_paper_save(Paper *pa);

void ui_generate_random(PList *db);
void ui_generate_tags(PList *db);
void ui_generate_secs(PList *db);
void ui_generate_dif(PList *db);
void ui_generate_custom(PList *db);

#endif
