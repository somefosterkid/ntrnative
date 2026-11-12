// libdwcutil's keyboard10.c (Black's ov016 0x021bf390-0x021c07a8): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

typedef struct tagCOORD {
    u16 x;
    u16 y;
} COORD;


void func_ov016_021c0528(void *param_1);
static s32 data_ov016_021de560;
// The prebuilt's names in the comments; defined in the places keyboard.c has its tables (the special keys' positions
// and a size before touchTriger, the cursor's moves before moveFocus), which gives Black's layout of MWCC's size sort
static const u8 data_ov016_021da360[2] = { 0x3c, 0x3d };  // CN_BTN
static const u8 data_ov016_021da35e[2] = { 0x37, 0x38 };  // CN_SKEY
static const u8 data_ov016_021da35c[2] = { 4, 5 };  // CL_BTN
static const u8 data_ov016_021da364[3] = { 0x12, 0x13, 0 };  // BTN_OUT
static const COORD data_ov016_021da36e = { 0x0c, 0x04 };  // S_CANV
static const COORD data_ov016_021da372 = { 0x1c, 0x14 };  // S_SKEY
static const COORD data_ov016_021da376 = { 0x78, 0x12 };  // S_BTN
static const u8 data_ov016_021da362[2] = { 2, 3 };  // CL_KEY
static const u8 data_ov016_021da367[3] = { 0x10, 0x11, 0 };  // SKEY_OUT
static const COORD data_ov016_021da382[2] = { { 0x04, 0xaa }, { 0x84, 0xaa } };  // C_BTN
static const COORD data_ov016_021da3b0[10] = {  // C_KEY
    { 0x52, 0x4c }, { 0x72, 0x4c }, { 0x92, 0x4c }, { 0x52, 0x63 }, { 0x72, 0x63 }, { 0x92, 0x63 }, { 0x52, 0x7a },
    { 0x72, 0x7a }, { 0x92, 0x7a }, { 0x52, 0x91 },
};
static const COORD data_ov016_021da3d8[14] = {  // C_FOCUS
    { 0x50, 0x4a }, { 0x70, 0x4a }, { 0x90, 0x4a }, { 0x50, 0x61 }, { 0x70, 0x61 }, { 0x90, 0x61 }, { 0x50, 0x78 },
    { 0x70, 0x78 }, { 0x90, 0x78 }, { 0x50, 0x8f }, { 0x70, 0x8f }, { 0x90, 0x8f }, { 0x02, 0xa8 }, { 0x82, 0xa8 },
};
static const u8 data_ov016_021da38a[11] = "7894561230";  // KEY_OUT
// KEY_CHR: "7894561230  " in 16-bit characters, written out (an L"" string's are 16 bits for MWCC and Windows, 32 for
// ARM clang and other hosts)
static const u16 data_ov016_021da396[13] = { '7', '8', '9', '4', '5', '6', '1', '2', '3', '0', ' ', ' ', 0 };
void disp_keyboard10(s32 line, s32 y);
void func_ov016_021c03f0(void);
void taskMain_keyboard10(void);
void touchTriger_keyboard10(void);
void touchRelease_keyboard10(void);
void touch_keyboard10(void);
void key_keyboard10(void);
void changeColor_keyboard10(s32 param_1, s32 param_2);
void taskStart1_keyboard10(void *param_1);
void taskStart2_keyboard10(s32 param_1);
void taskStart3_keyboard10(void *param_1);
void taskStart4_keyboard10(void *param_1);
void exchangeBack_keyboard10(s32 param_1);
void taskStart0_keyboard10(void *param_1);
void moveFocus_keyboard10(s32 param_1);
void taskEnd5_keyboard10(u32 param_1);
void func_ov016_021c0588(void *param_1);
void func_ov016_021d5f28(void *param_1, s32 param_2, s32 param_3, s32 param_4, s32 param_5);


