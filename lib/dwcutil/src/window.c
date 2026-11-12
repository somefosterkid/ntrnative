// libdwcutil's window.c (Black's ov016 0x021d1208-0x021d1d04): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

typedef struct tagCOORD {
    u16 x;
    u16 y;
} COORD;

static s32 data_ov016_021de658;
static const COORD data_ov016_021da99c = { 8, 8 };  // C_CHAR (the prebuilt's name): where the text starts in the window
static const u8 data_ov016_021da9a6[6] = { 2, 1, 2, 1, 2, 0 };
static const u8 data_ov016_021da9a0[6] = { 0, 0, 0x46, 0x46, 0x4f, 0x2f };
static const u16 data_ov016_021da9ac[4] = { 4, 0x1d, 0xfc, 0x44 };
static const u8 data_ov016_021da9b4[5][2] = { { 0, 1 }, { 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 } };
static const s8 data_ov016_021da9be[5][2] = { { 1, 0 }, { 0, -1 }, { 1, 0 }, { 0, -1 }, { 1, 0 } };
static const u8 data_ov016_021da9c8[5][2] = { { 0x1b, 0x19 }, { 0x57, 0 }, { 0x23, 0x1d }, { 0x59, 0 }, { 0x23, 0x1d } };
static const s16 data_ov016_021da9d2[5][2] = { { 0x6c, 0x10 }, { 0x6c, 0x10 }, { 0x78, 0x10 }, { 0x78, 0x10 }, { 0x78, 0x10 } };
static const u16 data_ov016_021da9e6[6][2] = {
    { 0xd8, 0x50 }, { 0xd8, 0x50 }, { 0xe6, 0x4f }, { 0xe6, 0x4f }, { 0xe6, 0x48 }, { 0xda, 0x5c },
};
static const u16 data_ov016_021da9fe[6][2] = {
    { 0x0b, 0x27 }, { 0x0b, 0x27 }, { 0x04, 0x4c }, { 0x04, 0x4c }, { 0x04, 0x54 }, { 0x0b, 0x27 },
};
static const u16 data_ov016_021daa16[6][2] = {
    { 0xea, 0x72 }, { 0xea, 0x72 }, { 0xf8, 0x70 }, { 0xf8, 0x70 }, { 0x64, 0x70 }, { 0xea, 0x72 },
};
static const u16 data_ov016_021daa2e[5][2][2] = {
    { { 0x10, 0x84 }, { 0x84, 0x84 } },
    { { 0x10, 0x84 }, { 0x84, 0x84 } },
    { { 0x09, 0xa7 }, { 0x83, 0xa7 } },
    { { 0x09, 0xa7 }, { 0x83, 0xa7 } },
    { { 0x09, 0xa7 }, { 0x83, 0xa7 } },
};
void taskMove(u32 param_1);
void taskEnd1_window(void *param_1);
void taskEnd3_window(u32 param_1);
void taskEnd_window(void *param_1);
void disp_window(u32 param_1);
void taskMain1(u32 param_1);
void taskMain_window(void);
void func_ov016_021d1c04(void *param_1);
void taskV(void *param_1);
void dispCell(s32 param_1, s32 param_2);
void pushButton(s32 param_1);


