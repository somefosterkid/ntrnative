// libdwcutil's cell.c (Black's ov016 0x021d4cf4-0x021d5280): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

static void *data_ov016_021de670;
void DWCi_LSTlDelete(u32 param_1);

typedef struct CellNode {
    struct CellNode *prev;
    struct CellNode *next;
    u8 *oam;
    u8 num;
} CellNode;

typedef struct CellGroup {
    u8 oam[0x200];
    CellNode first;
    CellNode last;
    u32 list;
    u16 *queue;
} CellGroup;

void DWCi_CELLlInit(void) {
    s32 i;

    data_ov016_021de670 = DWCi_HEAPlAllocEx(0x450, 4);
    for (i = 0; i < 2; i++) {
        ((CellGroup *)data_ov016_021de670)[i].queue =
            (u16 *)DWCi_QUElFormEx(0x20, (s32)&((CellGroup *)data_ov016_021de670)[i], 0x10);
        ((CellGroup *)data_ov016_021de670)[i].list = DWCi_LSTlForm();
        ((CellGroup *)data_ov016_021de670)[i].first.oam = (u8 *)DWCi_OBJlGetOam(i, 0x40);
        ((CellGroup *)data_ov016_021de670)[i].last.oam = (u8 *)(DWCi_OBJlGetOam(i, 0x7f) + 8);
        DWCi_LSTlPushFront(((CellGroup *)data_ov016_021de670)[i].list,
                           (s32 *)&((CellGroup *)data_ov016_021de670)[i].first);
        DWCi_LSTlPushBack(((CellGroup *)data_ov016_021de670)[i].list,
                          (s32 *)&((CellGroup *)data_ov016_021de670)[i].last);
    }
}

void DWCi_CELLlEnd(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        DWCi_LSTlDelete(*(u32 *)((u8 *)data_ov016_021de670 + i * 0x228 + 0x220));
        DWCi_QUElDelete(*(u32 *)((u8 *)data_ov016_021de670 + i * 0x228 + 0x224));
    }
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de670);
}

NNSG2dCharCanvas *DWCi_CELLlForm(s32 lcd, s32 num, s32 head) {
    CellNode *node;
    CellNode *cur;
    CellNode *adj;
    u8 *pos;
    OSIrqMask mask;

    node = (CellNode *)DWCi_QUElPopBack(((CellGroup *)data_ov016_021de670)[lcd].queue);
    mask = OS_DisableIrqMask(1);
    if (head != 0) {
        cur = &((CellGroup *)data_ov016_021de670)[lcd].first;
        if (cur != &((CellGroup *)data_ov016_021de670)[lcd].last) {
            do {
                adj = cur->next;
                pos = cur->oam + cur->num * 8;
                if (pos + num * 8 <= adj->oam) {
                    node->oam = pos;
                    DWCi_LSTlInsert((s32 *)adj, (s32 *)node);
                    break;
                }
                cur = adj;
            } while (cur != &((CellGroup *)data_ov016_021de670)[lcd].last);
        }
        if (cur == &((CellGroup *)data_ov016_021de670)[lcd].last) {
            OS_Terminate();
        }
    } else {
        cur = &((CellGroup *)data_ov016_021de670)[lcd].last;
        if (cur != &((CellGroup *)data_ov016_021de670)[lcd].first) {
            do {
                adj = cur->prev;
                pos = cur->oam - num * 8;
                if (pos >= adj->oam + adj->num * 8) {
                    node->oam = pos;
                    DWCi_LSTlInsert((s32 *)cur, (s32 *)node);
                    break;
                }
                cur = adj;
            } while (cur != &((CellGroup *)data_ov016_021de670)[lcd].first);
        }
        if (cur == &((CellGroup *)data_ov016_021de670)[lcd].first) {
            OS_Terminate();
        }
    }
    OS_EnableIrqMask(mask);
    node->num = num;
    return (NNSG2dCharCanvas *)node;
}


void DWCi_CELLlDelete(s32 *cell) {
    u32 *oam;
    s32 i;
    s32 which;

    oam = (u32 *)cell[2];
    which = 0;
    for (i = 0; i < *(u8 *)(cell + 3); i++) {
        *oam = (*oam & 0xC1FFFCFF) | 0x200;
        oam += 2;
    }
    DWCi_LSTlErase(cell);
    if ((u8 *)cell >= (u8 *)data_ov016_021de670 + 0x228) {
        which = 1;
    }
    DWCi_QUElPushBack(*(u16 **)((u8 *)data_ov016_021de670 + which * 0x228 + 0x224), (u32)cell);
}



s32 DWCi_CELLlGetObj(void *cell, s32 idx) {
    return *(s32 *)((u8 *)cell + 8) + idx * 8;
}

u8 DWCi_CELLlGetObjNum(s32 cell) {
    return *(u8 *)(cell + 0xc);
}