NNSG2dCharCanvas *DWCi_FNTlFormEx(s32 param_1, s32 param_2, s32 param_3, s32 param_4, s32 *param_5, s32 param_6);
NNSG2dCharCanvas *DWCi_CELLlForm(s32 param_1, s32 param_2, s32 param_3);
typedef struct tagAREA {
    u16 l;
    u16 t;
    u16 w;
    u16 h;
} tagAREA;

void func_ov016_021c05e0(void *param_1);
void func_ov016_021c0690(void *param_1);
void func_ov016_021c0638(void *param_1);

typedef struct Ov016Keyboard10Work {
    NNSG2dCharCanvas *canvas[4];
    void *keyCell[10];
    void *subCell[2];
    s32 *bg[2];
    s32 *bgCell[4];
    s32 *bgBase;
    s32 *task;
    u8 unk_60;
    s8 unk_61;
    u8 unk_62;
    u8 unk_63;
    u8 unk_64;
    u8 unk_65;
    u8 unk_66;
    u8 unk_67;
    u8 unk_68;
} Ov016Keyboard10Work;

void DWCi_KB10lInit(void) {
    tagAREA a = {0, 1};
    s32 canvasSize;
    u16 str[2];
    s32 i;
    s32 j;
    s32 k;
    s32 idx;
    s32 n;
    NNSG2dCharCanvas *canvas;

    a.w = 0x1c;
    a.h = 0x14;
    data_ov016_021de560 = (s32)DWCi_HEAPlAllocEx(0x6c, 4);
    *(u8 *)(data_ov016_021de560 + 0x60) = 0x1f;
    *(u8 *)(data_ov016_021de560 + 0x63) = 0;
    *(u8 *)(data_ov016_021de560 + 0x66) = 1;
    *(u8 *)(data_ov016_021de560 + 0x67) = 1;
    *(u8 *)(data_ov016_021de560 + 0x68) = 1;
    for (i = 0; i < 10; i++) {
        u32 *reg;
        u16 *attr;
        *(void **)(data_ov016_021de560 + i * 4 + 0x10) = DWCi_CEINlSetExObj(NULL, 0x36);
        reg = *(u32 **)(data_ov016_021de560 + i * 4 + 0x10);
        *reg = (*reg & 0xc1fffcff) | 0x200;
        attr = (u16 *)(*(s32 *)(data_ov016_021de560 + i * 4 + 0x10) + 4);
        *attr = (*attr & ~0xc00) | 0xc00;
    }
    for (j = 0; j < 2; j++) {
        u32 *reg;
        u16 *attr;
        *(void **)(data_ov016_021de560 + j * 4 + 0x38) = DWCi_CEINlSetExObj(NULL, data_ov016_021da35e[j]);
        reg = *(u32 **)(data_ov016_021de560 + j * 4 + 0x38);
        *reg = (*reg & 0xc1fffcff) | 0x200;
        attr = (u16 *)(*(s32 *)(data_ov016_021de560 + j * 4 + 0x38) + 4);
        *attr = (*attr & ~0xc00) | 0xc00;
    }
    for (k = 0; k < 2; k++) {
        *(s32 **)(data_ov016_021de560 + k * 4 + 0x40) = DWCi_CEINlSetExCell(0, data_ov016_021da360[k], 1);
        DWCi_CELLlSetEffect(*(void **)(data_ov016_021de560 + k * 4 + 0x40), -1, 0x200, 0);
        DWCi_CELLlSetPriority(*(void **)(data_ov016_021de560 + k * 4 + 0x40), -1, 3);
    }
    str[1] = 0;
    i = 0;
    do {
        n = 0;
        canvas = DWCi_FNTlFormEx(0, data_ov016_021da36e.x, data_ov016_021da36e.y, 0, &canvasSize, 0);
        a.l = 0;
        *(NNSG2dCharCanvas **)(data_ov016_021de560 + i * 4) = canvas;
        idx = i * 3;
        do {
            str[0] = data_ov016_021da396[idx];
            DWCi_FNTlDrawRect(*(NNSG2dCharCanvas **)(data_ov016_021de560 + i * 4), a.l, a.t, a.w, a.h, 2, 0x480, str);
            n++;
            a.l += 0x20;
            idx++;
        } while (n < 3);
        *(s32 **)(data_ov016_021de560 + i * 4 + 0x48) = (s32 *)DWCi_CELLlForm(0, canvasSize, 0);
        i++;
    } while (i < 4);
    *(s32 **)(data_ov016_021de560 + 0x58) = DWCi_CEINlSetExCell(0, 0x44, 1);
    DWCi_CELLlSetEffect(*(void **)(data_ov016_021de560 + 0x58), -1, 0x200, 0);
    DWCi_CELLlSetPriority(*(void **)(data_ov016_021de560 + 0x58), -1, 2);
    *(s32 **)(data_ov016_021de560 + 0x5c) = DWCi_TSKlForm(0, (s32)taskStart0_keyboard10, 0, 0x78);
    disp_keyboard10(0, 0xc0);
}

