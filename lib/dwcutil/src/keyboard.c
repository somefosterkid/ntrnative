// libdwcutil's keyboard.c (Black's ov016 0x021bd8d8-0x021bf390): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "nitro/fx/fx_cp.h"

typedef struct tagCOORD {
    u16 x;
    u16 y;
} COORD;

static s32 data_ov016_021de55c;
// The prebuilt's names in the comments; defined where pokebw2's dwc_util_keyboard.c has them, which gives Black's layout
// of MWCC's size sort: the special keys' positions and sizes before touchTriger, the cursor's moves before moveFocus
static const u8 data_ov016_021da09a[2] = { 4, 5 };  // CL_BTN
static const u8 data_ov016_021da09c[2] = { 0x3c, 0x3d };  // CN_BTN
static const u8 data_ov016_021da098[2] = { 2, 3 };  // CL_KEY
static const u8 data_ov016_021da09e[3] = { 0x82, 0x83, 0 };  // BTN_OUT
static const COORD data_ov016_021da0ae = { 0x11, 0x12 };  // S_KEY
static const COORD data_ov016_021da0a6 = { 0x1c, 2 };  // S_CANV
static const COORD data_ov016_021da0aa = { 0x78, 0x12 };  // S_BTN
static const u8 data_ov016_021da0a1[4] = { 0x3a, 0x3b, 0x35, 0x39 };  // CN_SKEY
static const u8 data_ov016_021da0b2[5] = { 0xff, 0xff, ' ', 0x80, 0 };  // SKEY_OUT
static const COORD data_ov016_021da0b8[2] = { { 0x04, 0xaa }, { 0x84, 0xaa } };  // C_BTN
static const COORD data_ov016_021da0f8[47] = {  // C_KEY
    { 0x04, 0x51 }, { 0x16, 0x51 }, { 0x28, 0x51 }, { 0x3a, 0x51 }, { 0x4c, 0x51 }, { 0x5e, 0x51 }, { 0x70, 0x51 },
    { 0x82, 0x51 }, { 0x94, 0x51 }, { 0xa6, 0x51 }, { 0xb8, 0x51 }, { 0xca, 0x51 }, { 0x20, 0x67 }, { 0x32, 0x67 },
    { 0x44, 0x67 }, { 0x56, 0x67 }, { 0x68, 0x67 }, { 0x7a, 0x67 }, { 0x8c, 0x67 }, { 0x9e, 0x67 }, { 0xb0, 0x67 },
    { 0xc2, 0x67 }, { 0xd4, 0x67 }, { 0xe6, 0x67 }, { 0x25, 0x7d }, { 0x37, 0x7d }, { 0x49, 0x7d }, { 0x5b, 0x7d },
    { 0x6d, 0x7d }, { 0x7f, 0x7d }, { 0x91, 0x7d }, { 0xa3, 0x7d }, { 0xb5, 0x7d }, { 0xc7, 0x7d }, { 0xd9, 0x7d },
    { 0xeb, 0x7d }, { 0x2d, 0x93 }, { 0x3f, 0x93 }, { 0x51, 0x93 }, { 0x63, 0x93 }, { 0x75, 0x93 }, { 0x87, 0x93 },
    { 0x99, 0x93 }, { 0xab, 0x93 }, { 0xbd, 0x93 }, { 0xcf, 0x93 }, { 0xe1, 0x93 },
};
static const COORD data_ov016_021da1b4[53] = {  // C_FOCUS
    { 0x02, 0x4f }, { 0x14, 0x4f }, { 0x26, 0x4f }, { 0x38, 0x4f }, { 0x4a, 0x4f }, { 0x5c, 0x4f }, { 0x6e, 0x4f },
    { 0x80, 0x4f }, { 0x92, 0x4f }, { 0xa4, 0x4f }, { 0xb6, 0x4f }, { 0xc8, 0x4f }, { 0x1e, 0x65 }, { 0x30, 0x65 },
    { 0x42, 0x65 }, { 0x54, 0x65 }, { 0x66, 0x65 }, { 0x78, 0x65 }, { 0x8a, 0x65 }, { 0x9c, 0x65 }, { 0xae, 0x65 },
    { 0xc0, 0x65 }, { 0xd2, 0x65 }, { 0xe4, 0x65 }, { 0x23, 0x7b }, { 0x35, 0x7b }, { 0x47, 0x7b }, { 0x59, 0x7b },
    { 0x6b, 0x7b }, { 0x7d, 0x7b }, { 0x8f, 0x7b }, { 0xa1, 0x7b }, { 0xb3, 0x7b }, { 0xc5, 0x7b }, { 0xd7, 0x7b },
    { 0xe9, 0x7b }, { 0x2b, 0x91 }, { 0x3d, 0x91 }, { 0x4f, 0x91 }, { 0x61, 0x91 }, { 0x73, 0x91 }, { 0x85, 0x91 },
    { 0x97, 0x91 }, { 0xa9, 0x91 }, { 0xbb, 0x91 }, { 0xcd, 0x91 }, { 0xdf, 0x91 }, { 0x02, 0x65 }, { 0x02, 0x7b },
    { 0x02, 0x91 }, { 0xda, 0x4f }, { 0x02, 0xa8 }, { 0x82, 0xa8 },
};
static u8 *data_ov016_021dae68[3] = {  // KEY_OUT
    "1234567890-=qwertyuiop[]asdfghjkl;'`zxcvbnm,./\x5c",
    "!@#$%^&*()_+QWERTYUIOP{}ASDFGHJKL:\x22~ZXCVBNM<>?|",
    "1234567890-=QWERTYUIOP[]ASDFGHJKL;'`ZXCVBNM,./\x5c",
};
static u16 *data_ov016_021dae74[3] = {  // KEY_CHR
    L"1234567890-=qwertyuiop[]asdfghjkl;'`zxcvbnm,./\x5c ",
    L"!@#$%^&*()_+QWERTYUIOP{}ASDFGHJKL:\x22~ZXCVBNM<>?| ",
    L"1234567890-=QWERTYUIOP[]ASDFGHJKL;'`ZXCVBNM,./\x5c ",
};
static s32 data_ov016_021dae80[4] = { 0, 0, 0, 2 };  // SKEY_TOUCH_SND