void DWCi_CELLlSetEffect(void *cell, s32 idx, u32 effect, s32 affine) {
    s32 val;
    int val2;
    s32 i;
    int val3;

    val = *(s32 *)((u8 *)cell + 8);
    if (idx >= 0) {
        val2 = val + idx * 8;
        if (effect != 0x100 && effect != 0x300) {
            i = *(s32 *)(val + idx * 8);
            *(s32 *)(val + idx * 8) = (i & 0xC1FFFCFF) | effect;
            return;
        }
        *(s32 *)(val + idx * 8) = (*(s32 *)val2 & 0xC1FFFCFF) | effect | (affine << 0x19);
        return;
    }
    i = 0;
    if ((s32)(*(u8 *)((u8 *)cell + 0xC)) <= 0) {
        return;
    }
    do {
        if (effect != 0x100 && effect != 0x300) {
            val3 = *(s32 *)(i * 8 + val) & 0xC1FFFCFF;
            *(s32 *)(val + i * 8) = val3 | effect;
        } else {
            val2 = *(s32 *)(val + i * 8) & 0xC1FFFCFF;
            *(s32 *)(val + i * 8) = val2 | effect | (affine << 0x19);
        }
        i += 1;
    } while (i < (s32)(*(u8 *)((u8 *)cell + 0xC)));
}

typedef struct CellInfo {
    void *prev;
    void *next;
    GXOamAttr *obj;
    u8 num;
} CellInfo;
void DWCi_CELLlSetPalette(void *cell, s32 idx, s32 mode, s32 plt) {
    CellInfo *c = (CellInfo *)cell;
    GXOamAttr *oam;
    s32 i;

    oam = c->obj;
    if (idx >= 0) {
        G2_SetOBJMode(&oam[idx], mode, plt);
        return;
    }
    for (i = 0; i < c->num; i++) {
        G2_SetOBJMode(&oam[i], mode, plt);
    }
}

typedef struct CellOam {
    u32 attr01;
    u16 attr2;
    u16 pad;
} CellOam;

static inline void CellGetPos(const CellOam *oam, u32 *x, u32 *y) {
    *x = (oam->attr01 & 0x1FF0000) >> 16;
    *y = oam->attr01 & 0xFF;
}

static inline void CellSetPos(CellOam *oam, u32 x, u32 y) {
    oam->attr01 = (oam->attr01 & 0xFE00FF00) | (y & 0xFF) | ((x & 0x1FF) << 16);
}

static inline void CellGetPosition(s32 cell, s32 idx, u32 *x, u32 *y) {
    CellOam *cells = *(CellOam **)(cell + 8);
    *x = (cells[idx].attr01 & 0x1FF0000) >> 16;
    *y = cells[idx].attr01 & 0xFF;
}
void func_ov016_021d5118(s32 cell, s32 idx, u32 x, u32 y) {
    CellInfo *c = (CellInfo *)cell;
    GXOamAttr *oam;
    s32 i;
    s32 dx;
    s32 dy;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;

    oam = c->obj;
    if (idx >= 0) {
        G2_SetOBJPosition(&oam[idx], x, y);
        return;
    }
    G2_GetOBJPosition(&oam[0], (u32 *)&x0, (u32 *)&y0);
    G2_SetOBJPosition(&oam[0], x, y);
    dx = x - x0;
    dy = y - y0;
    for (i = 1; i < c->num; i++) {
        G2_GetOBJPosition(&oam[i], (u32 *)&x1, (u32 *)&y1);
        G2_SetOBJPosition(&oam[i], x1 + dx, y1 + dy);
    }
}


void DWCi_CELLlSetPriority(void *cell, s32 idx, s32 priority) {
    s32 val;
    s32 i;
    void *ptr;
    void *ptr2;

    val = *(s32 *)((u8 *)cell + 8);
    if (idx >= 0) {
        ptr = (void *)(val + (idx * 8));
        *(u16 *)((u8 *)(u8 *)ptr + 4) = (u16) ((*(u16 *)((u8 *)(u8 *)ptr + 4) & ~0xC00) | (priority << 0xA));
        return;
    }
    i = 0;
    if ((s32) *(u8 *)((u8 *)cell + 0xC) <= 0) {
        return;
    }
    do {
        ptr2 = (void *)(val + (i * 8));
        i += 1;
        *(s16 *)((u8 *)(u8 *)ptr2 + 4) = (s16) (((*(u16 *)((u8 *)(u8 *)ptr2 + 4)) & ~0xC00) | (priority << 0xA));
    } while (i < (s32) *(u8 *)((u8 *)cell + 0xC));
}

void DWCi_CELLlGetPosition(void *cell, s32 idx, u32 *x, u32 *y) {
    s32 val;

    val = *(s32 *)((u8 *)cell + 8);
    *x = (*(u32 *)(val + idx * 8) & 0x1ff0000) >> 0x10;
    *y = *(u32 *)(val + idx * 8) & 0xff;
}