void DWCi_KB10lEnd(void) {
    DWCi_CELLlDelete(*(s32 **)(data_ov016_021de560 + 0x58));
    DWCi_TSKlChangeFunc(*(void **)(data_ov016_021de560 + 0x5c), (u32)func_ov016_021c0528);
}

u8 DWCi_KB10lGet(void) {
    return *(u8 *)(data_ov016_021de560 + 0x60);
}

void DWCi_KB10lSetErase(u8 enable) {
    *(u8 *)(data_ov016_021de560 + 0x66) = enable;
}

void DWCi_KB10lSetInput(u8 enable) {
    *(u8 *)(data_ov016_021de560 + 0x67) = enable;
}

void DWCi_KB10lSetShift(u8 enable) {
    *(u8 *)(data_ov016_021de560 + 0x68) = enable;
}

BOOL DWCi_KB10lIsInit(void) {
    return (u32)(data_ov016_021de560 != 0);
}

void taskStart0_keyboard10(void *task) {
    u32 *ptr;
    u32 val;
    s32 val2;
    s32 *ptr2;

    ptr = &val;
    ptr2 = &val2;
    val2 = (u8)(*(*(s32 **)(0x10 + (u8 *)data_ov016_021de560)));
    val2 = *ptr2 - 0xC;
    if (val2 > 0x4C) {
        val = (u32)(*ptr2);
        disp_keyboard10(0, *ptr);
        return;
    }
    disp_keyboard10(0, 0x4CU);
    disp_keyboard10(1, 0xC0U);
    DWCi_TSKlChangeFunc(task, (u32) taskStart1_keyboard10);
}

void taskStart1_keyboard10(void *task) {
    u32 *ptr;
    s32 *ptr2;
    s32 val;
    u32 val2;

    val = (u8) **(s32 **)((u8 *)data_ov016_021de560 + 0x1C) - 0xC;
    ptr2 = &val;
    ptr = &val2;
    if (val > 0x63) {
        val2 = (u32)(*ptr2);
        disp_keyboard10(1, *ptr);
        return;
    }
    val = 1;
    disp_keyboard10(*ptr2, 0x63U);
    disp_keyboard10(2, 0xC0U);
    DWCi_TSKlChangeFunc(task, (u32) taskStart2_keyboard10);
}

void taskStart2_keyboard10(s32 task) {
    s32 val;
    s32 val2;
    s32 *ptr;
    s32 *ptr2;

    val = (u8) **(s32 **)((u8 *)data_ov016_021de560 + 0x28) - 0xC;
    ptr = &val2;
    ptr2 = &val;
    if (val > 0x7A) {
        disp_keyboard10(2, (u32)(*ptr2));
        return;
    }
    disp_keyboard10(2, 0x7AU);
    val2 = task;
    disp_keyboard10(3, 0xC0U);
    DWCi_TSKlChangeFunc((void *)(*ptr), (u32)taskStart3_keyboard10);
}

