// libdwcutil's step.c (Black's ov016 0x021d0040-0x021d0b44): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

// The row of the five objects that slide in and out (the prebuilt's MEMSTEP)
typedef struct MemStep {
    void *cell[5];
    void *task;
    u8 end; // bool
} MemStep;

typedef struct tagCOORD {
    u16 x;
    u16 y;
} COORD;

// The prebuilt's File, CELL_IDX and PL_CELL (each step's cells and palettes), C_POS (where the five objects sit),
// MemStep and FILE_NAME: the screen that taskChange loads, the tables, the row and each step's screen
static void *data_ov016_021de64c;
static const u8 data_ov016_021da914[3][5] = { { 1, 0, 5, 0, 6 }, { 4, 0, 2, 0, 6 }, { 4, 0, 5, 0, 3 } };
static const u8 data_ov016_021da923[3][5] = { { 2, 1, 3, 1, 3 }, { 5, 1, 4, 1, 5 }, { 7, 1, 7, 1, 6 } };
static const COORD data_ov016_021da932[5] = { { 0x20, 0x21 }, { 0x50, 0x30 }, { 0x68, 0x21 }, { 0x98, 0x30 }, { 0xb0, 0x21 } };
static MemStep *data_ov016_021de650 = NULL;
void taskBegin2(s32 param_1);
void taskBegin3(s32 param_1);
void taskBegin4(s32 param_1);
void taskEnd2(s32 param_1);
void taskEnd3_step(s32 param_1);
void taskEnd4(u32 param_1);
void func_ov016_021d0558(u32 param_1);
void taskBegin0(s32 param_1);
void taskEnd0(s32 param_1);
void taskBegin1(s32 param_1);
void taskEnd1_step(s32 param_1);
void taskChange(u32 task);

static s8 *data_ov016_021db950[5] = {
    "char/jtStep1.nsc.l",
    "char/jtStep2.nsc.l",
    "char/jtStep3.nsc.l",
    "char/jtOption.nsc.l",
    "char/jtTop.nsc.l",
};

void DWCi_STEPlBegin(s32 step) {
    s32 i;

    data_ov016_021de650 = DWCi_HEAPlAllocEx(sizeof(MemStep), 4);
    for (i = 0; i < 5; i++) {
        data_ov016_021de650->cell[i] = DWCi_CEINlSetExCell(1, data_ov016_021da914[step][i], 1);
        DWCi_CELLlSetPriority(data_ov016_021de650->cell[i], -1, 0);
        func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, -42, data_ov016_021da932[i].y);
        DWCi_CELLlSetPalette(data_ov016_021de650->cell[i], -1, 0, data_ov016_021da923[step][i]);
    }
    data_ov016_021de650->task = DWCi_TSKlForm(0, (s32)taskBegin0, NULL, 0x78);
    DWCi_STEPlChangeScr(step);
    DWCi_SNDlPlay(0xd);
}

