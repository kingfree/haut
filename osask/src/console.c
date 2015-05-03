/* 终端相关 */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>

void console_task(sheet_t *sheet, unsigned int memtotal)
{
    timer_t *timer;
    task_t *task = task_now();
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    int i, fifobuf[128], *fat = (int *) memman_alloc_4k(memman, 4 * 2880);
    console cons;
    char cmdline[CONS_COLN];
    cons.sht = sheet;
    cons.cur_x = CONS_LEFT;
    cons.cur_y = CONS_TOP;
    cons.cur_c = -1;

    fifo32_init(&task->fifo, 128, fifobuf, task);
    timer = timer_alloc();
    timer_init(timer, &task->fifo, 1);
    timer_settime(timer, 50);
    file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));

    /* 命令提示符 */
    cons_putchar(&cons, '$', 1);
    cons_putchar(&cons, ' ', 1);

    for (;;) {
        io_cli();
        if (fifo32_status(&task->fifo) == 0) {
            task_sleep(task);
            io_sti();
        } else {
            i = fifo32_get(&task->fifo);
            io_sti();
            if (i <= 1) { /* 光标闪烁 */
                if (i != 0) {
                    timer_init(timer, &task->fifo, 0);
                    if (cons.cur_c >= 0) {
                        cons.cur_c = base3;
                    }
                } else {
                    timer_init(timer, &task->fifo, 1);
                    if (cons.cur_c >= 0) {
                        cons.cur_c = base03;
                    }
                }
                timer_settime(timer, 50);
            }
            if (i == 2) {	/* 光标ON */
                cons.cur_c = base3;
            }
            if (i == 3) {	/* 光标OFF */
                boxfill8(sheet->buf, sheet->bxsize, base03, cons.cur_x, cons.cur_y, cons.cur_x + FNT_W - 1, cons.cur_y + FNT_H - 1);
                cons.cur_c = -1;
            }
            if (256 <= i && i <= 511) { /* 键盘数据（从任务A） */
                if (i == 8 + 256) {
                    /* 退格键 */
                    if (cons.cur_x > CONS_LEFT + FNT_W * 2) {
                        /* 擦除光标，前移一位 */
                        cons_putchar(&cons, ' ', 0);
                        cons.cur_x -= FNT_W;
                    }
                } else if (i == 10 + 256) {
                    /* Enter */
                    /* 擦除光标，换行 */
                    cons_putchar(&cons, ' ', 0);
                    cmdline[(cons.cur_x - CONS_LEFT) / FNT_W - 2] = 0;
                    cons_newline(&cons);
                    cons_runcmd(cmdline, &cons, fat, memtotal);	/* 执行命令 */
                    /* 命令提示符 */
                    cons_putchar(&cons, '$', 1);
                    cons_putchar(&cons, ' ', 1);
                } else {
                    /* 一般字符 */
                    if (cons.cur_x < CONS_COLW) {
                        /* 显示字符，后移一位 */
                        cmdline[(cons.cur_x - CONS_LEFT) / FNT_W - 2] = i - 256;
                        cons_putchar(&cons, i - 256, 1);
                    }
                }
            }
            /* 重新显示光标 */
            if (cons.cur_c >= 0) {
                boxfill8(sheet->buf, sheet->bxsize, cons.cur_c, cons.cur_x, cons.cur_y, cons.cur_x + FNT_W - 1, cons.cur_y + FNT_H - 1);
            }
            sheet_refresh(sheet, cons.cur_x, cons.cur_y, cons.cur_x + FNT_W, cons.cur_y + FNT_H);
        }
    }
}

void cons_putchar(console *cons, int chr, char move)
{
    char s[2];
    s[0] = chr;
    s[1] = 0;
    if (s[0] == 0x09) {	/* Tab */
        for (;;) {
            putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, base3, base03, " ", 1);
            cons->cur_x += FNT_W;
            if (cons->cur_x == CONS_LEFT + CONS_COLW) {
                cons_newline(cons);
            }
            if ((cons->cur_x - CONS_LEFT) % (4 * FNT_W) == 0) {
                break;
            }
        }
    } else if (s[0] == 0x0a) { /* 换行 */
        cons_newline(cons);
    } else if (s[0] == 0x0d) { /* 回车 */
        /* 什么也不做 */
    } else { /* 一般字符 */
        putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, base3, base03, s, 1);
        if (move != 0) {
            /* move为0の时不后移光标 */
            cons->cur_x += FNT_W;
            if (cons->cur_x == CONS_LEFT + CONS_COLW) {
                cons_newline(cons);
            }
        }
    }
    return;
}

void cons_newline(console *cons)
{
    int x, y;
    sheet_t *sheet = cons->sht;
    if (cons->cur_y < CONS_TOP + CONS_LINH - FNT_H) {
        cons->cur_y += FNT_H; /* 换行 */
    } else {
        /* 滚动 */
        for (y = CONS_TOP; y < CONS_TOP + CONS_LINH - FNT_H; y++) {
            for (x = CONS_LEFT; x < CONS_LEFT + CONS_COLW; x++) {
                sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + FNT_H) * sheet->bxsize];
            }
        }
        for (y = CONS_TOP + CONS_LINH - FNT_H; y < CONS_TOP + CONS_LINH; y++) {
            for (x = CONS_LEFT; x < CONS_LEFT + CONS_COLW; x++) {
                sheet->buf[x + y * sheet->bxsize] = base03;
            }
        }
        sheet_refresh(sheet, CONS_LEFT, CONS_TOP, CONS_LEFT + CONS_COLW, CONS_TOP + CONS_LINH);
    }
    cons->cur_x = CONS_LEFT;
    return;
}