void taskStart3_keyboard10(void *task) {
    u32 val;
    s32 val2;
    s32 *ptr;
    u32 *ptr2;

    ptr2 = &val;
    val2 = (u8) **(s32 **)((u8 *)data_ov016_021de560 + 0x34) - 0xC;
    ptr = &val2;
    if (*ptr > 0x91) {
        val = (u32)(*ptr);
        disp_keyboard10(3, *ptr2);
        return;
    }
    disp_keyboard10(3, 0x91U);
    disp_keyboard10(4, 0xC0U);
    DWCi_TSKlChangeFunc(task, (u32) taskStart4_keyboard10);
}

void taskStart4_keyboard10(void *task) {
    u32 uStack_c;
    u32 uStack_10;


    DWCi_CELLlGetPosition(*(void **)(data_ov016_021de560 + 0x40), 0, &uStack_c, &uStack_10);
    uStack_10 = uStack_10 - 0xc;
    if (0xaa < (s32)uStack_10) {
        disp_keyboard10(4, uStack_10);
        return;
    }
    disp_keyboard10(4, 0xaa);
    func_ov016_021c03f0();
    DWCi_TSKlChangeFunc(task, (u32)taskMain_keyboard10);
}

void taskMain_keyboard10(void) {
    touchTriger_keyboard10();
    touchRelease_keyboard10();
    touch_keyboard10();
    key_keyboard10();
}

static const COORD data_ov016_021da37a[2] = { { 0x72, 0x91 }, { 0x92, 0x91 } };  // C_SKEY
static const COORD data_ov016_021da36a = { 0x1c, 0x14 };  // S_KEY
void touchTriger_keyboard10(void) {
    s16 rect[4];
    s32 i;

    if (!DWCi_TPlCheckTrigger((u16 *)&data_ov016_021daa7c)) {
        return;
    }
    *(s8 *)(data_ov016_021de560 + 0x61) = -1;
    for (i = 0; i < 10; i++) {
        func_ov016_021d600c((s16 *)&data_ov016_021da3b0[i], (s16 *)&data_ov016_021da36a, rect);
        if (DWCi_TPlCheckTrigger((u16 *)rect)) {
            if (*(u8 *)(data_ov016_021de560 + 0x67) == 0) {
                DWCi_SNDlPlay(9);
                return;
            }
            DWCi_SNDlPlay(0);
            *(s8 *)(data_ov016_021de560 + 0x61) = i;
            return;
        }
    }
    for (i = 0; i < 2; i++) {
        func_ov016_021d600c((s16 *)&data_ov016_021da37a[i], (s16 *)&data_ov016_021da372, rect);
        if (DWCi_TPlCheckTrigger((u16 *)rect)) {
            if ((i == 0 && *(u8 *)(data_ov016_021de560 + 0x66) == 0) ||
                (i == 1 && *(u8 *)(data_ov016_021de560 + 0x68) == 0)) {
                DWCi_SNDlPlay(9);
                return;
            }
            DWCi_SNDlPlay(0);
            *(s8 *)(data_ov016_021de560 + 0x61) = i + 10;
            return;
        }
    }
    for (i = 0; i < 2; i++) {
        func_ov016_021d600c((s16 *)&data_ov016_021da382[i], (s16 *)&data_ov016_021da376, rect);
        if (DWCi_TPlCheckTrigger((u16 *)rect)) {
            DWCi_SNDlPlay(0);
            *(s8 *)(data_ov016_021de560 + 0x61) = i + 12;
            return;
        }
    }
}


