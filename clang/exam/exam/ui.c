/* ui.c
 * 终端用户屏幕交互
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>

#include "addon.h"
#include "slist.h"
#include "problem.h"
#include "paper.h"
#include "user.h"
#include "ui.h"

static char *NAME = "标准化考试系统";
static char *VERSION = "0.3.6";

void cls()
{
#ifdef WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
#ifdef WIN32
    system("pause");
#else
    system("read -p \"请按任意键继续. . .\"");
#endif
}

char *dif2star(int dif)
{
    assert(0 <= dif && dif <= 10);
    char *s = calloc(16, sizeof(char));
    assert(s);
    char c[8][4] = { "" };
    int i;
    for (i = 0; i < dif / 2; i++)
        strcpy(c[i], "★");
    if ((i = dif % 2) == 1)
        strcpy(c[dif / 2 + 1], "☆");
    sprintf(s, "%s%s%s%s%s", c[0], c[1], c[2], c[3], c[4]);
    return s;
}

int ui_input_option(char *menu, bool cl)
{
    if (cl) {
        cls();
        printf("欢迎使用 %s v%s\n", NAME, VERSION);
    }
    printf("%s$ ", menu);
    int x;
    scanf("%i", &x);
    getchar();
    return x;
}

void ui_index()
{
    while (true) {
        switch (ui_input_option(
            "   1 - 学生登录\n"
            "   2 - 教师登录\n"
            "   3 - 帮助\n"
            "   4 - 关于\n"
            "   0 - 退出系统\n"
            , true)) {
        case 1: ui_student_login(); break;
        case 2: ui_teacher_login(); break;
        case 3: ui_help(); break;
        case 4: ui_about(); break;
        case 0: exit(0); break;
        default: return; break;
        }
    }
}

void ui_help()
{
#ifdef WIN32
    system("..\\exam.pdf");
#else
    printf("请查阅课程报告文档 exam.pdf \n");
    pause();
#endif
}

void ui_about()
{
    cls();
    printf("%s v%s \n"
        "开源的标准化考试系统（命令行版） \n"
        "\n"
        "版权所有 (c) 2014 田劲锋  保留所有权利 \n"
        "遵循 MIT 许可协议，允许有限制的自由使用 \n"
        "\n", NAME, VERSION);
    pause();
}

void ui_student_login()
{
    User *u = user_new();
    int res = 0;
    if ((res = ui_do_login(u)) > 0) {
        ui_student(u);
    } else {
        perror("以学生身份登录失败");
        pause();
        if (res != -2) {
            printf("自动建立新账号 %s...\n", u->username);
            if (user_reg(u) < 0) {
                perror("建立新用户失败");
                goto end;
            }
            printf("以建立的新用户 %s 登录。\n", u->username);
            pause();
            ui_student(u);
        }
    }
end:
    user_free(u);
}

void ui_student(User *u)
{
    while (true) {
        switch (ui_input_option(
            "   1 - 做卷子\n"
            "   2 - 看成绩\n"
            "   9 - 返回上一级\n"
            "   0 - 退出系统\n"
            , true)) {
        case 1: ui_student_test(u); break;
        case 2: ui_student_score(u); break;
        case 0: exit(0); break;
        default: return; break;
        }
    }
}

void ui_student_test(User *u)
{}

void ui_student_score(User *u)
{}

int ui_do_login(User *u)
{
    printf("用户名: ");
    scanf("%s", u->username);
	char *s = getpass("密码: ");
    strncpy(u->passwd, s, 64);
    return user_login(u);
}

void ui_teacher_login()
{
    User *u = user_new();
    if (ui_do_login(u) > 0 && u->teacher == true) {
        user_free(u);
        ui_teacher();
    } else {
        user_free(u);
        perror("以教师身份登录失败");
        pause();
    }
}

void ui_teacher()
{
    while (true) {
        switch (ui_input_option(
            "   1 - 浏览试题\n"
            "   2 - 添加试题（增）\n"
            "   3 - 删除试题（删）\n"
            "   4 - 修改试题（改）\n"
            "   5 - 查询试题（查）\n"
            "   6 - 智能组卷\n"
            "   7 - 成绩分析\n"
            "   9 - 返回上一级\n"
            "   0 - 退出系统\n"
            , true)) {
        case 1: ui_teacher_view(); break;
        case 2: ui_teacher_insert(); break;
        case 3: ui_teacher_delete(); break;
        case 4: ui_teacher_update(); break;
        case 5: ui_teacher_select(); break;
        case 6: ui_teacher_generate(); break;
        case 7: ui_teacher_score(); break;
        case 0: exit(0); break;
        default: return; break;
        }
    }
}

void ui_teacher_view()
{
    List *list = list_new();
    if (problem_read_file(list) < 0) {
        perror("读取题目数据库失败");
        return;
    }
    ui_output_count(list);
    list_each_call(list, ui_each_problem_show, NULL);
    list_free(list);
    pause();
}

void ui_teacher_insert()
{
    List *list = list_new();
    if (problem_read_file(list) < 0) {
        perror("读取题目数据库失败");
        goto end;
    }
    ui_output_count(list);
    Problem *p = ui_input_problem();
    p->id = list->max_id + 1;
    int id = p->id;
    ui_output_problem(p, true);
    if (list_insert(list, p) < 0) {
        perror("插入题目失败");
        goto end;
    }
    if (problem_write_file(list) < 0) {
        perror("写入题目数据库失败");
        goto end;
    }
    printf("插入题目 %d 成功！\n", id);
end:
    list_free(list);
    pause();
}

bool ui_get_yes(char *s)
{
    printf("%s输入'Y'或'y'以确认:\n$ ", s);
    gotn();
    char y = getchar();
    if (y == 'Y' || y == 'y') {
        return true;
    }
    return false;
}

void ui_teacher_delete()
{
    List *list = list_new();
    int n;
    if ((n = problem_read_file(list)) < 0) {
        perror("读取题目数据库失败");
        goto end;
    }
    ui_output_count(list);
    Problem *p = ui_select_id(list);
    if (p == NULL || p->id < 0) {
        perror("获取题目失败");
        goto end;
    }
    int id = p->id;
    if (!ui_get_yes("你确定要删除该题吗？")) {
        goto end;
    }
    if (list_remove(list, by_id, &id) != p) {
        perror("删除题目失败");
        goto end;
    }
    free(p);
    if (problem_write_file(list) < 0) {
        perror("写入题目数据库失败");
        goto end;
    }
    printf("删除题目 %d 成功！ \n", id);
end:
    list_free(list);
    pause();
}

void ui_teacher_update()
{
    List *list = list_new();
    int n;
    if ((n = problem_read_file(list)) < 0) {
        perror("读取题目数据库失败");
        goto end;
    }
    ui_output_count(list);
    Problem *p = ui_select_id(list);
    if (p == NULL || p->id < 0) {
        perror("获取题目失败");
        goto end;
    }
    int id = p->id;
    ui_edit_problem(p);
    ui_output_problem(p, true);
    if (problem_write_file(list) < 0) {
        perror("写入题目数据库失败");
        goto end;
    }
    printf("修改题目 %d 成功！ \n", id);
end:
    list_free(list);
    pause();
}

void ui_teacher_select()
{
    List *list = list_new();
    int n;
    if ((n = problem_read_file(list)) < 0) {
        perror("读取题目数据库失败");
        return;
    }
    ui_output_count(list);
    while (true) {
        cls();
        switch (ui_input_option(
            "可以查询的关键字:\n"
            "   1 - 题目编号\n"
            "   2 - 题目描述\n"
            "   3 - 题目选项\n"
            "   4 - 题目难度\n"
            "   5 - 题目标签\n"
            "   6 - 题目章节\n"
            "   7 - 模糊查询\n"
            "   9 - 返回上一级\n"
            "   0 - 退出系统\n"
            , false)) {
        case 1: ui_select_id(list); break;
        case 2: ui_select_des(list); break;
        case 3: ui_select_opt(list); break;
        case 4: ui_select_dif(list); break;
        case 5: ui_select_tag(list); break;
        case 6: ui_select_sec(list); break;
        case 7: ui_select_mul(list); break;
        case 0: exit(0); break;
        default: return; break;
        }
        pause();
    }
    list_free(list);
}

void ui_teacher_generate()
{
    List *list = list_new();
    if (problem_read_file(list) < 0) {
        perror("读取题目数据库失败");
        return;
    }
    while (true) {
        cls();
        switch (ui_input_option(
            "选择智能组卷算法:\n"
            "   1 - 随机生成\n"
            "   2 - 按标签生成\n"
            "   3 - 按章节生成\n"
            "   4 - 按难度生成\n"
            "   5 - 试卷列表\n"
            "   6 - 查看试卷\n"
            "   9 - 返回上一级\n"
            "   0 - 退出系统\n"
            , false)) {
        case 1: ui_generate_random(list); break;
        case 2: ui_generate_tags(list); break;
        case 3: ui_generate_secs(list); break;
        case 4: ui_generate_dif(list); break;
        case 5: ui_paper_list(); break;
        case 6: ui_paper_view(list); break;
        case 0: exit(0); break;
        default: return; break;
        }
        pause();
    }
    list_free(list);
}

void ui_generate_random(List *list)
{
    ui_output_count(list);
    printf("智能组卷 - 随机生成\n");
    printf("生成题目数:\n$ ");
    int n = ui_input_number();
    Paper *pa = paper_new();
    printf("试卷标题:\n$ ");
    scanf("%s", pa->title);
    paper_generate_random(pa, list, n);
    ui_paper_save(pa);
}

void ui_generate_tags(List *list)
{
    ui_output_count(list);
    printf("智能组卷 - 按知识点标签生成\n");
    int tags[64], i = 0;
    printf("题目标签编号（以 0 结尾）:\n$ ");
    for (; scanf("%d", &tags[i]), tags[i] > 0; i++);
    printf("生成题目数:\n$ ");
    int n = ui_input_number();
    Paper *pa = paper_new();
    printf("试卷标题:\n$ ");
    scanf("%s", pa->title);
    paper_generate_tags(pa, list, n, tags, i);
    ui_paper_save(pa);
}

void ui_generate_secs(List *list)
{
    ui_output_count(list);
    printf("智能组卷 - 按知识点章节生成\n");
    double secs[64];
    int i = 0;
    printf("题目章节编号（小数点分割章节，节可省略，以 0 结尾 ）:\n$ ");
    for (; scanf("%lf", &secs[i]), secs[i] > 0; i++);
    printf("生成题目数:\n$ ");
    int n = ui_input_number();
    Paper *pa = paper_new();
    printf("试卷标题:\n$ ");
    scanf("%s", pa->title);
    paper_generate_secs(pa, list, n, secs, i);
    ui_paper_save(pa);
}

void ui_generate_dif(List *list)
{
    ui_output_count(list);
    printf("智能组卷 - 按难度区间生成\n");
    int a, b;
    printf("题目难度区间（两个 0--10 的数字）:\n$ ");
    a = ui_input_number();
    b = ui_input_number();
    printf("生成题目数:\n$ ");
    int n = ui_input_number();
    Paper *pa = paper_new();
    printf("试卷标题:\n$ ");
    scanf("%s", pa->title);
    paper_generate_dif(pa, list, n, a, b);
    ui_paper_save(pa);
}

void ui_teacher_score()
{}

void *ui_each_problem_show(SList *item, void *userdata)
{
    Problem *p = (Problem *)item->userdata;
    // fprintf(stderr, "(%p)->%p\n", item, p);
    printf("%d. %s\n"
        "    [A]. %s\t[B]. %s\t[C]. %s\t[D]. %s\n"
        "", p->id, p->des,
        p->opt[0], p->opt[1], p->opt[2], p->opt[3]);
    printf("\t 答案: %c\t 难度: %-10s\t 标签: %i\t 章节: %i.%i\n",
        p->ans, dif2star(p->dif), p->tag, p->chapter, p->section);
    return NULL;
}

void *ui_each_paper_show(SList *item, void *userdata)
{
    Paper *pa = (Paper *)item->userdata;
    fprint_paper_pid(pa, stdout);
    return NULL;
}

char ui_input_ans()
{
    char c = ' ';
    while (scanf("%c", &c), c = toupper(c), !('A' <= c && c <= 'D'));
    return c;
}

int ui_input_number()
{
    int n = 0;
    static char s[64];
    while (scanf("%s", s), !('0' <= s[0] && s[0] <= '9'));
    sscanf(s, "%d", &n);
    return n;
}

void ui_output_count(List *list)
{
    printf("数据库中有 %d 条记录，最大编号为 %d\n", list->count, list->max_id);
}

Problem *ui_input_problem()
{
    int i = 0;
    Problem *p = problem_new();

    printf("请输入题目相关信息: \n");

    printf("题目描述:\n$ ");
    scanf("%s", p->des);

    for (i = 'A'; i <= 'D'; i++) {
        printf("[选项 %c]:\n$ ", i);
        scanf("%s", p->opt[i - 'A']);
    }

    printf("正确答案字母序号:\n$ ");
    p->ans = ui_input_ans();

    printf("题目难度(1--10):\n$ ");
    while (scanf("%hi", &p->dif) != 1);
    if (p->dif < 0) p->dif = 0;
    if (p->dif > 10) p->dif = 10;

    printf("知识点标签(数字编号):\n$ ");
    while (scanf("%hi", &p->tag) != 1);

    printf("知识点章节(章.节):\n$ ");
    while (scanf("%hi.%hi", &p->chapter, &p->section) != 2);

    return p;
}

void ui_edit_problem(Problem *p)
{
    printf("修改题目信息（直接回车表示不修改）: \n");
    gotn();

    printf("题目描述: %s\n$ ", p->des);
    if (!gotn()) {
        scanf("%s", p->des);
        gotn();
    }

    int i = 0;
    for (i = 'A'; i <= 'D'; i++) {
        printf("[选项 %c]: %s\n$ ", i, p->opt[i - 'A']);
        if (!gotn()) {
            scanf("%s", p->opt[i - 'A']);
            gotn();
        }
    }

    printf("正确答案: %c\n$ ", p->ans);
    if (!gotn()) {
        p->ans = ui_input_ans();
        gotn();
    }

    printf("题目难度: (%hi) %s\n$ ", p->dif, dif2star(p->dif));
    if (!gotn()) {
        p->dif = ui_input_number();
        if (p->dif < 0) p->dif = 0;
        if (p->dif > 10) p->dif = 10;
        gotn();
    }

    printf("知识点标签: %hi\n$ ", p->tag);
    if (!gotn()) {
        p->tag = ui_input_number();
        gotn();
    }

    printf("知识点章节: %hi.%hi\n$ ", p->chapter, p->section);
    if (!gotn()) {
        scanf("%hi.%hi", &p->chapter, &p->section); // 未作容错处理
        gotn();
    }
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

Problem *ui_select_id(List *list)
{
    int id = -1;
    printf("题目编号:\n$ ");
    while (scanf("%d", &id) != 1);

    Problem *p = (Problem *)list_find(list, by_id, &id);
    if (p == NULL) {
        return NULL;
    }
    ui_output_problem(p, true);
    return p;
}

int ui_select_output(List *list, SListCallback *find, void *data)
{
    int n = list_find_each_call(list, find, data, ui_each_problem_show);
    printf("共查询到 %d 个符合条件的结果。\n", n);
    return n;
}

int ui_select_des(List *list)
{
    char des[256] = "";
    printf("题目描述（只需输入部分内容）:\n$ ");
    while (scanf("%s", des) != 1);
    printf("查找题目描述中含有 \"%s\" 的题目...\n", des);
    return ui_select_output(list, by_des, des);
}

int ui_select_opt(List *list)
{
    char opt[64] = "";
    printf("题目选项（只需输入部分内容）:\n$ ");
    while (scanf("%s", opt) != 1);
    printf("查找题目选项中含有 \"%s\" 的题目...\n", opt);
    return ui_select_output(list, by_opt, opt);
}

int ui_select_dif(List *list)
{
    sel_num t;
    printf("题目难度（输入 < > = == <= >= != <> 后空格数字）:\n$ ");
    while (scanf("%s%d", t.mark, &t.num) != 2);
    printf("查找题目难度 %s %d 的题目...\n", t.mark, t.num);
    return ui_select_output(list, by_difs, &t);
}

int ui_select_tag(List *list)
{
    short tags[64], i = 0;
    printf("题目标签编号（以 0 结尾）:\n$ ");
    for (; scanf("%hi", &tags[i]), tags[i] > 0; i++);
    printf("查找题目标签为");
    for (i = 0; tags[i] > 0; i++) printf(" %d", tags[i]);
    printf(" 的题目...\n");
    return ui_select_output(list, by_tags, tags);
}

int ui_select_sec(List *list)
{
    double secs[64];
    int i = 0;
    printf("题目章节编号（小数点分割章节，节可省略，以 0 结尾 ）:\n$ ");
    for (; scanf("%lf", &secs[i]), secs[i] > 0; i++);
    printf("查找题目章节为");
    for (i = 0; secs[i] > 0; i++) printf(" %.2lf", secs[i]);
    printf(" 的题目...\n");
    return ui_select_output(list, by_secs, secs);
}

int ui_select_mul(List *list)
{
    char key[64] = "";
    printf("输入要查询的关键字:\n$ ");
    while (scanf("%s", key) != 1);
    printf("查找题目中含有 \"%s\" 的题目...\n", key);
    return ui_select_output(list, by_mul, key);
}

void ui_paper_list()
{
    List *list = list_new();
    if (paper_read_list(list) < 0) {
        perror("读取试卷数据库失败");
        return;
    }
    ui_output_count(list);
    list_each_call(list, ui_each_paper_show, NULL);
    list_free(list);
}

void ui_paper_view(List *problist)
{
    List *list = list_new();
    if (paper_read_list(list) < 0) {
        perror("读取试卷数据库失败");
        return;
    }
    ui_output_count(list);
    int id = -1;
    printf("试卷编号:\n$ ");
    while (scanf("%d", &id) != 1);
    Paper *p = (Paper *)list_find(list, by_id, &id);
    if (p == NULL) {
        perror("没有这套试卷");
        return;
    }
    cls();
    paper_problem_call(p, problist, ui_each_problem_show);
    list_free(list);
}

int ui_paper_save(Paper *pa)
{
    if (pa->length <= 0) {
        perror("试卷中没有题目");
        return -1;
    }
    fprint_paper_pid(pa, stdout);
    if (!ui_get_yes("是否保存试卷？\n$ ")) {
        return 0;
    }
    List *list = list_new();
    if (paper_read_list(list) < 0) {
        perror("读取试卷数据库失败");
        goto end;
    }
    ui_output_count(list);
    pa->id = list->max_id + 1;
    int id = pa->id;
    if (list_insert(list, pa) < 0) {
        perror("保存试卷失败");
        goto end;
    }
    if (paper_write_list(list) < 0) {
        perror("写出试卷数据库失败");
        goto end;
    }
    printf("保存试卷 %d 成功！ \n", id);
end:
    list_free(list);
    return 0;
}