void func_ov016_021bf0d8(void *param_1);
void disp_keyboard(s32 param_1, s32 param_2, u32 param_3);
void func_ov016_021bec20(void);
void taskMain_keyboard(void);
void touchTriger_keyboard(void);
void touchRelease_keyboard(void);
void touch_keyboard(void);
void key_keyboard(void);
void changePage(s32 param_1);
void changeColor_keyboard(s32 param_1, s32 param_2);
void taskStart2_keyboard(void *param_1);
void pushCaps(void);
void pushShift(void);
void exchangeBack_keyboard(s32 param_1);
void taskStart1_keyboard(void *param_1);
void taskStart4_keyboard(void *param_1);
void taskEnd5_keyboard(u32 param_1);
void taskStart3_keyboard(void *param_1);


void taskStart0_keyboard(void *param_1);

typedef struct Ov016KeyboardWork {
    NNSG2dCharCanvas *canvas[3][4];
    void *keyCell[0x2f];
    void *subCell[4];
    s32 *bg[2];
    s32 *bgCell[4];
    s32 *bgBase;
    s32 *task;
    u8 unk_11c;
    u8 unk_11d;
    s8 unk_11e;
    s8 unk_11f;
    s8 unk_120;
    s8 unk_121;
    u8 unk_122;
    u8 unk_123;
    u8 unk_124;
    u8 unk_125;
} Ov016KeyboardWork;

void func_ov016_021bf140(void *param_1);
void func_ov016_021becfc(s32 param_1);

static inline void keyboard_getPosition(u32 *obj, u32 *x, u32 *y) {
    *x = (*obj & 0x1ff0000) >> 0x10;
    *y = *obj & 0xff;
}


void func_ov016_021bf19c(void *param_1);
void func_ov016_021bf1f8(void *param_1);

typedef struct tagAREA {
    u16 l;
    u16 t;
    u16 w;
    u16 h;
} tagAREA;

