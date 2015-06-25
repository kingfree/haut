/* 鼠标和窗口叠加处理 */

#include "bootpack.h"

#define SHEET_USE 1

shtctl_t* shtctl_init(memman_t* memman, unsigned char* vram, int xsize, int ysize)
{
    shtctl_t* ctl;
    int i;
    ctl = (shtctl_t*)memman_alloc_4k(memman, sizeof(shtctl_t));
    if (ctl == 0) {
        goto err;
    }
    ctl->map = (unsigned char*)memman_alloc_4k(memman, xsize * ysize);
    if (ctl->map == 0) {
        memman_free_4k(memman, (int)ctl, sizeof(shtctl_t));
        goto err;
    }
    ctl->vram = vram;
    ctl->xsize = xsize;
    ctl->ysize = ysize;
    ctl->top = -1; /* 暂无图层 */
    for (i = 0; i < MAX_SHEETS; i++) {
        ctl->sheets0[i].flags = 0; /* 标记为未使用 */
        ctl->sheets0[i].ctl = ctl; /* 记录所属 */
    }
err:
    return ctl;
}

sheet_t* sheet_alloc(shtctl_t* ctl)
{
    sheet_t* sht;
    int i;
    for (i = 0; i < MAX_SHEETS; i++) {
        if (ctl->sheets0[i].flags == 0) {
            sht = &ctl->sheets0[i];
            sht->flags = SHEET_USE; /* 标记为使用中 */
            sht->height = -1; /* 隐藏 */
            sht->task = 0; /* 不使用自动关闭功能 */
            return sht;
        }
    }
    return 0; /* 所有图层都在使用中 */
}

void sheet_setbuf(sheet_t* sht, unsigned char* buf, int xsize, int ysize, int alpha)
{
    sht->buf = buf;
    sht->bxsize = xsize;
    sht->bysize = ysize;
    sht->alpha = alpha;
    return;
}

void sheet_refreshmap(shtctl_t* ctl, int vx0, int vy0, int vx1, int vy1, int h0)
{
    int h, bx, by, vx, vy, bx0, by0, bx1, by1, sid4, *p;
    unsigned char *buf, sid, *map = ctl->map;
    sheet_t* sht;
    if (vx0 < 0) {
        vx0 = 0;
    }
    if (vy0 < 0) {
        vy0 = 0;
    }
    if (vx1 > ctl->xsize) {
        vx1 = ctl->xsize;
    }
    if (vy1 > ctl->ysize) {
        vy1 = ctl->ysize;
    }
    for (h = h0; h <= ctl->top; h++) {
        sht = ctl->sheets[h];
        sid = sht - ctl->sheets0; /* 地址相减得到图层号 */
        buf = sht->buf;
        bx0 = vx0 - sht->vx0;
        by0 = vy0 - sht->vy0;
        bx1 = vx1 - sht->vx0;
        by1 = vy1 - sht->vy0;
        if (bx0 < 0) {
            bx0 = 0;
        }
        if (by0 < 0) {
            by0 = 0;
        }
        if (bx1 > sht->bxsize) {
            bx1 = sht->bxsize;
        }
        if (by1 > sht->bysize) {
            by1 = sht->bysize;
        }
        if (sht->alpha == -1) {
            if ((sht->vx0 & 3) == 0 && (bx0 & 3) == 0 && (bx1 & 3) == 0) {
                /* 无透明色专用的高速版（4字节型） */
                bx1 = (bx1 - bx0) / 4; /* MOV次数 */
                sid4 = sid | sid << 8 | sid << 16 | sid << 24;
                for (by = by0; by < by1; by++) {
                    vy = sht->vy0 + by;
                    vx = sht->vx0 + bx0;
                    p = (int*)&map[vy * ctl->xsize + vx];
                    for (bx = 0; bx < bx1; bx++) {
                        p[bx] = sid4;
                    }
                }
            } else {
                /* 无透明色专用的高速版（1字节型） */
                for (by = by0; by < by1; by++) {
                    vy = sht->vy0 + by;
                    for (bx = bx0; bx < bx1; bx++) {
                        vx = sht->vx0 + bx;
                        map[vy * ctl->xsize + vx] = sid;
                    }
                }
            }
        } else {
            /* 有透明色的一般版 */
            for (by = by0; by < by1; by++) {
                vy = sht->vy0 + by;
                for (bx = bx0; bx < bx1; bx++) {
                    vx = sht->vx0 + bx;
                    if (buf[by * sht->bxsize + bx] != sht->alpha) {
                        map[vy * ctl->xsize + vx] = sid;
                    }
                }
            }
        }
    }
    return;
}