void touchRelease_keyboard10(void) {
    BOOL result;
    s32 i;
    u16 auStack_20 [4];

    i = 0;
    *(u8 *)(data_ov016_021de560 + 0x60) = 0;
    result = DWCi_TPlCheckRelease((u16 *)&data_ov016_021daa7c);
    if (!result) {
        return;
    }
    do {
        func_ov016_021d600c((s16 *)&data_ov016_021da3b0[i], (s16 *)&data_ov016_021da36a,
                                                (s16 *)auStack_20);
        result = DWCi_TPlCheckRelease(auStack_20);
        if (result) {
            if (*(s8 *)(data_ov016_021de560 + 0x61) != i) {
                return;
            }
            *(u8 *)(data_ov016_021de560 + 0x60) = data_ov016_021da38a[i];
            *(s8 *)(data_ov016_021de560 + 99) = (s8)i;
            func_ov016_021c03f0();
            return;
        }
        i = i + 1;
    } while (i < 10);
    i = 0;
    do {
        func_ov016_021d600c((s16 *)&data_ov016_021da37a[i], (s16 *)&data_ov016_021da372,
                                                (s16 *)auStack_20);
        result = DWCi_TPlCheckRelease(auStack_20);
        if (result) {
            if ((s32)*(s8 *)(data_ov016_021de560 + 0x61) != i + 10) {
                return;
            }
            *(u8 *)(data_ov016_021de560 + 0x60) = data_ov016_021da367[i];
            *(s8 *)(data_ov016_021de560 + 99) = (s8)(i + 10);
            func_ov016_021c03f0();
            return;
        }
        i = i + 1;
    } while (i < 2);
    i = 0;
    do {
        func_ov016_021d600c((s16 *)&data_ov016_021da382[i], (s16 *)&data_ov016_021da376,
                                                (s16 *)auStack_20);
        result = DWCi_TPlCheckRelease(auStack_20);
        if (result) {
            if ((s32)*(s8 *)(data_ov016_021de560 + 0x61) != i + 0xc) {
                return;
            }
            *(u8 *)(data_ov016_021de560 + 0x60) = data_ov016_021da364[i];
            *(s8 *)(data_ov016_021de560 + 99) = (s8)(i + 0xc);
            func_ov016_021c03f0();
            return;
        }
        i = i + 1;
    } while (i < 2);
}

void touch_keyboard10(void) {
    s32 i;
    u16 rect[4];

    if (DWCi_TPlCheck((u16 *)&data_ov016_021daa7c)) {
        i = 0;
        do {
            func_ov016_021d600c((s16 *)&data_ov016_021da3b0[i], (s16 *)&data_ov016_021da36a,
                                (s16 *)rect);
            if (DWCi_TPlCheck(rect)) {
                if (*(s8 *)(data_ov016_021de560 + 0x61) != i) {
                    goto none;
                }
                exchangeBack_keyboard10(i);
                goto clear;
            }
            i++;
        } while (i < 10);
        i = 0;
        do {
            func_ov016_021d600c((s16 *)&data_ov016_021da37a[i], (s16 *)&data_ov016_021da372,
                                (s16 *)rect);
            if (DWCi_TPlCheck(rect)) {
                if (*(s8 *)(data_ov016_021de560 + 0x61) != i + 10) {
                    goto none;
                }
                exchangeBack_keyboard10(i + 10);
                if (i == 0) {
                    *(u8 *)(data_ov016_021de560 + 0x65) = *(u8 *)(data_ov016_021de560 + 0x65) + 1;
                    if (*(u8 *)(data_ov016_021de560 + 0x65) < 0x28) {
                        return;
                    }
                    if (*(u8 *)(data_ov016_021de560 + 0x66) == 0) {
                        DWCi_SNDlPlay(9);
                        *(s8 *)(data_ov016_021de560 + 0x61) = -1;
                        return;
                    }
                    *(u8 *)(data_ov016_021de560 + 0x60) = 0x10;
                    *(u8 *)(data_ov016_021de560 + 0x65) = *(u8 *)(data_ov016_021de560 + 0x65) - 7;
                    return;
                }
                goto clear;
            }
            i++;
        } while (i < 2);
        i = 0;
        do {
            func_ov016_021d600c((s16 *)&data_ov016_021da382[i], (s16 *)&data_ov016_021da376,
                                (s16 *)rect);
            if (DWCi_TPlCheck(rect)) {
                if (*(s8 *)(data_ov016_021de560 + 0x61) != i + 12) {
                    goto none;
                }
                exchangeBack_keyboard10(i + 12);
                goto clear;
            }
            i++;
        } while (i < 2);
    }
none:
    exchangeBack_keyboard10(-1);
clear:
    *(u8 *)(data_ov016_021de560 + 0x65) = 0;
}

