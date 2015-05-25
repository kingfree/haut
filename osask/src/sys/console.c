/* 终端相关 */

#include "bootpack.h"
#include <stdio.h>
#include <string.h>

void console_task(sheet_t* sheet, unsigned int memtotal)
{
    task_t* task = task_now();
    memman_t* memman = (memman_t*)MEMMAN_ADDR;
    int i, *fat = (int *)memman_alloc_4k(memman, 4 * 2880);
    console cons;
    char cmdline[CONS_COLN];
    filehandle fhandle[8];

    cons.sht = sheet;
    cons.cur_x = CONS_LEFT;
    cons.cur_y = CONS_TOP;
    cons.cur_c = -1;
    task->cons = &cons;
    task->cmdline = cmdline;

    if (cons.sht != 0) {
        cons.timer = timer_alloc();
        timer_init(cons.timer, &task->fifo, 1);
        timer_settime(cons.timer, 50);
    }
    file_readfat(fat, (unsigned char*)(ADR_DISKIMG + 0x000200));
    for (i = 0; i < 8; i++) {
        fhandle[i].buf = 0; /* 未使用 */
    }
    task->fhandle = fhandle;
    task->fat = fat;

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
            if (i <= 1 && cons.sht != 0) { /* 光标闪烁 */
                if (i != 0) {
                    timer_init(cons.timer, &task->fifo, 0);
                    if (cons.cur_c >= 0) {
                        cons.cur_c = base3;
                    }
                } else {
                    timer_init(cons.timer, &task->fifo, 1);
                    if (cons.cur_c >= 0) {
                        cons.cur_c = base03;
                    }
                }
                timer_settime(cons.timer, 50);
            }
            if (i == 2) { /* 光标ON */
                cons.cur_c = base3;
            }
            if (i == 3) { /* 光标OFF */
                if (cons.sht != 0) {
                    boxfill8(cons.sht->buf, cons.sht->bxsize, base03, cons.cur_x, cons.cur_y, cons.cur_x + FNT_W - 1, cons.cur_y + FNT_H - 1);
                }
                cons.cur_c = -1;
            }
            if (i == 4) { /* 关闭窗口 */
                cmd_exit(&cons, fat);
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
                    if (cons.sht == 0) {
                        cmd_exit(&cons, fat);
                    }
                    /* 命令提示符 */
                    cons_putchar(&cons, '$', 1);
                    cons_putchar(&cons, ' ', 1);
                } else {
                    /* 一般字符 */
                    if (cons.cur_x < CONS_LEFT + CONS_COLW - FNT_W) {
                        /* 显示字符，后移一位 */
                        cmdline[(cons.cur_x - CONS_LEFT) / FNT_W - 2] = i - 256;
                        cons_putchar(&cons, i - 256, 1);
                    }
                }
            }
            /* 重新显示光标 */
            if (cons.sht != 0) {
                if (cons.cur_c >= 0) {
                    boxfill8(cons.sht->buf, cons.sht->bxsize, cons.cur_c, cons.cur_x, cons.cur_y, cons.cur_x + FNT_W - 1, cons.cur_y + FNT_H - 1);
                }
                sheet_refresh(cons.sht, cons.cur_x, cons.cur_y, cons.cur_x + FNT_W, cons.cur_y + FNT_H);
            }
        }
    }
}

