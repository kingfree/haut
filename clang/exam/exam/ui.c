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
#include "ui.h"

static char *NAME = "标准化考试系统";
static char *VERSION = "0.2.7";

static char *problem_db_name = "problem.db";
static char *paper_filetype = ".paper.db";

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

int input_option(char *menu, bool cl)
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

void ui_login()
{
    while (true) {
        switch (input_option(
            "   1 - 以学生身份登录\n"
            "   2 - 以教师身份登录\n"
            "   3 - 帮助\n"
            "   4 - 关于\n"
            "   0 - 退出系统\n"
            , true)) {
        case 1: ui_student(); break;
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

void ui_student()
{
    while (true) {
        switch (input_option(
            "   1 - 做卷子\n"
            "   2 - 看成绩\n"
            "   9 - 返回上一级\n"
            "   0 - 退出系统\n"
            , true)) {
        case 1: ui_student_test(); break;
        case 2: ui_student_score(); break;
        case 0: exit(0); break;
        default: return; break;
        }
    }
}

void ui_student_test()
{}

void ui_student_score()
{}

void ui_teacher_login()
{
	char *s = getpass("密码: ");
	printf("%s\n", s);
    // 处理密码
	ui_teacher();
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
    PList *db = plist_new();
    int n;
    if ((n = problem_read_file(db, problem_db_name)) < 0) {
        perror("读取题目数据库失败");
        return;
    }
    ui_output_count(db);
    slist_foreach(db->slist, ui_each_problem_show, NULL);
    plist_free(db);
    pause();
}

void ui_teacher_insert()
{
    PList *db = plist_new();
    int n;
    if ((n = problem_read_file(db, problem_db_name)) < 0) {
        perror("读取题目数据库失败");
        goto end;
    }
    ui_output_count(db);
    Problem *p = ui_input_problem();
    p->id = db->max_id + 1;
    int id = p->id;
    ui_output_problem(p, true);
    if (problem_insert(db, p) < 0) {
        perror("插入题目失败");
        goto end;
    }
    if (problem_write_file(db, problem_db_name) < 0) {
        perror("写入题目数据库失败");
        goto end;
    }
    printf("插入题目 %d 成功！\n", id);
end:
    plist_free(db);
    pause();
}

void ui_teacher_delete()
{
    PList *db = plist_new();
    int n;
    if ((n = problem_read_file(db, problem_db_name)) < 0) {
        perror("读取题目数据库失败");
        goto end;
    }
    ui_output_count(db);
    Problem *p = ui_select_id(db);
    if (p == NULL || p->id < 0) {
        perror("获取题目失败");
        goto end;
    }
    int id = p->id;
    printf("你确定要删除该题吗？输入'Y'或'y'以确认:\n$ ");
    gotn();
    char y = getchar();
    if (y != 'Y' && y != 'y') goto end;
    if (p != slist_unbox(slist_remove(&db->slist, by_id, &id))) {
        perror("删除题目失败");
        goto end;
    }
    problem_free(p);
    if (problem_write_file(db, problem_db_name) < 0) {
        perror("写入题目数据库失败");
        goto end;
    }
    printf("删除题目 %d 成功！ \n", id);
end:
    plist_free(db);
    pause();
}

void ui_teacher_update()
{
    PList *db = plist_new();
    int n;
    if ((n = problem_read_file(db, problem_db_name)) < 0) {
        perror("读取题目数据库失败");
        goto end;
    }
    ui_output_count(db);
    Problem *p = ui_select_id(db);
    if (p == NULL || p->id < 0) {
        perror("获取题目失败");
        goto end;
    }
    int id = p->id;
    ui_edit_problem(p);
    ui_output_problem(p, true);
    if (problem_write_file(db, problem_db_name) < 0) {
        perror("写入题目数据库失败");
        goto end;
    }
    printf("修改题目 %d 成功！ \n", id);
end:
    plist_free(db);
    pause();
}

void ui_teacher_select()
{
    PList *db = plist_new();
    int n;
    if ((n = problem_read_file(db, problem_db_name)) < 0) {
        perror("读取题目数据库失败");
        return;
    }
    ui_output_count(db);
    while (true) {
        cls();
        switch (input_option(
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
        case 1: ui_select_id(db); break;
        case 2: ui_select_des(db); break;
        case 3: ui_select_opt(db); break;
        case 4: ui_select_dif(db); break;
        case 5: ui_select_tag(db); break;
        case 6: ui_select_sec(db); break;
        case 7: ui_select_mul(db); break;
        case 0: exit(0); break;
        default: return; break;
        }
        pause();
    }
    plist_free(db);
}

void ui_teacher_generate()
{
    PList *db = plist_new();
    if (problem_read_file(db, problem_db_name) < 0) {
        perror("读取题目数据库失败");
        return;
    }
    while (true) {
        cls();
        switch (input_option(
            "选择智能组卷算法:\n"
            "   1 - 随机生成\n"
            "   2 - 按标签生成\n"
            "   3 - 按章节生成\n"
            "   4 - 按难度生成\n"
            "   5 - 自定义生成\n"
            "   6 - 试卷列表\n"
            "   9 - 返回上一级\n"
            "   0 - 退出系统\n"
            , false)) {
        case 1: ui_generate_random(db); break;
        case 2: ui_generate_tags(db); break;
        case 3: ui_generate_secs(db); break;
        case 4: ui_generate_dif(db); break;
        case 5: ui_generate_custom(db); break;
        case 6: ui_paper_list(); break;
        case 0: exit(0); break;
        default: return; break;
        }
        pause();
    }
    plist_free(db);
}

void ui_teacher_score()
{}

void ui_paper_list()
{
#ifdef WIN32
    char s[64] = "dir /B *";
#else
    char s[64] = "ls -1 *";
#endif
    strcat(s, paper_filetype);
    printf("已有的试卷文件:\n");
    system(s);
}

void ui_generate_random(PList *db)
{
    ui_output_count(db);
    printf("智能组卷 - 随机生成\n");
    printf("生成题目数:\n$ ");
    int n = ui_input_number();
    Paper *pa = paper_new();
    printf("试卷标题:\n$ ");
    scanf("%s", pa->title);
    paper_generate_random(pa, db, n);
    ui_paper_save(pa);
    paper_free(pa);
}

void ui_generate_tags(PList *db)
{
    ui_output_count(db);
    printf("智能组卷 - 按知识点标签生成\n");
    int tags[64], i = 0;
    printf("题目标签编号（以 0 结尾）:\n$ ");
    for (; scanf("%d", &tags[i]), tags[i] > 0; i++);
    printf("生成题目数:\n$ ");
    int n = ui_input_number();
    Paper *pa = paper_new();
    printf("试卷标题:\n$ ");
    scanf("%s", pa->title);
    paper_generate_tags(pa, db, n, tags, i);
    ui_paper_save(pa);
    paper_free(pa);
}

void ui_generate_secs(PList *db)
{
    ui_output_count(db);
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
    paper_generate_secs(pa, db, n, secs, i);
    ui_paper_save(pa);
    paper_free(pa);
}

void ui_generate_dif(PList *db)
{
    ui_output_count(db);
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
    paper_generate_dif(pa, db, n, a, b);
    ui_paper_save(pa);
    paper_free(pa);
}

void ui_generate_custom(PList *db)
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

void ui_output_count(PList *db)
{
    printf("数据库中有 %d 条记录，最大编号为 %d\n", db->count, db->max_id);
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

Problem *ui_select_id(PList *db)
{
    int id = -1;
    printf("题目编号:\n$ ");
    while (scanf("%d", &id) != 1);

    SList *s = (SList *)slist_find(db->slist, by_id, &id);
    Problem *p = NULL;
    if (s == NULL) {
        perror("没有找到题目");
        goto end;
    }
    p = (Problem *)s->userdata;
    if (p == NULL) {
        perror("没有题目数据");
        goto end;
    }
    ui_output_problem(p, true);
end:
    return p;
}

int ui_select_output(PList *db, SListCallback *find, void *matchdata)
{
    int n = 0;
    SList *s = db->slist;
    while ((s = (SList *)slist_find(s, find, matchdata)) != NULL) {
        n++;
        ui_each_problem_show(s, NULL);
        s = slist_tail(s);
    }
    printf("共查询到 %d 个符合条件的结果。\n", n);
    return n;
}

int ui_select_des(PList *db)
{
    char des[256] = "";
    printf("题目描述（只需输入部分内容）:\n$ ");
    while (scanf("%s", des) != 1);
    printf("查找题目描述中含有 \"%s\" 的题目...\n", des);
    return ui_select_output(db, by_des, des);
}

int ui_select_opt(PList *db)
{
    char opt[64] = "";
    printf("题目选项（只需输入部分内容）:\n$ ");
    while (scanf("%s", opt) != 1);
    printf("查找题目选项中含有 \"%s\" 的题目...\n", opt);
    return ui_select_output(db, by_opt, opt);
}

int ui_select_dif(PList *db)
{
    sel_num t;
    printf("题目难度（输入 < > = == <= >= != <> 后空格数字）:\n$ ");
    while (scanf("%s%d", t.mark, &t.num) != 2);
    printf("查找题目难度 %s %d 的题目...\n", t.mark, t.num);
    return ui_select_output(db, by_difs, &t);
}

int ui_select_tag(PList *db)
{
    short tags[64], i = 0;
    printf("题目标签编号（以 0 结尾）:\n$ ");
    for (; scanf("%hi", &tags[i]), tags[i] > 0; i++);
    printf("查找题目标签为");
    for (i = 0; tags[i] > 0; i++) printf(" %d", tags[i]);
    printf(" 的题目...\n");
    return ui_select_output(db, by_tags, tags);
}

int ui_select_sec(PList *db)
{
    double secs[64];
    int i = 0;
    printf("题目章节编号（小数点分割章节，节可省略，以 0 结尾 ）:\n$ ");
    for (; scanf("%lf", &secs[i]), secs[i] > 0; i++);
    printf("查找题目章节为");
    for (i = 0; secs[i] > 0; i++) printf(" %.2lf", secs[i]);
    printf(" 的题目...\n");
    return ui_select_output(db, by_secs, secs);
}

int ui_select_mul(PList *db)
{
    char key[64] = "";
    printf("输入要查询的关键字:\n$ ");
    while (scanf("%s", key) != 1);
    printf("查找题目中含有 \"%s\" 的题目...\n", key);
    return ui_select_output(db, by_mul, key);
}

int ui_paper_save(Paper *pa)
{
    if (pa->length <= 0) {
        perror("试卷中没有题目");
        return -1;
    }
    fprint_paper_pid(stdout, pa);
    char filename[64] = "";
    printf("请输入要保存的文件名（直接回车不保存）:\n$ ");
    gotn();
    if (gotn()) {
        return 0;
    }
    scanf("%s", filename);
    strcat(filename, paper_filetype);
    printf("正在保存试卷文件 \"%s\" ...", filename);
    if (paper_write(pa, filename) != 0) {
        perror("保存文件失败");
        return -1;
    }
    printf("成功\n");
    return 0;
}