void DWCi_KBlInit(void) {
    tagAREA a = {0};
    s32 num;
    s32 ix;
    s32 i;
    s32 j;
    s32 k;
    u16 buf[2];

    a.w = 0x11;
    a.h = 0x12;
    data_ov016_021de55c = (s32)DWCi_HEAPlAllocEx(0x128, 4);
    ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11c = 0xff;
    ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121 = 0;
    ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_123 = 1;
    ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_124 = 1;
    for (i = 0; i < 0x2f; i++) {
        ((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[i] = DWCi_CEINlSetExObj(NULL, 0x34);
        G2_SetOBJEffect(((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[i], GX_OAM_EFFECT_NODISPLAY, 0);
        G2_SetOBJPriority(((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[i], 3);
    }
    for (i = 0; i < 4; i++) {
        ((Ov016KeyboardWork *)data_ov016_021de55c)->subCell[i] = DWCi_CEINlSetExObj(NULL, data_ov016_021da0a1[i]);
        G2_SetOBJEffect(((Ov016KeyboardWork *)data_ov016_021de55c)->subCell[i], GX_OAM_EFFECT_NODISPLAY, 0);
        G2_SetOBJPriority(((Ov016KeyboardWork *)data_ov016_021de55c)->subCell[i], 3);
    }
    for (i = 0; i < 2; i++) {
        ((Ov016KeyboardWork *)data_ov016_021de55c)->bg[i] = DWCi_CEINlSetExCell(0, data_ov016_021da09c[i], 1);
        DWCi_CELLlSetEffect(((Ov016KeyboardWork *)data_ov016_021de55c)->bg[i], -1, 0x200, 0);
        DWCi_CELLlSetPriority(((Ov016KeyboardWork *)data_ov016_021de55c)->bg[i], -1, 3);
    }
    buf[1] = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            ((Ov016KeyboardWork *)data_ov016_021de55c)->canvas[i][j] = DWCi_FNTlFormEx(0, data_ov016_021da0a6.x, data_ov016_021da0a6.y, 0, &num, 0);
            ix = j * 12;
            a.l = 0;
            for (k = 0; k < 12; k++, ix++, a.l += 0x12) {
                buf[0] = data_ov016_021dae74[i][ix];
                DWCi_FNTlDrawRect(((Ov016KeyboardWork *)data_ov016_021de55c)->canvas[i][j], a.l, a.t, a.w, a.h, 2, 0x480, buf);
            }
            if (i == 0) {
                ((Ov016KeyboardWork *)data_ov016_021de55c)->bgCell[j] = (s32 *)DWCi_CELLlForm(0, num, 1);
            }
        }
    }
    ((Ov016KeyboardWork *)data_ov016_021de55c)->bgBase = DWCi_CEINlSetExCell(0, 0x40, 1);
    DWCi_CELLlSetEffect(((Ov016KeyboardWork *)data_ov016_021de55c)->bgBase, -1, 0x200, 0);
    DWCi_CELLlSetPriority(((Ov016KeyboardWork *)data_ov016_021de55c)->bgBase, -1, 2);
    ((Ov016KeyboardWork *)data_ov016_021de55c)->task = DWCi_TSKlForm(0, (s32)taskStart0_keyboard, 0, 0x78);
    disp_keyboard(((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d, 0, 0xc0);
}

void DWCi_KBlEnd(void) {
    DWCi_CELLlDelete(*(s32 **)(data_ov016_021de55c + 0x114));
    DWCi_TSKlChangeFunc(((Ov016KeyboardWork *)data_ov016_021de55c)->task, (u32)func_ov016_021bf0d8);
}

u8 DWCi_KBlGet(void) {
    return *(u8 *)(data_ov016_021de55c + 0x11c);
}

void DWCi_KBlSetErase(u8 enable) {
    *(u8 *)(data_ov016_021de55c + 0x123) = enable;
}

void DWCi_KBlSetInput(u8 enable) {
    *(u8 *)(data_ov016_021de55c + 0x124) = enable;
}

BOOL DWCi_KBlIsInit(void) {
    return (u32)(data_ov016_021de55c != 0);
}


void taskStart0_keyboard(void *task) {
    u32 *ptr;
    s32 *ptr2;
    s32 val;
    u32 val2;

    ptr = &val2;
    ptr2 = &val;
    val = (u8) **(s32 **)((u8 *)data_ov016_021de55c + 0x30);
    val = *ptr2 - 0xC;
    if (val > 0x51) {
        val2 = (u32)(*ptr2);
        disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 0, *ptr);
        return;
    }
    disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 0, 0x51);
    disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 1, 0xc0);
    DWCi_TSKlChangeFunc(task, (u32)taskStart1_keyboard);
}


void taskStart1_keyboard(void *task) {
    u32 *ptr;
    s32 *ptr2;
    s32 val;
    u32 val2;

    ptr = &val2;
    ptr2 = &val;
    val = (u8) **(s32 **)((u8 *)data_ov016_021de55c + 0x60);
    val = *ptr2 - 0xC;
    if (val > 0x67) {
        val2 = (u32)(*ptr2);
        disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 1, *ptr);
        return;
    }
    disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 1, 0x67);
    disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 2, 0xc0);
    DWCi_TSKlChangeFunc(task, (u32)taskStart2_keyboard);
}


void taskStart2_keyboard(void *task) {
    u32 *ptr;
    s32 *ptr2;
    s32 val;
    u32 val2;

    ptr = &val2;
    ptr2 = &val;
    val = (u8) **(s32 **)((u8 *)data_ov016_021de55c + 0x90);
    val = *ptr2 - 0xC;
    if (val > 0x7d) {
        val2 = (u32)(*ptr2);
        disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 2, *ptr);
        return;
    }
    disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 2, 0x7d);
    disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 3, 0xc0);
    DWCi_TSKlChangeFunc(task, (u32)taskStart3_keyboard);
}

void taskStart3_keyboard(void *task) {
    u32 *ptr;
    s32 *ptr2;
    s32 val;
    u32 val2;

    ptr = &val2;
    ptr2 = &val;
    val = (u8) **(s32 **)((u8 *)data_ov016_021de55c + 0xc0);
    val = *ptr2 - 0xC;
    if (val > 0x93) {
        val2 = (u32)(*ptr2);
        disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 3, *ptr);
        return;
    }
    disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 3, 0x93);
    disp_keyboard(*(u8 *)(data_ov016_021de55c + 0x11d), 4, 0xc0);
    DWCi_TSKlChangeFunc(task, (u32)taskStart4_keyboard);
}



