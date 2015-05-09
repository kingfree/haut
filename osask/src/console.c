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
    *((int *) 0x0fec) = (int) &cons;
    
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
            if (i == 2) {   /* 光标ON */
                cons.cur_c = base3;
            }
            if (i == 3) {   /* 光标OFF */
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
                    cons_runcmd(cmdline, &cons, fat, memtotal); /* 执行命令 */
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
    if (s[0] == 0x09) { /* Tab */
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
        cons->cur_x = CONS_LEFT;
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

void cons_putstr0(console *cons, char *s)
{
    for (; *s != 0; s++) {
        cons_putchar(cons, *s, 1);
    }
    return;
}

void cons_putstr1(console *cons, char *s, int l)
{
    int i;
    for (i = 0; i < l; i++) {
        cons_putchar(cons, s[i], 1);
    }
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
    } else if (cmdline[0] != 0) {
        if (cmd_app(cons, fat, cmdline) == 0) {
            /* 不是有效命令，也不是空行 */
            cmdline[8] = 0;
            sprintf(s, "Command '%s' not found.\n", cmdline);
            cons_putstr0(cons, s);
        }
    }
    return;
}

void cmd_mem(console *cons, unsigned int memtotal)
{
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    char s[CONS_COLN];
    sprintf(s, "total %9dMB\nfree %10dKB\n", memtotal / (1024 * 1024), memman_total(memman) / 1024);
    cons_putstr0(cons, s);
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
                sprintf(s, "filename.ext   %7d\n", finfo[i].size);
                for (j = 0; j < 8; j++) {
                    s[j] = finfo[i].name[j];
                }
                s[9] = finfo[i].ext[0];
                s[10] = finfo[i].ext[1];
                s[11] = finfo[i].ext[2];
                cons_putstr0(cons, s);
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
    if (finfo != 0) {
        /* 找到文件 */
        p = (char *) memman_alloc_4k(memman, finfo->size);
        file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
        cons_putstr1(cons, p, finfo->size);
        memman_free_4k(memman, (int) p, finfo->size);
    } else {
        /* 未找到文件 */
        sprintf(s, "File '%s' not found.", filename);
        cons_putstr0(cons, s);
    }
    cons_newline(cons);
    return;
}

int cmd_app(console *cons, int *fat, char *cmdline)
{
    memman_t *memman = (memman_t *) MEMMAN_ADDR;
    fileinfo *finfo;
    segment_descriptor *gdt = (segment_descriptor *) ADR_GDT;
    char name[18], *p, *q;
    task_t *task = task_now();
    int i, segsiz, datsiz, esp, dathrb;
    
    /* 根据命令行生成文件名 */
    for (i = 0; i < 13; i++) {
        if (cmdline[i] <= ' ') {
            break;
        }
        name[i] = cmdline[i];
    }
    name[i] = 0; /* 先截断字符串 */
    
    /* 找文件 */
    finfo = file_search(name, (fileinfo *) (ADR_DISKIMG + 0x002600), 224);
    if (finfo == 0 && name[i - 1] != '.') {
        /* 找不到就加上后缀名再找一遍 */
        name[i] = '.';
        name[i + 1] = 'H';
        name[i + 2] = 'R';
        name[i + 3] = 'B';
        name[i + 4] = 0;
        finfo = file_search(name, (fileinfo *) (ADR_DISKIMG + 0x002600), 224);
    }
    
    if (finfo != 0) {
        /* 找到文件 */
        p = (char *) memman_alloc_4k(memman, finfo->size);
        file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
        if (finfo->size >= 36 && strncmp(p + 4, "Hari", 4) == 0 && *p == 0x00) {
            segsiz = *((int *) (p + 0x0000));
            esp = *((int *) (p + 0x000c));
            datsiz = *((int *) (p + 0x0010));
            dathrb = *((int *) (p + 0x0014));
            q = (char *) memman_alloc_4k(memman, segsiz);
            *((int *) 0xfe8) = (int) q;
            set_segmdesc(gdt + 1003, finfo->size - 1, (int) p, AR_CODE32_ER + 0x60);
            set_segmdesc(gdt + 1004, segsiz - 1, (int) q, AR_DATA32_RW + 0x60);
            for (i = 0; i < datsiz; i++) {
                q[esp + i] = p[dathrb + i];
            }
            start_app(0x1b, 1003 * 8, esp, 1004 * 8, &(task->tss.esp0));
            memman_free_4k(memman, (int) q, segsiz);
        } else {
            cons_putstr0(cons, ".hrb file format error.\n");
        }
        memman_free_4k(memman, (int) p, finfo->size);
        cons_newline(cons);
        return 1;
    }
    /* 未找到文件 */
    return 0;
}

/* 系统调用 API */
int *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
    int ds_base = *((int *) 0xfe8);
    task_t *task = task_now();
    console *cons = (console *) *((int *) 0x0fec);
    shtctl_t *shtctl = (shtctl_t *) *((int *) 0x0fe4);
    sheet_t *sht;
    int *reg = &eax + 1;	/* eax后面的地址 */
    /* 强行改写通过PUSHAD保存的值 */
    /* reg[0] : EDI,   reg[1] : ESI,   reg[2] : EBP,   reg[3] : ESP */
    /* reg[4] : EBX,   reg[5] : EDX,   reg[6] : ECX,   reg[7] : EAX */
    
    if (edx == 1) {
        cons_putchar(cons, eax & 0xff, 1);
    } else if (edx == 2) {
        cons_putstr0(cons, (char *) ebx + ds_base);
    } else if (edx == 3) {
        cons_putstr1(cons, (char *) ebx + ds_base, ecx);
    } else if (edx == 4) {
        return &(task->tss.esp0);
    } else if (edx == 5) {
        sht = sheet_alloc(shtctl);
        sheet_setbuf(sht, (char *) ebx + ds_base, esi, edi, eax);
        make_window8((char *) ebx + ds_base, esi, edi, (char *) ecx + ds_base, 0);
        sheet_slide(sht, 100, 50);
        sheet_updown(sht, 3);	/* 层高3 */
        reg[7] = (int) sht;
    } else if (edx == 6) {
        sht = (sheet_t *) ebx;
        putfonts8_asc(sht->buf, sht->bxsize, esi, edi, eax, (char *) ebp + ds_base);
        sheet_refresh(sht, esi, edi, esi + ecx * 8, edi + 16);
    } else if (edx == 7) {
        sht = (sheet_t *) ebx;
        boxfill8(sht->buf, sht->bxsize, ebp, eax, ecx, esi, edi);
        sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
    } else if (edx == 8) {
        memman_init((memman_t *) (ebx + ds_base));
		ecx &= 0xfffffff0;	/* 以16字节为单位 */
		memman_free((memman_t *) (ebx + ds_base), eax, ecx);
	} else if (edx == 9) {
		ecx = (ecx + 0x0f) & 0xfffffff0; /* 以16为单位向上取整 */
		reg[7] = memman_alloc((memman_t *) (ebx + ds_base), ecx);
	} else if (edx == 10) {
		ecx = (ecx + 0x0f) & 0xfffffff0; /* 以16字节为单位向上取整 */
		memman_free((memman_t *) (ebx + ds_base), eax, ecx);
	} else if (edx == 11) {
		sht = (sheet_t *) ebx;
		sht->buf[sht->bxsize * edi + esi] = eax;
		sheet_refresh(sht, esi, edi, esi + 1, edi + 1);
    }
    return 0;
}

int *inthandler0c(int *esp)
{
    console *cons = (console *) *((int *) 0x0fec);
    task_t *task = task_now();
    char s[30];
    cons_putstr0(cons, "\nINT 0C :\n Stack Exception.\n");
    sprintf(s, "EIP = %08X\n", esp[11]);
    cons_putstr0(cons, s);
    return &(task->tss.esp0);   /* 异常终止 */
}

int *inthandler0d(int *esp)
{
    console *cons = (console *) *((int *) 0x0fec);
    task_t *task = task_now();
    char s[30];
    cons_putstr0(cons, "\nINT 0D :\n General Protected Exception.\n");
    sprintf(s, "EIP = %08X\n", esp[11]);
    cons_putstr0(cons, s);
    return &(task->tss.esp0);   /* 异常终止 */
}