void DWCi_WINlInitEx(u32 msg, s32 type, u8 dark, s32 n, s32 num) {
    u8 tbl[6] = { 1, 1, 0, 0, 1, 1 };
    u16 *text;
    s32 canvas_size;
    s32 i;

    text = DWCi_MSGlGetEx(data_ov016_021de598, msg, n, num);
    data_ov016_021de658 = (s32)DWCi_HEAPlAllocEx(0x20, 4);
    *(u8 *)(data_ov016_021de658 + 0x1c) = type;
    *(s8 *)(data_ov016_021de658 + 0x1b) = -2;
    *(u8 *)(data_ov016_021de658 + 0x1e) = dark;
    G2_SetBlendBrightness(0x1f, 0);
    *(s32 **)data_ov016_021de658 = DWCi_CEINlSetExCell(0, data_ov016_021da9a0[type], 0);
    func_ov016_021d5118(*(s32 *)data_ov016_021de658, -1, 0x100, 0);
    DWCi_CELLlSetPriority(*(void **)data_ov016_021de658, -1, 0);
    for (i = 0; i < data_ov016_021da9a6[type]; i++) {
        *(s32 **)(data_ov016_021de658 + i * 4 + 8) = DWCi_CEINlSetExCell(0, data_ov016_021da9c8[type][i], 0);
        func_ov016_021d5118(*(s32 *)(data_ov016_021de658 + i * 4 + 8), -1, 0x100, 0);
        DWCi_CELLlSetPriority(*(void **)(data_ov016_021de658 + i * 4 + 8), -1, 0);
    }
    *(NNSG2dCharCanvas **)(data_ov016_021de658 + 0x10) = DWCi_FNTlFormEx(0, 0x20, 0xc, 1, &canvas_size, 0);
    *(NNSG2dCharCanvas **)(data_ov016_021de658 + 4) = DWCi_CELLlForm(0, canvas_size, 0);
    DWCi_FNTlDrawRect(*(NNSG2dCharCanvas **)(data_ov016_021de658 + 0x10), 0, 0, data_ov016_021da9e6[type][0],
                        data_ov016_021da9e6[type][1], 2, DWCi_CMNlFontFlag(), text);
    func_ov016_021d5f28(*(void **)(data_ov016_021de658 + 0x10), 0x100, 0, *(s32 *)(data_ov016_021de658 + 4), 0);
    DWCi_GXlSetWndPlane(0, 0, 0x1f, 0);
    DWCi_GXlSetWndPlane(0, 1, 0x1f, tbl[*(u8 *)(data_ov016_021de658 + 0x1c)]);
    DWCi_GXlSetWndPlane(0, 3, 0x1f, 1);
    DWCi_GXlSetWndPosition(0, 1, (u16 *)data_ov016_021da9ac);
    disp_window(0xc0);
    reg_GX_DISPCNT = (reg_GX_DISPCNT & ~0xe000) | 0x6000;
    if (dark != 0) {
        *(s32 **)(data_ov016_021de658 + 0x14) = DWCi_TSKlForm(1, (s32)taskV, 0, 0x78);
    } else {
        *(s32 **)(data_ov016_021de658 + 0x14) = DWCi_TSKlForm(1, (s32)taskMove, 0, 0x78);
    }
}

void DWCi_WINlEnd(void) {
    DWCi_TSKlDelete(0, *(u32 *)(data_ov016_021de658 + 0x14));
    *(s32 **)(data_ov016_021de658 + 0x14) = DWCi_TSKlForm(1, (s32)taskEnd_window, 0, 0x78);
}

s32 DWCi_WINlGet(void) {
    return (s32)*(s8 *)(data_ov016_021de658 + 0x1b);
}

BOOL DWCi_WINlIsInit(void) {
    return (u32)(data_ov016_021de658 != 0);
}

void taskV(void *task) {
    *(s8 *)(data_ov016_021de658 + 0x1a) = *(s8 *)(data_ov016_021de658 + 0x1a) + -1;
    G2_ChangeBlendBrightness((s32)*(s8 *)(data_ov016_021de658 + 0x1a));
    if (-0xc < *(s8 *)(data_ov016_021de658 + 0x1a)) {
        return;
    }
    DWCi_TSKlChangeFunc(task, (u32)taskMove);
}

void taskMove(u32 param_1) {
    u32 sp4;
    u32 sp0;
    u16 minY;
    u32 newY;

    DWCi_CELLlGetPosition(*(void **)data_ov016_021de658, 0, &sp4, &sp0);
    newY = sp0 - 0xC;
    sp0 = newY;
    minY = data_ov016_021da9fe[*(u8 *)((u8 *)data_ov016_021de658 + 0x1C)][1];
    if ((s32) newY > (s32) minY) {
        disp_window(newY);
        return;
    }
    disp_window((u32) minY);
    if (*(u8 *)((u8 *)data_ov016_021de658 + 0x1C) == 5) {
        *(s32 **)((u8 *)data_ov016_021de658 + 0x14) = DWCi_TSKlForm(0, (s32)taskMain1, 0, 0x78U);
    } else {
        *(s32 **)((u8 *)data_ov016_021de658 + 0x14) = DWCi_TSKlForm(0, (s32)taskMain_window, 0, 0x78U);
    }
    DWCi_TSKlDelete(1, param_1);
}