void taskStart4_keyboard(void *task) {
    u32 auStack_14 [1];
    u32 uStack_18;


    DWCi_CELLlGetPosition(((Ov016KeyboardWork *)data_ov016_021de55c)->bg[0], 0, auStack_14, &uStack_18);
    uStack_18 = uStack_18 - 0xc;
    if (0xaa < (s32)uStack_18) {
        disp_keyboard((u32)*(u8 *)(data_ov016_021de55c + 0x11d), 4, uStack_18);
        return;
    }
    disp_keyboard((u32)*(u8 *)(data_ov016_021de55c + 0x11d), 4, 0xaa);
    func_ov016_021bec20();
    DWCi_TSKlChangeFunc(task, (u32)taskMain_keyboard);
}

void taskMain_keyboard(void) {
    touchTriger_keyboard();
    touchRelease_keyboard();
    touch_keyboard();
    key_keyboard();
}


static const COORD data_ov016_021da0c0[4] = { { 0x04, 0x67 }, { 0x04, 0x7d }, { 0x04, 0x93 }, { 0xdc, 0x51 } };  // C_SKEY
static const COORD data_ov016_021da0d0[4] = { { 0x1b, 0x12 }, { 0x20, 0x12 }, { 0x28, 0x12 }, { 0x20, 0x12 } };  // S_SKEY
void touchTriger_keyboard(void) {
    s32 i;
    s32 j;
    s32 k;
    u16 buf[4];

    if (!DWCi_TPlCheckTrigger((u16 *)&data_ov016_021daa7c)) {
        return;
    }
    *(s8 *)(data_ov016_021de55c + 0x11e) = -1;
    i = 0;
    do {
        func_ov016_021d600c((s16 *)&data_ov016_021da0f8[i], (s16 *)&data_ov016_021da0ae, (s16 *)buf);
        if (DWCi_TPlCheckTrigger(buf)) {
            if (*(u8 *)(data_ov016_021de55c + 0x124) == 0) {
                DWCi_SNDlPlay(9);
                return;
            }
            DWCi_SNDlPlay(0);
            *(s8 *)(data_ov016_021de55c + 0x11e) = i;
            return;
        }
        i++;
    } while (i < 0x2f);
    j = 0;
    do {
        func_ov016_021d600c((s16 *)&data_ov016_021da0c0[j], (s16 *)&data_ov016_021da0d0[j], (s16 *)buf);
        if (DWCi_TPlCheckTrigger(buf)) {
            if ((j == 3 && *(u8 *)(data_ov016_021de55c + 0x123) == 0) ||
                (j == 2 && *(u8 *)(data_ov016_021de55c + 0x124) == 0)) {
                DWCi_SNDlPlay(9);
                return;
            }
            DWCi_SNDlPlay(data_ov016_021dae80[j]);
            *(s8 *)(data_ov016_021de55c + 0x11e) = j + 0x2f;
            return;
        }
        j++;
    } while (j < 4);
    k = 0;
    do {
        func_ov016_021d600c((s16 *)&data_ov016_021da0b8[k], (s16 *)&data_ov016_021da0aa, (s16 *)buf);
        if (DWCi_TPlCheckTrigger(buf)) {
            DWCi_SNDlPlay(0);
            *(s8 *)(data_ov016_021de55c + 0x11e) = k + 0x33;
            return;
        }
        k++;
    } while (k < 2);
}