void exchangeBack_keyboard10(s32 idx) {
    if (idx == *(s8 *)(data_ov016_021de560 + 0x62)) {
        return;
    }
    changeColor_keyboard10(idx, 1);
    changeColor_keyboard10((s32)*(s8 *)(data_ov016_021de560 + 0x62), 0);
    *(s8 *)(data_ov016_021de560 + 0x62) = (s8)idx;
}

void key_keyboard10(void) {
    s32 idx;
    if (DWCi_IPTlCheckRepeat(0x20)) {
        moveFocus_keyboard10(0);
    }
    if (DWCi_IPTlCheckRepeat(0x40)) {
        moveFocus_keyboard10(1);
    }
    if (DWCi_IPTlCheckRepeat(0x10)) {
        moveFocus_keyboard10(2);
    }
    if (DWCi_IPTlCheckRepeat(0x80)) {
        moveFocus_keyboard10(3);
    }
    if (DWCi_IPTlCheckTrigger(1)) {
        idx = *(s8 *)(data_ov016_021de560 + 0x63);
        if (idx < 10) {
            if (*(u8 *)(data_ov016_021de560 + 0x67) != 0) {
                *(u8 *)(data_ov016_021de560 + 0x60) = data_ov016_021da38a[idx];
                return;
            }
            DWCi_SNDlPlay(9);
            return;
        }
        if (idx - 10 < 2) {
            if ((idx - 10 == 0 && *(u8 *)(data_ov016_021de560 + 0x66) == 0) ||
                (idx - 10 == 1 && *(u8 *)(data_ov016_021de560 + 0x68) == 0)) {
                DWCi_SNDlPlay(9);
                return;
            }
            *(u8 *)(data_ov016_021de560 + 0x60) = data_ov016_021da367[idx - 10];
            return;
        }
        *(u8 *)(data_ov016_021de560 + 0x60) = data_ov016_021da364[idx - 12];
    }
    if (DWCi_IPTlCheckRepeat(2)) {
        if (*(u8 *)(data_ov016_021de560 + 0x66) == 0) {
            if (*(u8 *)(data_ov016_021de560 + 0x69) != 0) {
                return;
            }
            DWCi_SNDlPlay(9);
            *(u8 *)(data_ov016_021de560 + 0x69) = 1;
            return;
        }
        *(u8 *)(data_ov016_021de560 + 0x60) = 0x10;
    } else if (DWCi_IPTlCheckRelease(2)) {
        *(u8 *)(data_ov016_021de560 + 0x69) = 0;
    }
}

void disp_keyboard10(s32 line, s32 y) {
    const u8 N_KEY[5] = {3, 3, 3, 1, 0};
    const u8 N_SKEY[5] = {0, 0, 0, 2, 0};
    const u8 N_BTN[5] = {0, 0, 0, 0, 2};
    s32 ix;
    s32 i;

    ix = line * 3;
    for (i = 0; i < N_KEY[line]; i++, ix++) {
        G2_SetOBJEffect(((Ov016Keyboard10Work *)data_ov016_021de560)->keyCell[ix], GX_OAM_EFFECT_NONE, 0);
        G2_SetOBJPosition(((Ov016Keyboard10Work *)data_ov016_021de560)->keyCell[ix], data_ov016_021da3b0[ix].x, y);
    }
    if (line < 4) {
        func_ov016_021d5f28(((Ov016Keyboard10Work *)data_ov016_021de560)->canvas[line], data_ov016_021da3b0[line * 3].x, y,
                            (s32)((Ov016Keyboard10Work *)data_ov016_021de560)->bgCell[line], 2);
    }
    for (i = 0; i < N_SKEY[line]; i++) {
        G2_SetOBJEffect(((Ov016Keyboard10Work *)data_ov016_021de560)->subCell[i], GX_OAM_EFFECT_NONE, 0);
        G2_SetOBJPosition(((Ov016Keyboard10Work *)data_ov016_021de560)->subCell[i], data_ov016_021da37a[i].x, y);
    }
    for (i = 0; i < N_BTN[line]; i++) {
        DWCi_CELLlSetEffect(((Ov016Keyboard10Work *)data_ov016_021de560)->bg[i], -1, 0, 0);
        func_ov016_021d5118((s32)((Ov016Keyboard10Work *)data_ov016_021de560)->bg[i], -1, data_ov016_021da382[i].x, y);
    }
}