void cons_runcmd(char *cmdline, console *cons, int *fat, unsigned int memtotal)
{
    char s[CONS_COLN];
    if (strcmp(cmdline, "mem") == 0) {
        cmd_mem(cons, memtotal);
    } else if (strcmp(cmdline, "clear") == 0 || strcmp(cmdline, "cls") == 0) {
        cmd_cls(cons);
    } else if (strcmp(cmdline, "ls -l") == 0 || strcmp(cmdline, "dir") == 0) {
        cmd_dir(cons);
    } else if (strncmp(cmdline, "cat ", 4) == 0 || strncmp(cmdline, "type ", 5) == 0) {
        cmd_type(cons, fat, cmdline + (cmdline[0] == 'c' ? 4 : 5));
    } else if (strcmp(cmdline, "hlt") == 0) {
        cmd_hlt(cons, fat);
    } else if (cmdline[0] != 0) {
        /* 不是有效命令，也不是空行 */
        cmdline[16] = 0;
        sprintf(s, "Command '%s' not found.", cmdline);
        putfonts8_asc_sht(cons->sht, CONS_LEFT, cons->cur_y, base3, base03, s, strlen(s));
        cons_newline(cons);
    }
    return;
}

void cmd_mem(console *cons, unsigned int memtotal)
{
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    char s[CONS_COLN];
    sprintf(s, "total %9dMB", memtotal / (1024 * 1024));
    putfonts8_asc_sht(cons->sht, CONS_LEFT, cons->cur_y, base3, base03, s, CONS_COLN);
    cons_newline(cons);
    sprintf(s, "free %10dKB", memman_total(memman) / 1024);
    putfonts8_asc_sht(cons->sht, CONS_LEFT, cons->cur_y, base3, base03, s, CONS_COLN);
    cons_newline(cons);
    return;
}

void cmd_cls(console *cons)
{
    int x, y;
    sheet_t *sheet = cons->sht;
    for (y = CONS_TOP; y < CONS_TOP + CONS_LINH; y++) {
        for (x = CONS_LEFT; x < CONS_LEFT + CONS_COLW; x++) {
            sheet->buf[x + y * sheet->bxsize] = base03;
        }
    }
    sheet_refresh(sheet, CONS_LEFT, CONS_TOP, CONS_LEFT + CONS_COLW, CONS_TOP + CONS_LINH);
    cons->cur_y = CONS_TOP;
    return;
}

void cmd_dir(console *cons)
{
    fileinfo *finfo = (fileinfo *) (ADR_DISKIMG + 0x002600);
    int i, j;
    char s[CONS_COLN];
    for (i = 0; i < 224; i++) {
        if (finfo[i].name[0] == 0x00) {
            break;
        }
        if (finfo[i].name[0] != 0xe5) {
            if ((finfo[i].type & 0x18) == 0) {
                sprintf(s, "filename.ext   %7d", finfo[i].size);
                for (j = 0; j < 8; j++) {
                    s[j] = finfo[i].name[j];
                }
                s[9] = finfo[i].ext[0];
                s[10] = finfo[i].ext[1];
                s[11] = finfo[i].ext[2];
                putfonts8_asc_sht(cons->sht, CONS_LEFT, cons->cur_y, base3, base03, s, CONS_COLN);
                cons_newline(cons);
            }
        }
    }
    return;
}

void cmd_type(console *cons, int *fat, char *filename)
{
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    fileinfo *finfo = file_search(filename, (fileinfo *) (ADR_DISKIMG + 0x002600), 224);
    char *p, s[CONS_COLN];
    int i;
    if (finfo != 0) {
        /* 找到文件 */
        p = (char *) memman_alloc_4k(memman, finfo->size);
        file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
        for (i = 0; i < finfo->size; i++) {
            cons_putchar(cons, p[i], 1);
        }
        memman_free_4k(memman, (int) p, finfo->size);
    } else {
        /* 未找到文件 */
        sprintf(s, "File '%s' not found.", filename);
        putfonts8_asc_sht(cons->sht, CONS_LEFT, cons->cur_y, base3, base03, s, CONS_COLN);
        cons_newline(cons);
    }
    return;
}

void cmd_hlt(console *cons, int *fat)
{
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    fileinfo *finfo = file_search("HLT.HRB", (fileinfo *) (ADR_DISKIMG + 0x002600), 224);
    segment_descriptor *gdt = (segment_descriptor *) ADR_GDT;
    char *p;
    if (finfo != 0) {
        /* 找到文件 */
        p = (char *) memman_alloc_4k(memman, finfo->size);
        file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
        set_segmdesc(gdt + 1003, finfo->size - 1, (int) p, AR_CODE32_ER);
        farjmp(0, 1003 * 8);
        memman_free_4k(memman, (int) p, finfo->size);
    } else {
        /* 未找到文件 */
        putfonts8_asc_sht(cons->sht, CONS_LEFT, cons->cur_y, base3, base03, "Executive file not found.", CONS_COLN);
        cons_newline(cons);
    }
    cons_newline(cons);
    return;
}
