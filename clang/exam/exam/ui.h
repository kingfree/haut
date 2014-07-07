/* ui.h
 * 终端用户屏幕交互
 */

#ifndef _UI_H_
#define _UI_H_

#include <stdbool.h>

#include "slist.h"
#include "problem.h"
#include "paper.h"
#include "user.h"

void cls();
void pause();
char *dif2star(int dif);

void ui_index();
int ui_do_login(User *u);
void ui_student_login();
void ui_teacher_login();

void ui_student(User *u);
void ui_teacher();
void ui_help();
void ui_about();

void ui_student_test(User *u);
void ui_student_score(User *u);

void ui_teacher_view();
void ui_teacher_insert();
void ui_teacher_delete();
void ui_teacher_update();
void ui_teacher_select();
void ui_teacher_generate();
void ui_teacher_score();

Problem *ui_select_id(List *list);
int ui_select_des(List *list);
int ui_select_opt(List *list);
int ui_select_dif(List *list);
int ui_select_tag(List *list);
int ui_select_sec(List *list);
int ui_select_mul(List *list);
int ui_select_output(List *list, SListCallback *find, void *matchdata);

Problem *ui_input_problem();
void ui_edit_problem(Problem *p);
void ui_output_problem(Problem *p, bool show_more);
void *ui_each_problem_show(SList *item, void *userdata);
void ui_output_count(List *list);

int ui_input_number();
char ui_input_ans();

void ui_paper_list();
int ui_paper_save(Paper *pa);

void ui_generate_random(List *list);
void ui_generate_tags(List *list);
void ui_generate_secs(List *list);
void ui_generate_dif(List *list);

#endif