static inline void setObjColor_keyboard10(u32 color, u32 *obj) {
    *obj &= ~0xC00;
    ((u16 *)obj)[2] = (((u16 *)obj)[2] & ~0xF000) | (color << 12);
}
void changeColor_keyboard10(s32 idx, s32 push) {
    if (idx < 0) {
        return;
    }
    if (idx < 0xa) {
        G2_SetOBJMode(((Ov016Keyboard10Work *)data_ov016_021de560)->keyCell[idx], GX_OAM_MODE_NORMAL, data_ov016_021da362[push]);
    } else if (idx - 0xa < 2) {
        G2_SetOBJMode(((Ov016Keyboard10Work *)data_ov016_021de560)->subCell[idx - 0xa], GX_OAM_MODE_NORMAL, data_ov016_021da362[push]);
    } else {
        DWCi_CELLlSetPalette(((Ov016Keyboard10Work *)data_ov016_021de560)->bg[idx - 0xc], -1, 0, data_ov016_021da35c[push]);
    }
}

void func_ov016_021c03f0(void) {
    s32 val;

    val = 0x44;
    if (0xb < *(s8 *)(data_ov016_021de560 + 99)) {
        val = 0x45;
    }
    func_ov016_021d52d4(0, val, (void *)DWCi_CELLlGetObj(*(void **)(data_ov016_021de560 + 0x58), 0));
    DWCi_CELLlSetPriority(*(void **)(data_ov016_021de560 + 0x58), -1, 2);
    func_ov016_021d5118(*(s32 *)(data_ov016_021de560 + 0x58), -1,
                        data_ov016_021da3d8[*(s8 *)(data_ov016_021de560 + 99)].x,
                        data_ov016_021da3d8[*(s8 *)(data_ov016_021de560 + 99)].y);
}

static const s8 data_ov016_021da410[14][4] = {  // MOVE_TBL
    { 2, 12, 1, 3 }, { 0, 13, 2, 4 }, { 1, 13, 0, 5 }, { 5, 0, 4, 6 }, { 3, 1, 5, 7 }, { 4, 2, 3, 8 }, { 8, 3, 7, 9 },
    { 6, 4, 8, 10 }, { 7, 5, 6, 11 }, { 11, 6, 10, 12 }, { 9, 7, 11, 13 }, { 10, 8, 9, 13 }, { 13, 9, 13, 0 },
    { 12, -1, 12, -2 },
};
void moveFocus_keyboard10(s32 param_1) {
    s8 focus;
    s8 prev;

    focus = *(s8 *)(data_ov016_021de560 + 0x63);
    *(s8 *)(data_ov016_021de560 + 0x63) = data_ov016_021da410[focus][param_1];
    if (*(s8 *)(data_ov016_021de560 + 0x63) == 13 && (param_1 == 1 || param_1 == 3)) {
        *(s8 *)(data_ov016_021de560 + 0x64) = focus;
    } else if (*(s8 *)(data_ov016_021de560 + 0x63) == -1) {
        prev = *(s8 *)(data_ov016_021de560 + 0x64);
        if (prev == 1 || prev == 10) {
            *(s8 *)(data_ov016_021de560 + 0x63) = 10;
        } else {
            *(s8 *)(data_ov016_021de560 + 0x63) = 11;
        }
    } else if (*(s8 *)(data_ov016_021de560 + 0x63) == -2) {
        prev = *(s8 *)(data_ov016_021de560 + 0x64);
        if (prev == 1 || prev == 10) {
            *(s8 *)(data_ov016_021de560 + 0x63) = 1;
        } else {
            *(s8 *)(data_ov016_021de560 + 0x63) = 2;
        }
    }
    func_ov016_021c03f0();
    DWCi_SNDlPlay(8);
}