void taskBegin0(s32 task) {
    s32 x;
    s32 y;
    s32 i;

    DWCi_CELLlGetPosition(data_ov016_021de650->cell[0], 0, (u32 *)&x, (u32 *)&y);
    x += 8;
    if (x < 0x20 || x > 0x100) {
        for (i = 0; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
    } else {
        x = 0x20;
        for (i = 0; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
        DWCi_TSKlChangeFunc((void *)task, (u32)taskBegin1);
    }
}

void taskBegin1(s32 task) {
    s32 x;
    s32 y;
    s32 i;

    DWCi_CELLlGetPosition(data_ov016_021de650->cell[1], 0, (u32 *)&x, (u32 *)&y);
    x += 8;
    if (x < 0x50 || x > 0x100) {
        for (i = 1; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
    } else {
        x = 0x50;
        for (i = 1; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
        DWCi_TSKlChangeFunc((void *)task, (u32)taskBegin2);
    }
}

void taskBegin2(s32 task) {
    s32 x;
    s32 y;
    s32 i;

    DWCi_CELLlGetPosition(data_ov016_021de650->cell[2], 0, (u32 *)&x, (u32 *)&y);
    x += 8;
    if (x < 0x68 || x > 0x100) {
        for (i = 2; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
    } else {
        x = 0x68;
        for (i = 2; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
        DWCi_TSKlChangeFunc((void *)task, (u32)taskBegin3);
    }
}

void taskBegin3(s32 task) {
    s32 x;
    s32 y;
    s32 i;

    DWCi_CELLlGetPosition(data_ov016_021de650->cell[3], 0, (u32 *)&x, (u32 *)&y);
    x += 8;
    if (x < 0x98 || x > 0x100) {
        for (i = 3; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
    } else {
        x = 0x98;
        for (i = 3; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
        DWCi_TSKlChangeFunc((void *)task, (u32)taskBegin4);
    }
}

void taskBegin4(s32 task) {
    s32 x;
    s32 y;

    DWCi_CELLlGetPosition(data_ov016_021de650->cell[4], 0, (u32 *)&x, (u32 *)&y);
    x += 8;
    if (x < 0xb0 || x > 0x100) {
        func_ov016_021d5118((s32)data_ov016_021de650->cell[4], -1, x, data_ov016_021da932[4].y);
    } else {
        x = 0xb0;
        func_ov016_021d5118((s32)data_ov016_021de650->cell[4], -1, x, data_ov016_021da932[4].y);
        DWCi_TSKlChangeFunc((void *)task, (u32)func_ov016_021d0558);
    }
}

void func_ov016_021d0558(u32 task) {
    DWCi_TSKlDelete(0, task);
    data_ov016_021de650->task = NULL;
}

void func_ov016_021d0580(void) {
    data_ov016_021de650->end = TRUE;
    data_ov016_021de650->task = DWCi_TSKlForm(0, (s32)taskEnd0, NULL, 0x78);
}

void DWCi_STEPlChange(s32 step) {
    long i;

    for (i = 0; i < 5; i += 2) {
        func_ov016_021d52d4(1, data_ov016_021da914[step][i],
                (void *)DWCi_CELLlGetObj(data_ov016_021de650->cell[i], 0));
        DWCi_CELLlSetPriority(data_ov016_021de650->cell[i], -1, 0);
        func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, data_ov016_021da932[i].x,
                data_ov016_021da932[i].y);
        DWCi_CELLlSetPalette(data_ov016_021de650->cell[i], -1, 0, data_ov016_021da923[step][i]);
    }
    DWCi_STEPlChangeScr(step);
}

BOOL DWCi_STEPlGet(void) {
    if (data_ov016_021de650->task != NULL) {
        return TRUE;
    }
    return FALSE;
}

void taskEnd0(s32 task) {
    s32 x;
    s32 y;

    DWCi_CELLlGetPosition(data_ov016_021de650->cell[4], 0, (u32 *)&x, (u32 *)&y);
    x -= 8;
    if (x > 0x98) {
        func_ov016_021d5118((s32)data_ov016_021de650->cell[4], -1, x, data_ov016_021da932[4].y);
    } else {
        x = 0x98;
        func_ov016_021d5118((s32)data_ov016_021de650->cell[4], -1, x, data_ov016_021da932[4].y);
        DWCi_TSKlChangeFunc((void *)task, (u32)taskEnd1_step);
    }
}

void taskEnd1_step(s32 task) {
    s32 x;
    s32 y;
    s32 i;

    DWCi_CELLlGetPosition(data_ov016_021de650->cell[3], 0, (u32 *)&x, (u32 *)&y);
    x -= 8;
    if (x > 0x68) {
        for (i = 3; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
    } else {
        x = 0x68;
        for (i = 3; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
        DWCi_TSKlChangeFunc((void *)task, (u32)taskEnd2);
    }
}

void taskEnd2(s32 task) {
    s32 x;
    s32 y;
    s32 i;

    DWCi_CELLlGetPosition(data_ov016_021de650->cell[2], 0, (u32 *)&x, (u32 *)&y);
    x -= 8;
    if (x > 0x50) {
        for (i = 2; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
    } else {
        x = 0x50;
        for (i = 2; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
        DWCi_TSKlChangeFunc((void *)task, (u32)taskEnd3_step);
    }
}

void taskEnd3_step(s32 task) {
    s32 x;
    s32 y;
    s32 i;

    DWCi_CELLlGetPosition(data_ov016_021de650->cell[1], 0, (u32 *)&x, (u32 *)&y);
    x -= 8;
    if (x > 0x20) {
        for (i = 1; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
    } else {
        x = 0x20;
        for (i = 1; i < 5; i++) {
            func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
        }
        DWCi_TSKlChangeFunc((void *)task, (u32)taskEnd4);
    }
}

void taskEnd4(u32 task) {
    s32 x;
    s32 y;
    s32 i;

    DWCi_CELLlGetPosition(data_ov016_021de650->cell[0], 0, (u32 *)&x, (u32 *)&y);
    x -= 8;
    for (i = 0; i < 5; i++) {
        func_ov016_021d5118((s32)data_ov016_021de650->cell[i], -1, x, data_ov016_021da932[i].y);
    }
    if (x <= 0x1d6 && x >= 0x100) {
        DWCi_TSKlDelete(0, task);
        for (i = 0; i < 5; i++) {
            DWCi_CELLlDelete((s32 *)data_ov016_021de650->cell[i]);
        }
        DWCi_HEAPlFree_((u32 *)&data_ov016_021de650);
    }
}

BOOL DWCi_STEPlIsEnd(void) {
    if (data_ov016_021de650 == NULL) {
        return TRUE;
    }
    if (!data_ov016_021de650->end) {
        return TRUE;
    }
    return FALSE;
}

void DWCi_STEPlChangeScr(s32 step) {
    data_ov016_021de64c = DWCi_ARClReadEx((s8 *)DWCi_Char(data_ov016_021db950[step]), NULL, 4);
    DWCi_TSKlForm(1, (s32)taskChange, NULL, 0x78);
}

void taskChange(u32 task) {
    DC_FlushRange(data_ov016_021de64c, 0x600);
    GXS_LoadBG1Scr(data_ov016_021de64c, 0, 0x600);
    DWCi_ARClRelease((u32)data_ov016_021de64c);
    DWCi_TSKlDelete(1, task);
}