void cons_putchar(console* cons, int chr, char move)
{
    int s[2] = {chr, 0};
    if (s[0] == 0x09) { /* Tab */
        for (;;) {
            if (cons->sht != 0) {
                putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, base3, base03, " ", 1);
            }
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
        if (cons->sht != 0) {
            putfonts8_asc_sht(cons->sht, cons->cur_x, cons->cur_y, base3, base03, s, 1);
        }
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

void cons_newline(console* cons)
{
    int x, y;
    sheet_t* sheet = cons->sht;
    if (cons->cur_y < CONS_TOP + CONS_LINH - FNT_H) {
        cons->cur_y += FNT_H; /* 换行 */
    } else {
        /* 滚动 */
        if (sheet != 0) {
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
    }
    cons->cur_x = CONS_LEFT;
    return;
}

void cons_putstr0(console* cons, char* s)
{
    int c;
    while (s = utf8char(s, &c)) {
        cons_putchar(cons, c, 1);
    }
    return;
}

void cons_putstr1(console* cons, char* s, int l)
{
    int i;
    for (i = 0; i < l; i++) {
        cons_putchar(cons, s[i], 1);
    }
    return;
}

void cons_runcmd(char* cmdline, console* cons, int* fat, unsigned int memtotal)
{
    char s[CONS_COLN];
    if (strcmp(cmdline, "mem") == 0) {
        cmd_mem(cons, memtotal);
    } else if (strcmp(cmdline, "clear") == 0 || strcmp(cmdline, "cls") == 0) {
        cmd_cls(cons);
    } else if (strcmp(cmdline, "ls -l") == 0 || strcmp(cmdline, "dir") == 0) {
        cmd_dir(cons);
    } else if (strcmp(cmdline, "exit") == 0) {
        cmd_exit(cons, fat);
    } else if (strncmp(cmdline, "start ", 6) == 0) {
        cmd_start(cons, cmdline, memtotal);
    } else if (strncmp(cmdline, "open ", 5) == 0) {
        cmd_open(cons, cmdline, memtotal);
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

void cmd_mem(console* cons, unsigned int memtotal)
{
    memman_t* memman = (memman_t*)MEMMAN_ADDR;
    char s[CONS_COLN];
    sprintf(s, "total %9dMB\nfree %10dKB\n", memtotal / (1024 * 1024), memman_total(memman) / 1024);
    cons_putstr0(cons, s);
    return;
}

void cmd_cls(console* cons)
{
    int x, y;
    sheet_t* sheet = cons->sht;
    for (y = CONS_TOP; y < CONS_TOP + CONS_LINH; y++) {
        for (x = CONS_LEFT; x < CONS_LEFT + CONS_COLW; x++) {
            sheet->buf[x + y * sheet->bxsize] = base03;
        }
    }
    sheet_refresh(sheet, CONS_LEFT, CONS_TOP, CONS_LEFT + CONS_COLW, CONS_TOP + CONS_LINH);
    cons->cur_y = CONS_TOP;
    return;
}

void cmd_dir(console* cons)
{
    fileinfo* finfo = (fileinfo*)(ADR_DISKIMG + 0x002600);
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

void cmd_exit(console* cons, int* fat)
{
    memman_t* memman = (memman_t*)MEMMAN_ADDR;
    task_t* task = task_now();
    shtctl_t* shtctl = (shtctl_t*)*((int*)0x0fe4);
    fifo32* fifo = (fifo32*)*((int*)0x0fec);
    timer_cancel(cons->timer);
    memman_free_4k(memman, (int)fat, 4 * 2880);
    io_cli();
    if (cons->sht != 0) {
        fifo32_put(fifo, cons->sht - shtctl->sheets0 + 768); /* 768〜1023 */
    } else {
        fifo32_put(fifo, task - taskctl->tasks0 + 1024); /* 1024〜2023 */
    }
    io_sti();
    for (;;) {
        task_sleep(task);
    }
}

void cmd_start(console* cons, char* cmdline, int memtotal)
{
    shtctl_t* shtctl = (shtctl_t*)*((int*)0x0fe4);
    sheet_t* sht = open_console(shtctl, memtotal);
    fifo32* fifo = &sht->task->fifo;
    int i;
    sheet_slide(sht, 32, 4);
    sheet_updown(sht, shtctl->top);
    /* 将键入的命令复制到新命令行窗口 */
    for (i = 6; cmdline[i] != 0; i++) {
        fifo32_put(fifo, cmdline[i] + 256);
    }
    fifo32_put(fifo, 10 + 256); /* Enter */
    cons_newline(cons);
    return;
}

void cmd_open(console* cons, char* cmdline, int memtotal)
{
    task_t* task = open_constask(0, memtotal);
    fifo32* fifo = &task->fifo;
    int i;
    /* 将键入的命令复制到新命令行窗口 */
    for (i = 5; cmdline[i] != 0; i++) {
        fifo32_put(fifo, cmdline[i] + 256);
    }
    fifo32_put(fifo, 10 + 256); /* Enter */
    cons_newline(cons);
    return;
}

int cmd_app(console* cons, int* fat, char* cmdline)
{
    memman_t* memman = (memman_t*)MEMMAN_ADDR;
    fileinfo* finfo;
    char name[18], *p, *q;
    task_t* task = task_now();
    int i, segsiz, datsiz, esp, dathrb;
    shtctl_t* shtctl;
    sheet_t* sht;

    /* 根据命令行生成文件名 */
    for (i = 0; i < 13; i++) {
        if (cmdline[i] <= ' ') {
            break;
        }
        name[i] = cmdline[i];
    }
    name[i] = 0; /* 先截断字符串 */

    /* 找文件 */
    finfo = file_search(name, (fileinfo*)(ADR_DISKIMG + 0x002600), 224);
    if (finfo == 0 && name[i - 1] != '.') {
        /* 找不到就加上后缀名再找一遍 */
        name[i] = '.';
        name[i + 1] = 'H';
        name[i + 2] = 'R';
        name[i + 3] = 'B';
        name[i + 4] = 0;
        finfo = file_search(name, (fileinfo*)(ADR_DISKIMG + 0x002600), 224);
    }

    if (finfo != 0) {
        /* 找到文件 */
        p = (char*)memman_alloc_4k(memman, finfo->size);
        file_loadfile(finfo->clustno, finfo->size, p, fat, (char*)(ADR_DISKIMG + 0x003e00));
        if (finfo->size >= 36 && strncmp(p + 4, "Hari", 4) == 0 && *p == 0x00) {
            segsiz = *((int*)(p + 0x0000));
            esp = *((int*)(p + 0x000c));
            datsiz = *((int*)(p + 0x0010));
            dathrb = *((int*)(p + 0x0014));
            q = (char*)memman_alloc_4k(memman, segsiz);
            task->ds_base = (int)q;
            set_segmdesc(task->ldt + 0, finfo->size - 1, (int)p, AR_CODE32_ER + 0x60);
            set_segmdesc(task->ldt + 1, segsiz - 1, (int)q, AR_DATA32_RW + 0x60);
            for (i = 0; i < datsiz; i++) {
                q[esp + i] = p[dathrb + i];
            }
            start_app(0x1b, 0 * 8 + 4, esp, 1 * 8 + 4, &(task->tss.esp0));
            shtctl = (shtctl_t*)*((int*)0x0fe4);
            for (i = 0; i < MAX_SHEETS; i++) {
                sht = &(shtctl->sheets0[i]);
                if ((sht->flags & 0x11) == 0x11 && sht->task == task) {
                    /* 找到应用程序遗留的窗口 */
                    sheet_free(sht); /* 关闭之 */
                }
            }
            for (i = 0; i < 8; i++) { /* 关闭所有打开的文件 */
                if (task->fhandle[i].buf != 0) {
                    memman_free_4k(memman, (int)task->fhandle[i].buf, task->fhandle[i].size);
                    task->fhandle[i].buf = 0;
                }
            }
            timer_cancelall(&task->fifo);
            memman_free_4k(memman, (int)q, segsiz);
        } else {
            cons_putstr0(cons, ".hrb file format error.\n");
        }
        memman_free_4k(memman, (int)p, finfo->size);
        cons_newline(cons);
        return 1;
    }
    /* 未找到文件 */
    return 0;
}

/* 系统调用 API */
int* hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
    task_t* task = task_now();
    int ds_base = task->ds_base;
    console* cons = task->cons;
    shtctl_t* shtctl = (shtctl_t*)*((int*)0x0fe4);
    sheet_t* sht;
    fifo32* sys_fifo = (fifo32*)*((int*)0x0fec);
    int* reg = &eax + 1; /* eax后面的地址 */
    /* 强行改写通过PUSHAD保存的值 */
    /* reg[0] : EDI,   reg[1] : ESI,   reg[2] : EBP,   reg[3] : ESP */
    /* reg[4] : EBX,   reg[5] : EDX,   reg[6] : ECX,   reg[7] : EAX */
    int i;
    fileinfo* finfo;
    filehandle* fh;
    memman_t* memman = (memman_t*)MEMMAN_ADDR;

    if (edx == 1) {
        cons_putchar(cons, eax & 0xff, 1);
    } else if (edx == 2) {
        cons_putstr0(cons, (char*)ebx + ds_base);
    } else if (edx == 3) {
        cons_putstr1(cons, (char*)ebx + ds_base, ecx);
    } else if (edx == 4) {
        return &(task->tss.esp0);
    } else if (edx == 5) {
        sht = sheet_alloc(shtctl);
        sht->task = task;
        sht->flags |= 0x10;
        sheet_setbuf(sht, (char*)ebx + ds_base, esi, edi, eax);
        make_window8((char*)ebx + ds_base, esi, edi, (char*)ecx + ds_base, 0);
        sheet_slide(sht, (shtctl->xsize - esi) / 2, (shtctl->ysize - edi) / 2);
        sheet_updown(sht, shtctl->top);
        reg[7] = (int)sht;
    } else if (edx == 6) {
        sht = (sheet_t*)(ebx & 0xfffffffe);
        putfonts8_asc(sht->buf, sht->bxsize, esi, edi, eax, (char*)ebp + ds_base);
        if ((ebx & 1) == 0) {
            sheet_refresh(sht, esi, edi, esi + ecx * FNT_W, edi + FNT_H);
        }
    } else if (edx == 7) {
        sht = (sheet_t*)(ebx & 0xfffffffe);
        boxfill8(sht->buf, sht->bxsize, ebp, eax, ecx, esi, edi);
        if ((ebx & 1) == 0) {
            sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
        }
    } else if (edx == 8) {
        memman_init((memman_t*)(ebx + ds_base));
        ecx &= 0xfffffff0; /* 以16字节为单位 */
        memman_free((memman_t*)(ebx + ds_base), eax, ecx);
    } else if (edx == 9) {
        ecx = (ecx + 0x0f) & 0xfffffff0; /* 以16为单位向上取整 */
        reg[7] = memman_alloc((memman_t*)(ebx + ds_base), ecx);
    } else if (edx == 10) {
        ecx = (ecx + 0x0f) & 0xfffffff0; /* 以16字节为单位向上取整 */
        memman_free((memman_t*)(ebx + ds_base), eax, ecx);
    } else if (edx == 11) {
        sht = (sheet_t*)(ebx & 0xfffffffe);
        sht->buf[sht->bxsize * edi + esi] = eax;
        if ((ebx & 1) == 0) {
            sheet_refresh(sht, esi, edi, esi + 1, edi + 1);
        }
    } else if (edx == 12) {
        sht = (sheet_t*)ebx;
        sheet_refresh(sht, eax, ecx, esi, edi);
    } else if (edx == 13) {
        sht = (sheet_t*)(ebx & 0xfffffffe);
        hrb_api_linewin(sht, eax, ecx, esi, edi, ebp);
        if ((ebx & 1) == 0) {
            sheet_refresh(sht, eax, ecx, esi + 1, edi + 1);
        }
    } else if (edx == 14) {
        sheet_free((sheet_t*)ebx);
    } else if (edx == 15) {
        for (;;) {
            io_cli();
            if (fifo32_status(&task->fifo) == 0) {
                if (eax != 0) {
                    task_sleep(task); /* FIFO为空时休眠并等待 */
                } else {
                    io_sti();
                    reg[7] = -1;
                    return 0;
                }
            }
            i = fifo32_get(&task->fifo);
            io_sti();
            if (i <= 1) { /* 光标用计时器 */
                /* 应用程序运行时并不需要显示光标 */
                timer_init(cons->timer, &task->fifo, 1); /* 置为1 */
                timer_settime(cons->timer, 50);
            }
            if (i == 2) { /* 光标显示 */
                cons->cur_c = base3;
            }
            if (i == 3) { /* 光标隐藏 */
                cons->cur_c = -1;
            }
            if (i == 4) { /* 只关闭终端窗口 */
                timer_cancel(cons->timer);
                io_cli();
                fifo32_put(sys_fifo, cons->sht - shtctl->sheets0 + 2024); /* 2024〜2279 */
                cons->sht = 0;
                io_sti();
            }
            if (256 <= i) { /* 通过任务A接收的键盘数据 */
                reg[7] = i - 256;
                return 0;
            }
        }
    } else if (edx == 16) {
        reg[7] = (int)timer_alloc();
        ((timer_t*)reg[7])->flags2 = 1; /* 允许自动取消 */
    } else if (edx == 17) {
        timer_init((timer_t*)ebx, &task->fifo, eax + 256);
    } else if (edx == 18) {
        timer_settime((timer_t*)ebx, eax);
    } else if (edx == 19) {
        timer_free((timer_t*)ebx);
    } else if (edx == 20) {
        if (eax == 0) {
            i = io_in8(0x61);
            io_out8(0x61, i & 0x0d);
        } else {
            i = 1193180000 / eax;
            io_out8(0x43, 0xb6);
            io_out8(0x42, i & 0xff);
            io_out8(0x42, i >> 8);
            i = io_in8(0x61);
            io_out8(0x61, (i | 0x03) & 0x0f);
        }
    } else if (edx == 21) {
        for (i = 0; i < 8; i++) {
            if (task->fhandle[i].buf == 0) {
                break;
            }
        }
        fh = &task->fhandle[i];
        reg[7] = 0;
        if (i < 8) {
            finfo = file_search((char*)ebx + ds_base,
                (fileinfo*)(ADR_DISKIMG + 0x002600), 224);
            if (finfo != 0) {
                reg[7] = (int)fh;
                fh->buf = (char*)memman_alloc_4k(memman, finfo->size);
                fh->size = finfo->size;
                fh->pos = 0;
                file_loadfile(finfo->clustno, finfo->size, fh->buf, task->fat, (char*)(ADR_DISKIMG + 0x003e00));
            }
        }
    } else if (edx == 22) {
        fh = (filehandle*)eax;
        memman_free_4k(memman, (int)fh->buf, fh->size);
        fh->buf = 0;
    } else if (edx == 23) {
        fh = (filehandle*)eax;
        if (ecx == 0) {
            fh->pos = ebx;
        } else if (ecx == 1) {
            fh->pos += ebx;
        } else if (ecx == 2) {
            fh->pos = fh->size + ebx;
        }
        if (fh->pos < 0) {
            fh->pos = 0;
        }
        if (fh->pos > fh->size) {
            fh->pos = fh->size;
        }
    } else if (edx == 24) {
        fh = (filehandle*)eax;
        if (ecx == 0) {
            reg[7] = fh->size;
        } else if (ecx == 1) {
            reg[7] = fh->pos;
        } else if (ecx == 2) {
            reg[7] = fh->pos - fh->size;
        }
    } else if (edx == 25) {
        fh = (filehandle*)eax;
        for (i = 0; i < ecx; i++) {
            if (fh->pos == fh->size) {
                break;
            }
            *((char*)ebx + ds_base + i) = fh->buf[fh->pos];
            fh->pos++;
        }
        reg[7] = i;
    } else if (edx == 26) {
        i = 0;
        for (;;) {
            *((char *) ebx + ds_base + i) =  task->cmdline[i];
            if (task->cmdline[i] == 0) {
                break;
            }
            if (i >= ecx) {
                break;
            }
            i++;
        }
        reg[7] = i;
    } else if (edx == 27) { /* 从UTF-8转换成Unicode */
    }
    return 0;
}

int* inthandler0c(int* esp)
{
    task_t* task = task_now();
    console* cons = task->cons;
    char s[30];
    cons_putstr0(cons, "\nINT 0C :\n Stack Exception.\n");
    sprintf(s, "EIP = %08X\n", esp[11]);
    cons_putstr0(cons, s);
    return &(task->tss.esp0); /* 异常终止 */
}

int* inthandler0d(int* esp)
{
    task_t* task = task_now();
    console* cons = task->cons;
    char s[30];
    cons_putstr0(cons, "\nINT 0D :\n General Protected Exception.\n");
    sprintf(s, "EIP = %08X\n", esp[11]);
    cons_putstr0(cons, s);
    return &(task->tss.esp0); /* 异常终止 */
}

void hrb_api_linewin(sheet_t* sht, int x0, int y0, int x1, int y1, int col)
{
    int i, x, y, len, dx, dy;

    dx = x1 - x0;
    dy = y1 - y0;
    x = x0 << 10;
    y = y0 << 10;
    if (dx < 0) {
        dx = -dx;
    }
    if (dy < 0) {
        dy = -dy;
    }
    if (dx >= dy) {
        len = dx + 1;
        if (x0 > x1) {
            dx = -1024;
        } else {
            dx = 1024;
        }
        if (y0 <= y1) {
            dy = ((y1 - y0 + 1) << 10) / len;
        } else {
            dy = ((y1 - y0 - 1) << 10) / len;
        }
    } else {
        len = dy + 1;
        if (y0 > y1) {
            dy = -1024;
        } else {
            dy = 1024;
        }
        if (x0 <= x1) {
            dx = ((x1 - x0 + 1) << 10) / len;
        } else {
            dx = ((x1 - x0 - 1) << 10) / len;
        }
    }

    for (i = 0; i < len; i++) {
        sht->buf[(y >> 10) * sht->bxsize + (x >> 10)] = col;
        x += dx;
        y += dy;
    }

    return;
}