void touchRelease_keyboard(void) {
    u16 r[4];
    s32 i;

    ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11c = 0;
    if (!DWCi_TPlCheckRelease((u16 *)&data_ov016_021daa7c)) {
        return;
    }
    for (i = 0; i < 0x2f; i++) {
        func_ov016_021d600c((s16 *)&data_ov016_021da0f8[i], (s16 *)&data_ov016_021da0ae, (s16 *)r);
        if (DWCi_TPlCheckRelease(r)) {
            if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11e != i) {
                return;
            }
            ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11c = data_ov016_021dae68[((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d][i];
            if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d == 1) {
                changePage(0);
            }
            ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121 = i;
            func_ov016_021bec20();
            return;
        }
    }
    for (i = 2; i < 4; i++) {
        func_ov016_021d600c((s16 *)&data_ov016_021da0c0[i], (s16 *)&data_ov016_021da0d0[i], (s16 *)r);
        if (DWCi_TPlCheckRelease(r)) {
            if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11e != i + 0x2f) {
                return;
            }
            ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11c = data_ov016_021da0b2[i];
            if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d == 1) {
                changePage(0);
            }
            ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121 = i + 0x2f;
            func_ov016_021bec20();
            return;
        }
    }
    for (i = 0; i < 2; i++) {
        func_ov016_021d600c((s16 *)&data_ov016_021da0b8[i], (s16 *)&data_ov016_021da0aa, (s16 *)r);
        if (DWCi_TPlCheckRelease(r)) {
            if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11e != i + 0x33) {
                return;
            }
            ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11c = data_ov016_021da09e[i];
            ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121 = i + 0x33;
            func_ov016_021bec20();
            return;
        }
    }
    func_ov016_021d600c((s16 *)&data_ov016_021da0c0, (s16 *)&data_ov016_021da0d0, (s16 *)r);
    if (DWCi_TPlCheckRelease(r)) {
        if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11e != 0x2f) {
            return;
        }
        pushCaps();
        ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121 = 0x2f;
        func_ov016_021bec20();
        return;
    }
    func_ov016_021d600c((s16 *)&data_ov016_021da0c0[1], (s16 *)&data_ov016_021da0d0[1], (s16 *)r);
    if (!DWCi_TPlCheckRelease(r)) {
        return;
    }
    if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11e != 0x30) {
        return;
    }
    pushShift();
    ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121 = 0x30;
    func_ov016_021bec20();
}

void pushCaps(void) {
    s32 val;

    if (*(u8 *)(data_ov016_021de55c + 0x11d) == 2) {
        val = 0;
    }
    else {
        val = 2;
    }
    changePage(val);
    DWCi_SNDlPlay(1);
}

void pushShift(void) {
    changePage((u32)(*(u8 *)(data_ov016_021de55c + 0x11d) != 1));
    DWCi_SNDlPlay(1);
}

void touch_keyboard(void) {
    u16 r[4];
    s32 i;
    s32 j;

    if (DWCi_TPlCheck((u16 *)&data_ov016_021daa7c)) {
        for (i = 0; i < 0x2f; i++) {
            func_ov016_021d600c((s16 *)&data_ov016_021da0f8[i], (s16 *)&data_ov016_021da0ae, (s16 *)r);
            if (DWCi_TPlCheck(r)) {
                if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11e != i) {
                    goto release;
                }
                exchangeBack_keyboard(i);
                goto clear;
            }
        }
        for (i = 2; i < 4; i++) {
            func_ov016_021d600c((s16 *)&data_ov016_021da0c0[i], (s16 *)&data_ov016_021da0d0[i], (s16 *)r);
            if (DWCi_TPlCheck(r)) {
                if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11e != i + 0x2f) {
                    goto release;
                }
                exchangeBack_keyboard(i + 0x2f);
                if (i != 3) {
                    return;
                }
                ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_122++;
                if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_122 < 0x28) {
                    return;
                }
                if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_123 == 0) {
                    DWCi_SNDlPlay(9);
                    ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11e = -1;
                    return;
                }
                ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11c = 0x80;
                ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_122 -= 7;
                return;
            }
        }
        for (j = 0; j < 2; j++) {
            func_ov016_021d600c((s16 *)&data_ov016_021da0b8[j], (s16 *)&data_ov016_021da0aa, (s16 *)r);
            if (DWCi_TPlCheck(r)) {
                if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11e != j + 0x33) {
                    goto release;
                }
                exchangeBack_keyboard(j + 0x33);
                goto clear;
            }
        }
    }
release:
    exchangeBack_keyboard(-1);
clear:
    ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_122 = 0;
}

void exchangeBack_keyboard(s32 idx) {
    if (idx == *(s8 *)(data_ov016_021de55c + 0x11f)) {
        return;
    }
    changeColor_keyboard(idx, 1);
    changeColor_keyboard((s32)*(s8 *)(data_ov016_021de55c + 0x11f), 0);
    *(s8 *)(data_ov016_021de55c + 0x11f) = (s8)idx;
}

void key_keyboard(void) {
    s32 ix;

    if (DWCi_IPTlCheckRepeat(0x20)) {
        func_ov016_021becfc(0);
    }
    if (DWCi_IPTlCheckRepeat(0x40)) {
        func_ov016_021becfc(1);
    }
    if (DWCi_IPTlCheckRepeat(0x10)) {
        func_ov016_021becfc(2);
    }
    if (DWCi_IPTlCheckRepeat(0x80)) {
        func_ov016_021becfc(3);
    }
    if (DWCi_IPTlCheckTrigger(1)) {
        ix = ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121;
        if (ix < 0x2f) {
            if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_124 == 0) {
                DWCi_SNDlPlay(9);
                return;
            }
            ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11c = data_ov016_021dae68[((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d][ix];
            if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d == 1) {
                changePage(0);
            }
            return;
        }
        if (ix - 0x2f < 4) {
            switch (ix - 0x2f) {
            case 0:
                pushCaps();
                return;
            case 1:
                pushShift();
                return;
            case 2:
                if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_124 == 0) {
                    DWCi_SNDlPlay(9);
                    return;
                }
                break;
            case 3:
                if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_123 == 0) {
                    DWCi_SNDlPlay(9);
                    return;
                }
                break;
            }
            if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d == 1) {
                changePage(0);
            }
            ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11c = data_ov016_021da0b2[ix - 0x2f];
            return;
        }
        ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11c = data_ov016_021da09e[ix - 0x33];
    }
    if (DWCi_IPTlCheckRepeat(2)) {
        if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_123 == 0) {
            if (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_125 != 0) {
                return;
            }
            DWCi_SNDlPlay(9);
            ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_125 = 1;
            return;
        }
        ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11c = 0x80;
    } else if (DWCi_IPTlCheckRelease(2)) {
        ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_125 = 0;
    }
    if (DWCi_IPTlCheckTrigger(0x400)) {
        pushCaps();
    }
    if (DWCi_IPTlCheckTrigger(0x800)) {
        pushShift();
    }
}


