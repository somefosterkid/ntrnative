#include "math_util.h"
s32 ov96_021EB03C(s32, s32, s32, s32 *, s32 *);
s32 ov96_021EB0A4(s32, s32, s32, s32 *, s32 *);
void *ov96_02208FB8(void *);
s32 ov96_022090A8(void *, u16, u16);
s32 ov96_022090D8(void *);

typedef struct { u8 *obj; u16 x; u16 y; } UnkPos; /* invented */
typedef struct { s32 a; s32 b; s32 c; s32 d; s16 score; s16 idx; } UnkCell; /* invented */
typedef struct { s32 active; s32 pad[3]; u16 x; u16 y; } UnkItem; /* invented */
typedef struct {
    UnkPos pos[4];
    VecFx32 a[20];
    VecFx32 b[10];
    UnkItem *items;
    u8 pad18C[4];
    UnkCell cells[48];
    u8 pad550[8];
    u8 count;
} UnkMain; /* invented */

void ov96_02208BB8(void *arg0, u32 arg1) {
    VecFx32 spD0[20];
    VecFx32 sp58[10];
    UnkPos sp38[4];
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    UnkPos *self;
    UnkItem *items;
    u8 *obj;
    UnkMain *p;
    UnkItem *item;
    UnkCell *cell;
    u8 *ret;
    s32 i;
    s32 k;
    s32 j;
    s32 col;
    s32 row;

    p = arg0;
    obj = p->pos[arg1].obj;
    if (obj[0xAB] != 0 || obj[0xA6] == 1 || obj[0xA4] != 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        u8 *o;
        sp38[i] = p->pos[i];
        o = sp38[i].obj;
        ov96_021EB0A4(*(s32 *)(o + o[0xB1] * 4), *(s32 *)(o + 0x58) / 4096, *(s32 *)(o + 0x5C) / 4096, &sp34, &sp30);
        ov96_021EB03C(*(s32 *)(o + o[0xB1] * 4), sp34 << 12, sp30 << 12, &sp2C, &sp28);
        sp38[i].x = sp2C / 4096;
        sp38[i].y = sp28 / 4096;
    }
    self = &sp38[arg1];
    for (i = 0; i < 20; i++) {
        spD0[i] = p->a[i];
    }
    for (i = 0; i < 10; i++) {
        sp58[i] = p->b[i];
    }
    items = p->items;
    for (i = 0; i < 48; i++) {
        p->cells[i].idx = i;
        col = (i % 6) * 32;
        p->cells[i].a = self->x - 0x50 + col;
        row = (i / 6) * 32;
        p->cells[i].b = self->y - 0x30 + row;
        p->cells[i].c = self->x - 0x60 + col + LCRandom() % 32;
        p->cells[i].d = self->y - 0x80 + row + LCRandom() % 32;
    }
    for (i = 0; i < 48; i++) {
        p->cells[i].score = 0;
    }
    for (j = 0; j < 2; j++) {
        for (k = 0; k < 4; k++) {
            p->cells[(u8)(k + 19)].score += 2;
        }
    }
    for (i = 0, cell = p->cells; i < 48; i++) {
        item = items;
        for (k = 0; k < 5; k++) {
            if (item->active != 0 && ov96_022090A8(cell, item->x, item->y) != 0) {
                cell->score += 7;
            }
            item++;
        }
        for (k = 0; k < 4; k++) {
            if (k != arg1 && ov96_022090A8(cell, sp38[k].x, sp38[k].y) != 0) {
                cell->score -= 3;
            }
        }
        for (k = 0; k < 20; k++) {
            if (spD0[k].x == 0) {
                break;
            }
            if (ov96_022090A8(cell, spD0[k].y, spD0[k].z) != 0) {
                cell->score -= 2;
            }
        }
        for (k = 0; k < 10; k++) {
            if (sp58[k].x == 0) {
                break;
            }
            if (ov96_022090A8(cell, sp58[k].y, sp58[k].z) != 0) {
                cell->score += 2;
            }
        }
        if (ov96_022090D8(cell) == 0) {
            cell->score -= 100;
        }
        cell++;
    }
    if ((u16)(*(s32 *)(obj + 0x5C) / 4096) > 0x100) {
        {
            u8 *q = (u8 *)p;
            for (k = 0; k < 6; k++) {
                s16 *s = (s16 *)(q + 0x1A0);
                q += 0x14;
                s[300] += 3; /* p->cells[30 + k].score */
            }
        }
    } else if ((u16)(*(s32 *)(obj + 0x5C) / 4096) < 0x100) {
        for (k = 0; k < 6; k++) {
            p->cells[k].score += 3;
        }
    }
    if (obj[0xAA] != 0) {
        {
            u8 *q = (u8 *)p;
            for (k = 0; k < 6; k++) {
                s16 *s = (s16 *)(q + 0x1A0);
                q += 0x14;
                s[300] += obj[0xAA]; /* p->cells[30 + k].score */
            }
        }
        if ((u16)(*(s32 *)(obj + 0x58) / 4096) > 0x100) {
            {
            u8 *q = (u8 *)p;
            for (k = 0; k < 3; k++) {
                s16 *s = (s16 *)(q + 0x1A0);
                q += 0x14;
                s[300] += 2; /* p->cells[30 + k].score */
            }
        }
        } else if ((u16)(*(s32 *)(obj + 0x58) / 4096) < 0x100) {
            {
            u8 *q = (u8 *)p;
            for (k = 0; k < 3; k++) {
                s16 *s = (s16 *)(q + 0x1A0);
                q += 0x14;
                s[330] += 2; /* p->cells[33 + k].score */
            }
        }
        }
    }
    ret = ov96_02208FB8(p);
    if (arg1 >= p->count) {
        ov96_021EB0A4(*(s32 *)(self->obj + self->obj[0xB1] * 4), self->x, self->y, &sp24, &sp20);
        *(s32 *)(self->obj + 0x7C) = sp24 << 12;
        *(s32 *)(self->obj + 0x80) = sp20 << 12;
        *(s32 *)(self->obj + 0x84) = 0;
        *(s32 *)(self->obj + 0x88) = *(s32 *)(ret + 8) << 12;
        *(s32 *)(self->obj + 0x8C) = *(s32 *)(ret + 0xC) << 12;
        *(s32 *)(self->obj + 0x90) = 0;
        self->obj[0xA5] = 1;
    }
}
