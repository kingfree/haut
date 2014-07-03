/* ui.h
 * 终端用户屏幕交互
 */

#ifndef _UI_H_
#define _UI_H_

#include "slist.h"
#include "problem.h"

#define NAME    "标准化考试系统"
#define VERSION "0.0.4"

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

Problem ui_input_problem();

#endif