void sheet_refreshsub(shtctl_t* ctl, int vx0, int vy0, int vx1, int vy1, int h0, int h1)
{
    int h, bx, by, vx, vy, bx0, by0, bx1, by1, bx2, sid4, i, i1, *p, *q, *r;
    unsigned char *buf, *vram = ctl->vram, *map = ctl->map, sid;
    sheet_t* sht;
    /* 修正超出屏幕的刷新范围 */
    if (vx0 < 0) {
        vx0 = 0;
    }
    if (vy0 < 0) {
        vy0 = 0;
    }
    if (vx1 > ctl->xsize) {
        vx1 = ctl->xsize;
    }
    if (vy1 > ctl->ysize) {
        vy1 = ctl->ysize;
    }
    for (h = h0; h <= h1; h++) {
        sht = ctl->sheets[h];
        buf = sht->buf;
        sid = sht - ctl->sheets0;
        /* 用vx0～vy1逆推bx0～by1 */
        bx0 = vx0 - sht->vx0;
        by0 = vy0 - sht->vy0;
        bx1 = vx1 - sht->vx0;
        by1 = vy1 - sht->vy0;
        if (bx0 < 0) {
            bx0 = 0;
        }
        if (by0 < 0) {
            by0 = 0;
        }
        if (bx1 > sht->bxsize) {
            bx1 = sht->bxsize;
        }
        if (by1 > sht->bysize) {
            by1 = sht->bysize;
        }
        if ((sht->vx0 & 3) == 0) {
            /* 4字节型 */
            i = (bx0 + 3) / 4; /* bx0除以4（上取整） */
            i1 = bx1 / 4; /* bx1除以4（下取整） */
            i1 = i1 - i;
            sid4 = sid | sid << 8 | sid << 16 | sid << 24;
            for (by = by0; by < by1; by++) {
                vy = sht->vy0 + by;
                for (bx = bx0; bx < bx1 && (bx & 3) != 0; bx++) { /* 前面被4除余下部分逐字节写入 */
                    vx = sht->vx0 + bx;
                    if (map[vy * ctl->xsize + vx] == sid) {
                        vram[vy * ctl->xsize + vx] = buf[by * sht->bxsize + bx];
                    }
                }
                vx = sht->vx0 + bx;
                p = (int*)&map[vy * ctl->xsize + vx];
                q = (int*)&vram[vy * ctl->xsize + vx];
                r = (int*)&buf[by * sht->bxsize + bx];
                for (i = 0; i < i1; i++) { /* 4的倍数部分 */
                    if (p[i] == sid4) {
                        q[i] = r[i];
                    } else {
                        bx2 = bx + i * 4;
                        vx = sht->vx0 + bx2;
                        if (map[vy * ctl->xsize + vx + 0] == sid) {
                            vram[vy * ctl->xsize + vx + 0] = buf[by * sht->bxsize + bx2 + 0];
                        }
                        if (map[vy * ctl->xsize + vx + 1] == sid) {
                            vram[vy * ctl->xsize + vx + 1] = buf[by * sht->bxsize + bx2 + 1];
                        }
                        if (map[vy * ctl->xsize + vx + 2] == sid) {
                            vram[vy * ctl->xsize + vx + 2] = buf[by * sht->bxsize + bx2 + 2];
                        }
                        if (map[vy * ctl->xsize + vx + 3] == sid) {
                            vram[vy * ctl->xsize + vx + 3] = buf[by * sht->bxsize + bx2 + 3];
                        }
                    }
                }
                for (bx += i1 * 4; bx < bx1; bx++) { /* 余下部分逐字节写入 */
                    vx = sht->vx0 + bx;
                    if (map[vy * ctl->xsize + vx] == sid) {
                        vram[vy * ctl->xsize + vx] = buf[by * sht->bxsize + bx];
                    }
                }
            }
        } else {
            /* 1字节型 */
            for (by = by0; by < by1; by++) {
                vy = sht->vy0 + by;
                for (bx = bx0; bx < bx1; bx++) {
                    vx = sht->vx0 + bx;
                    if (map[vy * ctl->xsize + vx] == sid) {
                        vram[vy * ctl->xsize + vx] = buf[by * sht->bxsize + bx];
                    }
                }
            }
        }
    }
    return;
}