void disp_keyboard(s32 page, s32 line, u32 y) {
    const u8 N_KEY[5] = {12, 12, 12, 11, 0};
    const u8 N_SKEY[5] = {1, 1, 1, 1, 0};
    const u8 N_BTN[5] = {0, 0, 0, 0, 2};
    s32 ix;
    s32 i;
    s32 sk;

    ix = line * 12;
    for (i = 0; i < N_KEY[line]; i++, ix++) {
        G2_SetOBJEffect(((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[ix], GX_OAM_EFFECT_NONE, 0);
        G2_SetOBJPosition(((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[ix], data_ov016_021da0f8[ix].x, y);
    }
    if (line < 4) {
        func_ov016_021d5f28(((Ov016KeyboardWork *)data_ov016_021de55c)->canvas[page][line], data_ov016_021da0f8[line * 12].x, y,
                            (s32)((Ov016KeyboardWork *)data_ov016_021de55c)->bgCell[line], 2);
    }
    sk = FX_ModS32(line + 3, 4);
    for (i = 0; i < N_SKEY[line]; i++) {
        G2_SetOBJEffect(((Ov016KeyboardWork *)data_ov016_021de55c)->subCell[sk], GX_OAM_EFFECT_NONE, 0);
        G2_SetOBJPosition(((Ov016KeyboardWork *)data_ov016_021de55c)->subCell[sk], data_ov016_021da0c0[sk].x, y);
    }
    for (i = 0; i < N_BTN[line]; i++) {
        DWCi_CELLlSetEffect(((Ov016KeyboardWork *)data_ov016_021de55c)->bg[i], -1, 0, 0);
        func_ov016_021d5118((s32)((Ov016KeyboardWork *)data_ov016_021de55c)->bg[i], -1, data_ov016_021da0b8[i].x, y);
    }
}


void changePage(s32 page) {
    s32 i;
    s32 flag;
    int val;
    s32 flag2;

    flag = 0;
    flag2 = 0;
    i = 0;
    *(s8 *)((u8 *)data_ov016_021de55c + 0x11D) = (s8)page;
    do {
        val = i * 0xC;
        disp_keyboard(page, i, data_ov016_021da0f8[val].y);
        i += 1;
    } while (i < 4);
    if (page == 2) {
        flag = 1;
    } else if (page == 1) {
        flag2 = 1;
    }
    changeColor_keyboard(0x2F, flag);
    changeColor_keyboard(0x30, flag2);
}

void changeColor_keyboard(s32 idx, s32 push) {
    if (idx < 0) {
        return;
    }
    if (idx < 0x2f) {
        G2_SetOBJMode(((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[idx], GX_OAM_MODE_NORMAL, data_ov016_021da098[push]);
    } else if (idx - 0x2f < 4) {
        G2_SetOBJMode(((Ov016KeyboardWork *)data_ov016_021de55c)->subCell[idx - 0x2f], GX_OAM_MODE_NORMAL, data_ov016_021da098[push]);
    } else {
        DWCi_CELLlSetPalette(((Ov016KeyboardWork *)data_ov016_021de55c)->bg[idx - 0x33], -1, 0, data_ov016_021da09a[push]);
    }
}

void func_ov016_021bec20(void) {
    s32 code;

    switch (((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121) {
    case 0x2f:
        code = 0x42;
        break;
    case 0x30:
        code = 0x41;
        break;
    case 0x31:
        code = 0x43;
        break;
    case 0x32:
        code = 0x41;
        break;
    case 0x33:
    case 0x34:
        code = 0x45;
        break;
    default:
        code = 0x40;
        break;
    }
    func_ov016_021d52d4(0, code, (void *)DWCi_CELLlGetObj(((Ov016KeyboardWork *)data_ov016_021de55c)->bgBase, 0));
    DWCi_CELLlSetPriority(((Ov016KeyboardWork *)data_ov016_021de55c)->bgBase, -1, 2);
    func_ov016_021d5118((s32)((Ov016KeyboardWork *)data_ov016_021de55c)->bgBase, -1,
            data_ov016_021da1b4[((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121].x,
            data_ov016_021da1b4[((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121].y);
}

static const s8 data_ov016_021da288[53][4] = {  // MOVE_TBL
    { 0x32, 0x33, 0x01, 0x2f }, { 0x00, 0x33, 0x02, 0x0c }, { 0x01, 0x33, 0x03, 0x0d }, { 0x02, 0x33, 0x04, 0x0e },
    { 0x03, 0x33, 0x05, 0x0f }, { 0x04, 0x33, 0x06, 0x10 }, { 0x05, 0x34, 0x07, 0x11 }, { 0x06, 0x34, 0x08, 0x12 },
    { 0x07, 0x34, 0x09, 0x13 }, { 0x08, 0x34, 0x0a, 0x14 }, { 0x09, 0x34, 0x0b, 0x15 }, { 0x0a, 0x34, 0x32, 0x16 },
    { 0x2f, 0x01, 0x0d, 0x18 }, { 0x0c, 0x02, 0x0e, 0x19 }, { 0x0d, 0x03, 0x0f, 0x1a }, { 0x0e, 0x04, 0x10, 0x1b },
    { 0x0f, 0x05, 0x11, 0x1c }, { 0x10, 0x06, 0x12, 0x1d }, { 0x11, 0x07, 0x13, 0x1e }, { 0x12, 0x08, 0x14, 0x1f },
    { 0x13, 0x09, 0x15, 0x20 }, { 0x14, 0x0a, 0x16, 0x21 }, { 0x15, 0x0b, 0x17, 0x22 }, { 0x16, 0x32, 0x2f, 0x23 },
    { 0x30, 0x0c, 0x19, 0x24 }, { 0x18, 0x0d, 0x1a, 0x25 }, { 0x19, 0x0e, 0x1b, 0x26 }, { 0x1a, 0x0f, 0x1c, 0x27 },
    { 0x1b, 0x10, 0x1d, 0x28 }, { 0x1c, 0x11, 0x1e, 0x29 }, { 0x1d, 0x12, 0x1f, 0x2a }, { 0x1e, 0x13, 0x20, 0x2b },
    { 0x1f, 0x14, 0x21, 0x2c }, { 0x20, 0x15, 0x22, 0x2d }, { 0x21, 0x16, 0x23, 0x2e }, { 0x22, 0x17, 0x30, 0x2e },
    { 0x31, 0x18, 0x25, 0x33 }, { 0x24, 0x19, 0x26, 0x33 }, { 0x25, 0x1a, 0x27, 0x33 }, { 0x26, 0x1b, 0x28, 0x33 },
    { 0x27, 0x1c, 0x29, 0x33 }, { 0x28, 0x1d, 0x2a, 0x34 }, { 0x29, 0x1e, 0x2b, 0x34 }, { 0x2a, 0x1f, 0x2c, 0x34 },
    { 0x2b, 0x20, 0x2d, 0x34 }, { 0x2c, 0x21, 0x2e, 0x34 }, { 0x2d, -1, 0x31, 0x34 }, { 0x17, 0x00, 0x0c, 0x30 },
    { 0x23, 0x2f, 0x18, 0x31 }, { 0x2e, 0x30, 0x24, 0x33 }, { 0x0b, 0x34, 0x00, 0x17 }, { 0x34, -2, 0x34, -4 },
    { 0x33, -3, 0x33, -5 },
};

void func_ov016_021becfc(s32 param_1) {
    Ov016KeyboardWork *w;
    s8 prev;
    s8 code;

    prev = ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121;
    ((Ov016KeyboardWork *)data_ov016_021de55c)->unk_121 = data_ov016_021da288[prev][param_1];
    w = (Ov016KeyboardWork *)data_ov016_021de55c;
    if (w->unk_121 == 0x2e && param_1 == 3) {
        w->unk_120 = prev;
    } else if (w->unk_121 == 0x33 && (param_1 == 1 || param_1 == 3)) {
        w->unk_120 = prev;
    } else if (w->unk_121 == 0x34 && (param_1 == 1 || param_1 == 3)) {
        if (prev != 0x2e) {
            w->unk_120 = prev;
        }
    } else if (w->unk_121 == -1) {
        if (w->unk_120 == 0x23 || w->unk_120 == 0x32) {
            w->unk_121 = 0x23;
        } else {
            w->unk_121 = 0x22;
        }
    } else if (w->unk_121 == -2) {
        switch (w->unk_120) {
        case 0:
        case 0x31:
            code = 0x31;
            break;
        case 1:
        case 0x24:
            code = 0x24;
            break;
        case 3:
        case 0x26:
            code = 0x26;
            break;
        case 4:
        case 0x27:
            code = 0x27;
            break;
        case 5:
        case 0x28:
            code = 0x28;
            break;
        default:
            code = 0x25;
            break;
        }
        goto store;
    } else if (w->unk_121 == -3) {
        switch (w->unk_120) {
        case 6:
        case 0x29:
            code = 0x29;
            break;
        case 7:
        case 0x2a:
            code = 0x2a;
            break;
        case 9:
        case 0x2c:
            code = 0x2c;
            break;
        case 10:
        case 0x2d:
            code = 0x2d;
            break;
        case 11:
        case 0x22:
        case 0x23:
        case 0x32:
            code = 0x2e;
            break;
        default:
            code = 0x2b;
            break;
        }
        goto store;
    } else if (w->unk_121 == -4) {
        switch (w->unk_120) {
        case 0:
        case 0x31:
            code = 0;
            break;
        case 1:
        case 0x24:
            code = 1;
            break;
        case 3:
        case 0x26:
            code = 3;
            break;
        case 4:
        case 0x27:
            code = 4;
            break;
        case 5:
        case 0x28:
            code = 5;
            break;
        default:
            code = 2;
            break;
        }
        goto store;
    } else if (w->unk_121 == -5) {
        switch (w->unk_120) {
        case 6:
        case 0x29:
            code = 6;
            break;
        case 7:
        case 0x2a:
            code = 7;
            break;
        case 9:
        case 0x2c:
            code = 9;
            break;
        case 10:
        case 0x2d:
            code = 10;
            break;
        case 11:
        case 0x22:
            code = 11;
            break;
        case 0x23:
        case 0x32:
            code = 0x32;
            break;
        default:
            code = 8;
            break;
        }
store:
        w->unk_121 = code;
    }
    func_ov016_021bec20();
    DWCi_SNDlPlay(8);
}


void func_ov016_021bf0d8(void *param_1) {
    int x;
    int y;

    keyboard_getPosition((u32 *)DWCi_CELLlGetObj(((Ov016KeyboardWork *)data_ov016_021de55c)->bg[0], 0), &x, &y);
    y += 0xc;
    disp_keyboard(((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d, 4, y);
    if (y >= 0xc0) {
        DWCi_TSKlChangeFunc(param_1, (u32)func_ov016_021bf140);
    }
}


void func_ov016_021bf140(void *param_1) {
    int x;
    int y;

    keyboard_getPosition(((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[0x24], &x, &y);
    y += 0xc;
    disp_keyboard(((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d, 3, y);
    if (y >= 0xc0) {
        DWCi_TSKlChangeFunc(param_1, (u32)func_ov016_021bf19c);
    }
}

void func_ov016_021bf19c(void *param_1) {
    int x;
    int y;

    keyboard_getPosition(((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[0x18], &x, &y);
    y += 0xc;
    disp_keyboard(((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d, 2, y);
    if (y >= 0xc0) {
        DWCi_TSKlChangeFunc(param_1, (u32)func_ov016_021bf1f8);
    }
}



void func_ov016_021bf254(void *param_1);

void func_ov016_021bf1f8(void *param_1) {
    int x;
    int y;

    keyboard_getPosition(((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[0xc], &x, &y);
    y += 0xc;
    disp_keyboard(((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d, 1, y);
    if (y >= 0xc0) {
        DWCi_TSKlChangeFunc(param_1, (u32)func_ov016_021bf254);
    }
}

void func_ov016_021bf254(void *param_1) {
    int x;
    int y;

    keyboard_getPosition(((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[0], &x, &y);
    y += 0xc;
    disp_keyboard(((Ov016KeyboardWork *)data_ov016_021de55c)->unk_11d, 0, y);
    if (y >= 0xc0) {
        DWCi_TSKlChangeFunc(param_1, (u32)taskEnd5_keyboard);
    }
}

void taskEnd5_keyboard(u32 param_1) {
    s32 i, j;

    DWCi_TSKlDelete(0, param_1);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            DWCi_FNTlDelete((s32)((Ov016KeyboardWork *)data_ov016_021de55c)->canvas[i][j]);
            if (i == 0) {
                DWCi_CELLlDelete(((Ov016KeyboardWork *)data_ov016_021de55c)->bgCell[j]);
            }
        }
    }
    for (i = 0; i < 2; i++) {
        DWCi_CELLlDelete(((Ov016KeyboardWork *)data_ov016_021de55c)->bg[i]);
    }
    for (i = 0; i < 4; i++) {
        DWCi_OBJlDelete(((Ov016KeyboardWork *)data_ov016_021de55c)->subCell[i]);
    }
    for (i = 0; i < 0x2f; i++) {
        DWCi_OBJlDelete(((Ov016KeyboardWork *)data_ov016_021de55c)->keyCell[i]);
    }
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de55c);
}