void taskMain_window(void) {
    u16 rect[4];
    s32 i;

    for (i = 0; i < data_ov016_021da9a6[*(u8 *)(data_ov016_021de658 + 0x1c)]; i++) {
        u8 type = *(u8 *)(data_ov016_021de658 + 0x1c);
        func_ov016_021d600c((s16 *)data_ov016_021daa2e[type][data_ov016_021da9b4[type][i]],
                            data_ov016_021da9d2[type], (s16 *)rect);
        if (DWCi_TPlCheckTrigger(rect)) {
            *(s8 *)(data_ov016_021de658 + 0x1b) = i;
            break;
        }
    }
    if (DWCi_IPTlCheckTrigger(1)) {
        *(s8 *)(data_ov016_021de658 + 0x1b) = data_ov016_021da9be[*(u8 *)(data_ov016_021de658 + 0x1c)][0];
    }
    if (DWCi_IPTlCheckTrigger(2)) {
        *(s8 *)(data_ov016_021de658 + 0x1b) = data_ov016_021da9be[*(u8 *)(data_ov016_021de658 + 0x1c)][1];
    }
    for (i = 0; i < data_ov016_021da9a6[*(u8 *)(data_ov016_021de658 + 0x1c)]; i++) {
        if (i == *(s8 *)(data_ov016_021de658 + 0x1b)) {
            pushButton(i);
            return;
        }
    }
    *(s8 *)(data_ov016_021de658 + 0x1b) = -1;
}


void taskMain1(u32 task) {
    *(s8 *)((u8 *)data_ov016_021de658 + 0x1B) = -1;
    *(u16 *)((u8 *)data_ov016_021de658 + 0x18) = (u16) (*(u16 *)((u8 *)data_ov016_021de658 + 0x18) + 1);
    if ((u32) *(u16 *)((u8 *)data_ov016_021de658 + 0x18) < 0x78U) {
        return;
    }
    DWCi_TSKlDelete(0, task);
    *(s32 **)((u8 *)data_ov016_021de658 + 0x14) = DWCi_TSKlForm(1, (s32) taskEnd_window, 0, 0x78U);
}

void disp_window(u32 y) {
    u16 rect[4];
    s32 i;
    s32 top;
    s32 bottom;

    func_ov016_021d5118(*(s32 *)data_ov016_021de658, -1, data_ov016_021da9fe[*(u8 *)(data_ov016_021de658 + 0x1c)][0], y);
    func_ov016_021d5118(*(s32 *)(data_ov016_021de658 + 4), -1, data_ov016_021da9fe[*(u8 *)(data_ov016_021de658 + 0x1c)][0] + data_ov016_021da99c.x, y + data_ov016_021da99c.y);
    dispCell(*(s32 *)data_ov016_021de658, y);
    dispCell(*(s32 *)(data_ov016_021de658 + 4), y);
    for (i = 0; i < data_ov016_021da9a6[*(u8 *)(data_ov016_021de658 + 0x1c)]; i++) {
        u8 k = data_ov016_021da9b4[*(u8 *)(data_ov016_021de658 + 0x1c)][i];

        func_ov016_021d5118(*(s32 *)(data_ov016_021de658 + i * 4 + 8), -1, data_ov016_021daa2e[*(u8 *)(data_ov016_021de658 + 0x1c)][k][0],
                            y + data_ov016_021daa2e[*(u8 *)(data_ov016_021de658 + 0x1c)][k][1] - data_ov016_021da9fe[*(u8 *)(data_ov016_021de658 + 0x1c)][1]);
        dispCell(*(s32 *)(data_ov016_021de658 + i * 4 + 8), y);
    }
    top = (u8)y;
    if (top >= 0xc0) {
        top = bottom = 0;
    } else {
        bottom = top + data_ov016_021daa16[*(u8 *)(data_ov016_021de658 + 0x1c)][1];
    }
    if (bottom > 0xc0) {
        bottom = 0xc0;
    }
    DWCi_InputRect(data_ov016_021da9fe[*(u8 *)(data_ov016_021de658 + 0x1c)][0], top,
                   data_ov016_021da9fe[*(u8 *)(data_ov016_021de658 + 0x1c)][0] + data_ov016_021daa16[*(u8 *)(data_ov016_021de658 + 0x1c)][0],
                   bottom, rect);
    DWCi_GXlSetWndPosition(0, 0, rect);
}