void sheet_updown(sheet_t* sht, int height)
{
    shtctl_t* ctl = sht->ctl;
    int h, old = sht->height; /* 备份层高 */

    /* 修正层高 */
    if (height > ctl->top + 1) {
        height = ctl->top + 1;
    }
    if (height < -1) {
        height = -1;
    }
    sht->height = height; /* 设置层高 */

    /* 重新排列sheets[] */
    if (old > height) { /* 比以前低 */
        if (height >= 0) {
            /* 中间图层上升 */
            for (h = old; h > height; h--) {
                ctl->sheets[h] = ctl->sheets[h - 1];
                ctl->sheets[h]->height = h;
            }
            ctl->sheets[height] = sht;
            sheet_refreshmap(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height + 1);
            sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height + 1, old);
        } else { /* 隐藏 */
            if (ctl->top > old) {
                /* 上面图层下降 */
                for (h = old; h < ctl->top; h++) {
                    ctl->sheets[h] = ctl->sheets[h + 1];
                    ctl->sheets[h]->height = h;
                }
            }
            ctl->top--; /* 显示中的图层减少，最高层下降 */
            sheet_refreshmap(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, 0);
            sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, 0, old - 1);
        }
    } else if (old < height) { /* 比以前高 */
        if (old >= 0) {
            /* 中间图层下降 */
            for (h = old; h < height; h++) {
                ctl->sheets[h] = ctl->sheets[h + 1];
                ctl->sheets[h]->height = h;
            }
            ctl->sheets[height] = sht;
        } else { /* 显示 */
            /* 上面图层上升 */
            for (h = ctl->top; h >= height; h--) {
                ctl->sheets[h + 1] = ctl->sheets[h];
                ctl->sheets[h + 1]->height = h + 1;
            }
            ctl->sheets[height] = sht;
            ctl->top++; /* 显示中的图层增加，最高层上升 */
        }
        sheet_refreshmap(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height);
        sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height, height);
    }
    return;
}

void sheet_refresh(sheet_t* sht, int bx0, int by0, int bx1, int by1)
{
    if (sht->height >= 0) { /* 如果可视则刷新画面 */
        sheet_refreshsub(sht->ctl, sht->vx0 + bx0, sht->vy0 + by0, sht->vx0 + bx1, sht->vy0 + by1, sht->height, sht->height);
    }
    return;
}

void sheet_slide(sheet_t* sht, int vx0, int vy0)
{
    shtctl_t* ctl = sht->ctl;
    int old_vx0 = sht->vx0, old_vy0 = sht->vy0;
    sht->vx0 = vx0;
    sht->vy0 = vy0;
    if (sht->height >= 0) { /* 如果可视则刷新画面 */
        sheet_refreshmap(ctl, old_vx0, old_vy0, old_vx0 + sht->bxsize, old_vy0 + sht->bysize, 0);
        sheet_refreshmap(ctl, vx0, vy0, vx0 + sht->bxsize, vy0 + sht->bysize, sht->height);
        sheet_refreshsub(ctl, old_vx0, old_vy0, old_vx0 + sht->bxsize, old_vy0 + sht->bysize, 0, sht->height - 1);
        sheet_refreshsub(ctl, vx0, vy0, vx0 + sht->bxsize, vy0 + sht->bysize, sht->height, sht->height);
    }
    return;
}

void sheet_free(sheet_t* sht)
{
    if (sht->height >= 0) {
        sheet_updown(sht, -1); /* 如果可视，先隐藏起来 */
    }
    sht->flags = 0; /* 标记为未使用 */
    return;
}
