/* 终端相关 */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>

void console_task(sheet_t *sheet, unsigned int memtotal)
{
    timer_t *timer;
    task_t *task = task_now();
    int fifobuf[128], cursor_x = CONS_LEFT + FNT_W * 2, cursor_y = CONS_TOP, cursor_c = -1;
    char s[CONS_COLN], cmdline[CONS_COLN], *p;
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    fileinfo *finfo = (fileinfo *) (ADR_DISKIMG + 0x002600);
    int *fat = (int *) memman_alloc_4k(memman, 4 * 2880); /* FAT12 */
    int x, y, i;
    segment_descriptor *gdt = (segment_descriptor *) ADR_GDT;

    fifo32_init(&task->fifo, 128, fifobuf, task);
    timer = timer_alloc();
    timer_init(timer, &task->fifo, 1);
    timer_settime(timer, 50);
    file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));

    /* 命令提示符 */
    putfonts8_asc_sht(sheet, 3, 23, base3, base03, "$ ", 2);

    for (;;) {
        io_cli();
        if (fifo32_status(&task->fifo) == 0) {
            task_sleep(task);
            io_sti();
        } else {
            i = fifo32_get(&task->fifo);
            io_sti();
            if (i <= 1) {
                if (i != 0) {
                    timer_init(timer, &task->fifo, 0);
                    if (cursor_c >= 0) {
                        cursor_c = base3;
                    }
                } else {
                    timer_init(timer, &task->fifo, 1);
                    if (cursor_c >= 0) {
                        cursor_c = base03;
                    }
                }
                timer_settime(timer, 50);
            }
            if (i == 2) {
                cursor_c = base3;
            }
            if (i == 3) {
                boxfill8(sheet->buf, sheet->bxsize, base03, cursor_x, cursor_y, cursor_x + FNT_W - 1, cursor_y + FNT_H - 1);
                cursor_c = -1;
            }
            if (256 <= i && i <= 511) { /* 键盘数据 */
                if (i == 8 + 256) { /* 退格键 */
                    if (cursor_x > CONS_LEFT + FNT_W * 2) {
                        putfonts8_asc_sht(sheet, cursor_x, cursor_y, base3, base03, " ", 1);
                        cursor_x -= FNT_W;
                    }
                } else if (i == 10 + 256) { /* 回车键 */
                    /* 擦除光标 */
                    putfonts8_asc_sht(sheet, cursor_x, cursor_y, base3, base03, " ", 1);
                    cmdline[cursor_x / FNT_W - 2] = 0;
                    cursor_y = cons_newline(cursor_y, sheet);
                    /* 执行命令 */
                    if (strcmp(cmdline, "mem") == 0) {
                        sprintf(s, "total %8dMB", memtotal / (1024 * 1024));
                        putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, s, CONS_COLN);
                        cursor_y = cons_newline(cursor_y, sheet);
                        sprintf(s, "free  %8dKB", memman_total(memman) / 1024);
                        putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, s, CONS_COLN);
                        cursor_y = cons_newline(cursor_y, sheet);
                    } else if (strcmp(cmdline, "clear") == 0 || strcmp(cmdline, "cls") == 0) {
                        for (y = CONS_TOP; y < CONS_LINH + CONS_TOP; y++) {
                            for (x = CONS_LEFT; x < CONS_COLW + CONS_LEFT; x++) {
                                sheet->buf[x + y * sheet->bxsize] = base03;
                            }
                        }
                        sheet_refresh(sheet, CONS_LEFT, CONS_TOP, CONS_COLW + CONS_LEFT, CONS_LINH + CONS_TOP);
                        cursor_y = CONS_TOP;
                    } else if (strcmp(cmdline, "ls -l") == 0 || strcmp(cmdline, "dir") == 0) {
                        for (x = 0; x < 224; x++) {
                            if (finfo[x].name[0] == 0x00) {
                                break;
                            }
                            if (finfo[x].name[0] != 0xe5) {
                                if ((finfo[x].type & 0x18) == 0) {
                                    sprintf(s, "filename.ext   %7d", finfo[x].size);
                                    for (y = 0; y < 8; y++) {
                                        s[y] = finfo[x].name[y];
                                    }
                                    s[9] = finfo[x].ext[0];
                                    s[10] = finfo[x].ext[1];
                                    s[11] = finfo[x].ext[2];
                                    putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, s, CONS_COLN);
                                    cursor_y = cons_newline(cursor_y, sheet);
                                }
                            }
                        }
                    } else if (strncmp(cmdline, "cat ", 4) == 0 || strncmp(cmdline, "type ", 5) == 0) {
                        for (y = 0; y < 11; y++) {
                            s[y] = ' ';
                        }
                        s[11] = 0;
                        x = (cmdline[0] == 'c') ? 4 : 5;
                        for (y = 0; y < 11 && cmdline[x] != 0; x++) {
                            if (cmdline[x] == '.' && y <= 8) {
                                y = 8;
                            } else {
                                s[y] = cmdline[x];
                                if ('a' <= s[y] && s[y] <= 'z') {
                                    s[y] -= 0x20; /* 转成大写 */
                                }
                                y++;
                            }
                        }
                        for (x = 0; x < 224;) {
                            if (finfo[x].name[0] == 0x00) {
                                break;
                            }
                            if ((finfo[x].type & 0x18) == 0) {
                                for (y = 0; y < 11; y++) {
                                    if (finfo[x].name[y] != s[y]) {
                                        goto type_next_file;
                                    }
                                }
                                break; /* 找到文件 */
                            }
type_next_file:
                            x++;
                        }
                        if (x < 224 && finfo[x].name[0] != 0x00) {
                            /* 找到文件 */
                            p = (char *) memman_alloc_4k(memman, finfo[x].size);
                            file_loadfile(finfo[x].clustno, finfo[x].size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
                            cursor_x = CONS_LEFT;
                            for (y = 0; y < finfo[x].size; y++) {
                                /* 逐字输出 */
                                s[0] = p[y], s[1] = 0;
                                if (s[0] == 0x09) { /* 制表符 */
                                    for (;;) {
                                        putfonts8_asc_sht(sheet, cursor_x, cursor_y, base3, base03, " ", 1);
                                        cursor_x += FNT_W;
                                        if (cursor_x == CONS_LEFT + CONS_COLW - FNT_W) {
                                            cursor_x = CONS_LEFT;
                                            cursor_y = cons_newline(cursor_y, sheet);
                                        }
                                        if ((cursor_x - CONS_LEFT) % (FNT_W * 4) == 0) {
                                            break;
                                        }
                                    }
                                } else if (s[0] == 0x0a) { /* 换行符 */
                                    cursor_x = CONS_LEFT;
                                    cursor_y = cons_newline(cursor_y, sheet);
                                } else if (s[0] == 0x0d) { /* 回车符 */
                                    cursor_x = CONS_LEFT;
                                } else { /* 一般字符 */
                                    putfonts8_asc_sht(sheet, cursor_x, cursor_y, base3, base03, s, 1);
                                    cursor_x += FNT_W;
                                    if (cursor_x == CONS_LEFT + CONS_COLW - FNT_W) {
                                        cursor_x = CONS_LEFT;
                                        cursor_y = cons_newline(cursor_y, sheet);
                                    }
                                }
                            }
                            memman_free_4k(memman, (int) p, finfo[x].size);
                        } else {
                            /* 没有找到文件 */
                            sprintf(cmdline, "File '%s' not found.", s);
                            putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, cmdline, CONS_COLN);
                        }
                        cursor_y = cons_newline(cursor_y, sheet);
                    } else if (strcmp(cmdline, "hlt") == 0) {
                        /* 启动hlt.hrb应用程序 */
                        for (y = 0; y < 11; y++) {
                            s[y] = ' ';
                        }
                        s[11] = 0;
                        s[0] = 'H';
                        s[1] = 'L';
                        s[2] = 'T';
                        s[8] = 'H';
                        s[9] = 'R';
                        s[10] = 'B';
                        for (x = 0; x < 224;) {
                            if (finfo[x].name[0] == 0x00) {
                                break;
                            }
                            if ((finfo[x].type & 0x18) == 0) {
                                for (y = 0; y < 11; y++) {
                                    if (finfo[x].name[y] != s[y]) {
                                        goto hlt_next_file;
                                    }
                                }
                                break; /* 没找到文件 */
                            }
hlt_next_file:
                            x++;
                        }
                        if (x < 224 && finfo[x].name[0] != 0x00) {
                            /* 找到文件 */
                            p = (char *) memman_alloc_4k(memman, finfo[x].size);
                            file_loadfile(finfo[x].clustno, finfo[x].size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
                            set_segmdesc(gdt + 1003, finfo[x].size - 1, (int) p, AR_CODE32_ER);
                            farjmp(0, 1003 * 8);
                            memman_free_4k(memman, (int) p, finfo[x].size);
                        } else {
                            /* 没找到文件 */
                            sprintf(cmdline, "File '%s' not found.", s);
                            putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, cmdline, CONS_COLN);
                        }
                        cursor_y = cons_newline(cursor_y, sheet);
                    } else if (cmdline[0] != 0) {
                        cmdline[16] = 0;
                        sprintf(s, "Command '%s' not found.", cmdline);
                        putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, s, CONS_COLN);
                        cursor_y = cons_newline(cursor_y, sheet);
                    }
                    /* 命令提示符 */
                    putfonts8_asc_sht(sheet, CONS_LEFT, cursor_y, base3, base03, "$ ", 2);
                    cursor_x = CONS_LEFT + FNT_W * 2;
                } else { /* 一般字符 */
                    if (cursor_x < CONS_LEFT + CONS_COLW - FNT_W) {
                        s[0] = i - 256;
                        s[1] = 0;
                        cmdline[cursor_x / FNT_W - 2] = i - 256;
                        putfonts8_asc_sht(sheet, cursor_x, cursor_y, base3, base03, s, 2);
                        cursor_x += FNT_W;
                    }
                }
            }
            if (cursor_c >= 0) {
                boxfill8(sheet->buf, sheet->bxsize, cursor_c, cursor_x, cursor_y, cursor_x + FNT_W - 1, cursor_y + FNT_H - 1);
            }
            sheet_refresh(sheet, cursor_x, cursor_y, cursor_x + FNT_W, cursor_y + FNT_H);
        }
    }
}

int cons_newline(int cursor_y, sheet_t *sheet)
{
    if (cursor_y < CONS_LINH - FNT_H + CONS_TOP) {
        cursor_y += FNT_H;
    } else { /* 向下滚动一行 */
        int x, y;
        for (y = CONS_TOP; y < CONS_LINH - FNT_H + CONS_TOP; y++) {
            for (x = CONS_LEFT; x < CONS_COLW + CONS_LEFT; x++) {
                sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + FNT_H) * sheet->bxsize];
            }
        }
        for (y = CONS_LINH - FNT_H + CONS_TOP; y < CONS_LINH + CONS_TOP; y++) {
            for (x = CONS_LEFT; x < CONS_COLW + CONS_LEFT; x++) {
                sheet->buf[x + y * sheet->bxsize] = base03;
            }
        }
        sheet_refresh(sheet, CONS_LEFT, CONS_TOP, CONS_COLW + CONS_LEFT, FNT_H * CONS_LINH + CONS_TOP);
    }
    return cursor_y;
}