static inline void keyboard10_getPosition(u32 *obj, u32 *x, u32 *y) {
    *x = (*obj & 0x1ff0000) >> 0x10;
    *y = *obj & 0xff;
}

void func_ov016_021c0528(void *param_1) {
    int x;
    int y;

    keyboard10_getPosition((u32 *)DWCi_CELLlGetObj(*(void **)(data_ov016_021de560 + 0x40), 0), &x, &y);
    y += 0xc;
    disp_keyboard10(4, y);
    if (y >= 0xc0) {
        DWCi_TSKlChangeFunc(param_1, (u32)func_ov016_021c0588);
    }
}

static inline s32 KbGetPos(u32 *p) {
    u32 v = *p;
    return (v & 0xff) + 0xc;
}
void func_ov016_021c0588(void *param_1) {
    int x;
    int y;

    keyboard10_getPosition(*(u32 **)(data_ov016_021de560 + 0x34), &x, &y);
    y += 0xc;
    disp_keyboard10(3, y);
    if (y >= 0xc0) {
        DWCi_TSKlChangeFunc(param_1, (u32)func_ov016_021c05e0);
    }
}

void func_ov016_021c05e0(void *param_1) {
    int x;
    int y;

    keyboard10_getPosition(*(u32 **)(data_ov016_021de560 + 0x28), &x, &y);
    y += 0xc;
    disp_keyboard10(2, y);
    if (y >= 0xc0) {
        DWCi_TSKlChangeFunc(param_1, (u32)func_ov016_021c0638);
    }
}


static inline void KbGetPosition(u32 *obj, u32 *pos) {
    pos[0] = (*obj & 0x1ff0000) >> 0x10;
    pos[1] = *obj & 0xff;
}
void func_ov016_021c0638(void *param_1) {
    int x;
    int y;

    keyboard10_getPosition(*(u32 **)(data_ov016_021de560 + 0x1c), &x, &y);
    y += 0xc;
    disp_keyboard10(1, y);
    if (y >= 0xc0) {
        DWCi_TSKlChangeFunc(param_1, (u32)func_ov016_021c0690);
    }
}

void func_ov016_021c0690(void *param_1) {
    int x;
    int y;

    keyboard10_getPosition(*(u32 **)(data_ov016_021de560 + 0x10), &x, &y);
    y += 0xc;
    disp_keyboard10(0, y);
    if (y >= 0xc0) {
        DWCi_TSKlChangeFunc(param_1, (u32)taskEnd5_keyboard10);
    }
}





void taskEnd5_keyboard10(u32 param_1) {
    s32 i;

    DWCi_TSKlDelete(0, param_1);
    for (i = 0; i < 4; i++) {
        DWCi_CELLlDelete(*(s32 **)(data_ov016_021de560 + i * 4 + 0x48));
        DWCi_FNTlDelete(*(s32 *)(data_ov016_021de560 + i * 4));
    }
    for (i = 0; i < 2; i++) {
        DWCi_CELLlDelete(*(s32 **)(data_ov016_021de560 + i * 4 + 0x40));
    }
    for (i = 0; i < 2; i++) {
        DWCi_OBJlDelete(*(u32 **)(data_ov016_021de560 + i * 4 + 0x38));
    }
    for (i = 0; i < 10; i++) {
        DWCi_OBJlDelete(*(u32 **)(data_ov016_021de560 + i * 4 + 0x10));
    }
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de560);
}