void dispCell(s32 cell, s32 y) {
    u32 sp4;
    u32 sp0;
    s32 result;
    s32 i;
    u32 val;

    result = (s32) DWCi_CELLlGetObjNum(cell);
    i = 0;
    if (result <= 0) {
        return;
    }
    do {
        DWCi_CELLlGetPosition((void *)cell, i, &sp4, &sp0);
        if (((s32) sp0 >= y) && ((s32) sp0 < 0xC0)) {
            val = 0;
        } else {
            val = 0x200;
        }
        DWCi_CELLlSetEffect((void *)cell, i, val, 0);
        i += 1;
    } while (i < result);
}

void pushButton(s32 idx) {
    s32 obj;
    u8 type;

    obj = DWCi_CELLlGetObj(*(void **)(data_ov016_021de658 + idx * 4 + 8), 0);
    func_ov016_021d52d4(0, data_ov016_021da9c8[*(u8 *)(data_ov016_021de658 + 0x1c)][idx] + 1, (void *)obj);
    type = *(u8 *)(data_ov016_021de658 + 0x1c);
    func_ov016_021d5118(*(s32 *)(data_ov016_021de658 + idx * 4 + 8), -1,
                        data_ov016_021daa2e[type][data_ov016_021da9b4[type][idx]][0],
                        data_ov016_021daa2e[type][data_ov016_021da9b4[type][idx]][1]);
    DWCi_CELLlSetPriority(*(void **)(data_ov016_021de658 + idx * 4 + 8), -1, 0);
}

void taskEnd_window(void *task) {
    *(u8 *)(data_ov016_021de658 + 0x1d) = *(u8 *)(data_ov016_021de658 + 0x1d) + 1;
    if (*(u8 *)(data_ov016_021de658 + 0x1d) < 8) {
        return;
    }
    DWCi_TSKlChangeFunc(task, (u32)taskEnd1_window);
}

void taskEnd1_window(void *param_1) {
    u32 x;
    u32 y;

    DWCi_CELLlGetPosition(*(void **)data_ov016_021de658, 0, &x, &y);
    y = y + 0xc;
    disp_window(y);
    if ((s32)y < 0xc0) {
        return;
    }
    if (*(u8 *)(0x1e + data_ov016_021de658)) {
        DWCi_TSKlChangeFunc(param_1, (u32)func_ov016_021d1c04);
        return;
    }
    DWCi_TSKlChangeFunc(param_1, (u32)taskEnd3_window);
}

void func_ov016_021d1c04(void *param_1) {
    *(s8 *)(data_ov016_021de658 + 0x1a) = *(s8 *)(data_ov016_021de658 + 0x1a) + 1;
    G2_ChangeBlendBrightness((s32)*(s8 *)(data_ov016_021de658 + 0x1a));
    if (*(s8 *)(data_ov016_021de658 + 0x1a) < 0) {
        return;
    }
    DWCi_TSKlChangeFunc(param_1, (u32)taskEnd3_window);
}

void taskEnd3_window(u32 task) {
    s32 i;

    reg_GX_DISPCNT = reg_GX_DISPCNT & 0xffff1fff;
    DWCi_CELLlDelete(*(s32 **)data_ov016_021de658);
    DWCi_CELLlDelete(*(s32 **)(data_ov016_021de658 + 4));
    for (i = 0; i < data_ov016_021da9a6[*(u8 *)(data_ov016_021de658 + 0x1c)]; i++) {
        s32 *p = *(s32 **)(data_ov016_021de658 + i * 4 + 8);
        if (p != NULL) {
            DWCi_CELLlDelete(p);
        }
    }
    DWCi_FNTlDelete(*(s32 *)(data_ov016_021de658 + 0x10));
    DWCi_TSKlDelete(1, task);
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de658);
}

