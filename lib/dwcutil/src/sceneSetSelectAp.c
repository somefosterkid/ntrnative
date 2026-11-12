// libdwcutil's sceneSetSelectAp.c (Black's ov016 0x021cbc90-0x021cc998): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"


typedef struct SelectApWork {
    u32 *unk_0;
    s32 unk_4[3];
    s32 unk_10[3];
    u8 unk_1c;
    u8 unk_1d;
} SelectApWork;

typedef struct tagCOORD {
    u16 x;
    u16 y;
} COORD;

typedef struct tagRECT {
    u16 l;
    u16 t;
    u16 r;
    u16 b;
} RECT;

// The prebuilt's names in the comments
static u8 data_ov016_021de60c;  // FocusPos
static const u8 data_ov016_021da720[4] = { 0x13, 0x14, 0x12, 0x56 };  // CN_SET
static SelectApWork *data_ov016_021de610;  // MemSssa
static const s8 data_ov016_021da724[7] = { 3, 0x2d, 0x27, 0x1d, 0x32, 0x30, 6 };  // MSG_IDX
static const COORD data_ov016_021da72c[6] = {  // C_CELL
    { 0x08, 0x30 }, { 0x5a, 0x30 }, { 0xac, 0x30 }, { 0x0c, 0x58 }, { 0x5e, 0x58 }, { 0xb0, 0x58 },
};
static const RECT data_ov016_021da744[7] = {  // R_TOUCH
    { 0x08, 0x20, 0x54, 0x56 }, { 0x5a, 0x20, 0xa6, 0x56 }, { 0xac, 0x20, 0xf8, 0x56 }, { 0x08, 0x78, 0xf8, 0xa0 },
    { 0x08, 0x54, 0x54, 0x70 }, { 0x5a, 0x54, 0xa6, 0x70 }, { 0xac, 0x54, 0xf8, 0x70 },
};
static const RECT data_ov016_021da77c[7] = {  // R_FOCUS
    { 0x06, 0x1e, 0x46, 0x48 }, { 0x58, 0x1e, 0x98, 0x48 }, { 0xaa, 0x1e, 0xea, 0x48 }, { 0x06, 0x76, 0xea, 0x92 },
    { 0x09, 0x54, 0x43, 0x70 }, { 0x5b, 0x54, 0x95, 0x70 }, { 0xad, 0x54, 0xe7, 0x70 },
};

void initGraphics_sceneSetSelectAp(void);
void initObj(void);
void dispFocus_sceneSetSelectAp(void);
void func_ov016_021cbf00(void);
void func_ov016_021cbf68(void);
void procInit2_sceneSetSelectAp(void);
void procScene_sceneSetSelectAp(void);
void input_sceneSetSelectAp(void);
void draw_sceneSetSelectAp(void);
void button_sceneSetSelectAp(void);
void moveFocus_sceneSetSelectAp(s32 param_1);
void endScene1_sceneSetSelectAp(void);
void endScene2_sceneSetSelectAp(void);
void endScene_sceneSetSelectAp(void);
void dispPushClear(void);
void procErase1(void);

void DWCi_SceneSetSelectAp(void) {
    data_ov016_021de610 = DWCi_HEAPlAllocEx(0x20, 4);
    data_ov016_021de610->unk_1d = 0;
    initGraphics_sceneSetSelectAp();
    DWCi_HLlLoad(1);
    DWCi_COMMENTlDispEx(0x2f, -1, 0);
    DWCi_CMNlStep(0);
    initObj();
    dispFocus_sceneSetSelectAp();
    DWCi_ChangeScene((u32)func_ov016_021cbf00);
}

void initGraphics_sceneSetSelectAp(void) {
    char name[] = "char/ybBgStep11.ncl.l";

    DWCi_CMNlLoadResource("char/ybObjWay.ncl.l", (u8 *) GX_LoadOBJPltt);
    DWCi_CMNlLoadResource("char/jbBgStep1.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/jbBgStep1.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/jb2Ap.nsc.l", (u8 *) GX_LoadBG2Scr);
    data_ov016_021de610->unk_0 = DWCi_ARClReadEx((s8 *) DWCi_Char(name), NULL, 4);
    DWCi_GXlVisible(1, 0x10);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void initObj(void) {
    s32 type;
    s32 i;

    for (i = 0; i < 3; i++) {
        type = DWCi_SETTINGlGetSetType(i);
        if (type == 0xff) {
            type = 3;
        } else {
            s32 pos = i + 3;

            data_ov016_021de610->unk_10[i] = (s32)DWCi_CEINlSetExCell(0, 0x11, 1);
            func_ov016_021d5118(data_ov016_021de610->unk_10[i], -1, data_ov016_021da72c[pos].x, data_ov016_021da72c[pos].y);
            DWCi_CELLlSetPriority((void *)data_ov016_021de610->unk_10[i], -1, 3);
        }
        data_ov016_021de610->unk_4[i] = (s32)DWCi_CEINlSetExCell(0, data_ov016_021da720[type], 1);
        func_ov016_021d5118(data_ov016_021de610->unk_4[i], -1, data_ov016_021da72c[i].x, data_ov016_021da72c[i].y);
        DWCi_CELLlSetPriority((void *)data_ov016_021de610->unk_4[i], -1, 3);
    }
}

void func_ov016_021cbf00(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x14, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x14);
    DWCi_ChangeScene((u32)func_ov016_021cbf68);
}

void func_ov016_021cbf68(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_CMNlButton(0);
    DWCi_ChangeScene((u32)procInit2_sceneSetSelectAp);
}

void procInit2_sceneSetSelectAp(void) {
    BOOL BVar2;

    if (DWCi_BTNlGet() == -2) {
        return;
    }
    BVar2 = DWCi_STEPlGet();
    if (BVar2 == 1) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetSelectAp);
}

void procScene_sceneSetSelectAp(void) {
    input_sceneSetSelectAp();
    draw_sceneSetSelectAp();
    button_sceneSetSelectAp();
}

void input_sceneSetSelectAp(void) {
    BOOL result;
    u32 i;

    i = 0;
    do {
        result = DWCi_TPlCheckTrigger((u16 *)&data_ov016_021da744[i]);
        if (result) {
            func_ov016_021bcb28(1);
            data_ov016_021de60c = (u8)i;
            dispFocus_sceneSetSelectAp();
            return;
        }
        i = i + 1;
    } while (i < 7);
    result = DWCi_IPTlCheckTrigger(1);
    if (result) {
        func_ov016_021bcb28(1);
        return;
    }
    result = DWCi_IPTlCheckTrigger(2);
    if (result) {
        func_ov016_021bcb28(0);
        return;
    }
    result = DWCi_IPTlCheckRepeat(0x40);
    if (result) {
        moveFocus_sceneSetSelectAp(1);
        return;
    }
    result = DWCi_IPTlCheckRepeat(0x80);
    if (result) {
        moveFocus_sceneSetSelectAp(3);
        return;
    }
    result = DWCi_IPTlCheckRepeat(0x20);
    if (result) {
        moveFocus_sceneSetSelectAp(0);
        return;
    }
    result = DWCi_IPTlCheckRepeat(0x10);
    if (!result) {
        return;
    }
    moveFocus_sceneSetSelectAp(2);
}

void draw_sceneSetSelectAp(void) {
}

void procErase(void);
void dispPush_sceneSetSelectAp(void);

static inline void SelectApFail(s32 code) {
    DWCi_SNDlPlay(code);
    DWCi_BTNlSetForce(code - 10);
}
void button_sceneSetSelectAp(void) {
    u8 v;
    u32 idx;
    s32 t;

    switch (DWCi_BTNlGet()) {
    case 0:
        switch (DWCi_Entry()) {
        case 0:
            DWCi_SNDlPlay(7);
            data_ov016_021de610->unk_1d = 2;
            break;
        case 1:
            DWCi_BTNlDisable();
            DWCi_ChangeScene((u32)DWCi_SceneEnd);
            return;
        }
        break;
    case 1:
        v = 1;
        data_ov016_021de610->unk_1d = v;
        if (data_ov016_021de60c >= 4) {
            idx = data_ov016_021de60c - 4;
            if (DWCi_SETTINGlGetSetType(idx) == 0xff) {
                DWCi_SNDlPlay(9);
                DWCi_BTNlSetForce(-1);
                return;
            }
            DWCi_SNDlPlay(6);
            t = data_ov016_021da724[DWCi_Language()];
            DWCi_WINlInitEx(0x46, 0, v, t, idx + 1);
            dispPushClear();
            DWCi_BTNlDisable();
            DWCi_ChangeScene((u32)procErase);
            return;
        }
        if (data_ov016_021de60c <= 2) {
            DWCi_SETTINGlSetSetting(data_ov016_021de60c);
        }
        DWCi_SNDlPlay(6);
        dispPush_sceneSetSelectAp();
        break;
    default:
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetSelectAp);
}


void endScene_sceneSetSelectAp(void) {
    if (data_ov016_021de610->unk_1d == 2) {
        func_ov016_021d0580();
    }
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetSelectAp);
}

void endScene1_sceneSetSelectAp(void) {
    s32 result;

    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if ((data_ov016_021de610->unk_1d == 1) &&
          ((data_ov016_021de60c == 3 ||
            (result = DWCi_SETTIGNlGet(), *(u8 *)(result + 0xe7) != 0xff)))) {
        DWCi_BTNlEnd();
    }
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x14, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetSelectAp);
}

void endScene2_sceneSetSelectAp(void) {
    u32 i;

    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    i = 0;
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    if (DWCi_STEPlIsEnd() == 0) {
        return;
    }
    if (DWCi_BTNlIsEnd() == 0) {
        return;
    }
    do {
        if (data_ov016_021de610->unk_4[i] != 0) {
            DWCi_CELLlDelete((s32 *)data_ov016_021de610->unk_4[i]);
        }
        i++;
    } while (i < 3);
    i = 0;
    do {
        if (data_ov016_021de610->unk_10[i] != 0) {
            DWCi_CELLlDelete((s32 *)data_ov016_021de610->unk_10[i]);
        }
        i++;
    } while (i < 3);
    DWCi_FOCUSlDelete();
    DWCi_COMMENTlDelete();
    DWCi_ARClRelease((u32)data_ov016_021de610->unk_0);
    DWCi_CMNlLoadResource("char/ybObjMain.ncl.l", (u8 *) GX_LoadOBJPltt);
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x14);
    if (data_ov016_021de610->unk_1d == 2) {
        DWCi_SetParam_(0, 0);
        DWCi_ChangeScene((u32)DWCi_SceneMenu);
    } else {
        switch (data_ov016_021de60c) {
        case 0:
        case 1:
        case 2:
            DWCi_SetParam_(2, 0);
            if (((u8 *)DWCi_SETTIGNlGet())[0xe7] == 0xff) {
                DWCi_ChangeScene((u32)DWCi_SceneSetSelectWay);
            } else {
                DWCi_SetParam2_(0, 0);
                DWCi_SetListReturn(0);
                DWCi_ChangeScene((u32)DWCi_SceneSetList);
            }
            break;
        case 3:
            DWCi_SetParam_(2, 1);
            DWCi_ChangeScene((u32)DWCi_SceneSetUsbReady);
            break;
        }
    }
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de610);
}

void dispFocus_sceneSetSelectAp(void) {
    if (data_ov016_021de60c < 4) {
        DWCi_FOCUSlDisp4(data_ov016_021da77c[data_ov016_021de60c].l, data_ov016_021da77c[data_ov016_021de60c].r,
                         data_ov016_021da77c[data_ov016_021de60c].t, data_ov016_021da77c[data_ov016_021de60c].b);
        return;
    }
    DWCi_FOCUSlDisp(0, data_ov016_021da77c[data_ov016_021de60c].l, data_ov016_021da77c[data_ov016_021de60c].r,
                    data_ov016_021da77c[data_ov016_021de60c].t);
}

void moveFocus_sceneSetSelectAp(s32 dir) {
    s32 play = 1;
    u8 prev;
    int next;

    switch (data_ov016_021de60c) {
    case 0:
        data_ov016_021de610->unk_1c = 0;
        if (dir == 0) {
            data_ov016_021de60c = 2;
        } else if (dir == 2) {
            data_ov016_021de60c = 1;
        } else if (dir == 1) {
            data_ov016_021de60c = 3;
        } else {
            data_ov016_021de60c = 4;
        }
        break;
    case 1:
        data_ov016_021de610->unk_1c = 1;
        if (dir == 0) {
            data_ov016_021de60c = 0;
        } else if (dir == 2) {
            data_ov016_021de60c = 2;
        } else if (dir == 1) {
            data_ov016_021de60c = 3;
        } else {
            data_ov016_021de60c = 5;
        }
        break;
    case 2:
        data_ov016_021de610->unk_1c = 2;
        if (dir == 0) {
            data_ov016_021de60c = 1;
        } else if (dir == 2) {
            data_ov016_021de60c = 0;
        } else if (dir == 1) {
            data_ov016_021de60c = 3;
        } else {
            data_ov016_021de60c = 6;
        }
        break;
    case 3:
        prev = data_ov016_021de610->unk_1c;
        next = prev + 4;
        if (dir == 1) {
            data_ov016_021de60c = next;
        } else if (dir == 3) {
            data_ov016_021de60c = prev;
        } else {
            play = 0;
        }
        break;
    case 4:
        data_ov016_021de610->unk_1c = 0;
        if (dir == 0) {
            data_ov016_021de60c = 6;
        } else if (dir == 2) {
            data_ov016_021de60c = 5;
        } else if (dir == 1) {
            data_ov016_021de60c = 0;
        } else {
            data_ov016_021de60c = 3;
        }
        break;
    case 5:
        data_ov016_021de610->unk_1c = 1;
        if (dir == 0) {
            data_ov016_021de60c = 4;
        } else if (dir == 2) {
            data_ov016_021de60c = 6;
        } else if (dir == 1) {
            data_ov016_021de60c = 1;
        } else {
            data_ov016_021de60c = 3;
        }
        break;
    case 6:
        data_ov016_021de610->unk_1c = 2;
        if (dir == 0) {
            data_ov016_021de60c = 5;
        } else if (dir == 2) {
            data_ov016_021de60c = 4;
        } else if (dir == 1) {
            data_ov016_021de60c = 2;
        } else {
            data_ov016_021de60c = 3;
        }
        break;
    }
    if (play) {
        DWCi_SNDlPlay(8);
        dispFocus_sceneSetSelectAp();
    }
}


void dispPushClear(void) {
    u32 idx;

    idx = data_ov016_021de60c - 4;
    func_ov016_021d52d4(0, 0x32, (void *)DWCi_CELLlGetObj((void *)data_ov016_021de610->unk_10[idx], 0));
    func_ov016_021d5118(data_ov016_021de610->unk_10[idx], -1, data_ov016_021da72c[idx + 3].x, data_ov016_021da72c[idx + 3].y);
    DWCi_CELLlSetPriority((void *)data_ov016_021de610->unk_10[idx], -1, 3);
}

void procErase(void) {
    u32 idx;
    s32 ret;

    idx = data_ov016_021de60c - 4;
    ret = DWCi_WINlGet();
    switch (ret) {
    case 1:
        DWCi_SNDlPlay(0xe);
        DWCi_SETTINGlErasePage(idx);
        func_ov016_021d52d4(0, 0x56, (void *)DWCi_CELLlGetObj((void *)data_ov016_021de610->unk_4[idx], 0));
        func_ov016_021d5118(data_ov016_021de610->unk_4[idx], -1, data_ov016_021da72c[idx].x, data_ov016_021da72c[idx].y);
        DWCi_CELLlSetPriority((void *)data_ov016_021de610->unk_4[idx], -1, 3);
        DWCi_CELLlDelete((s32 *)data_ov016_021de610->unk_10[idx]);
        data_ov016_021de610->unk_10[idx] = 0;
        break;
    case 0:
        DWCi_SNDlPlay(7);
        func_ov016_021d52d4(0, 0x11, (void *)DWCi_CELLlGetObj((void *)data_ov016_021de610->unk_10[idx], 0));
        func_ov016_021d5118(data_ov016_021de610->unk_10[idx], -1, data_ov016_021da72c[idx + 3].x, data_ov016_021da72c[idx + 3].y);
        DWCi_CELLlSetPriority((void *)data_ov016_021de610->unk_10[idx], -1, 3);
        break;
    default:
        return;
    }
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procErase1);
}


void procErase1(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetSelectAp);
}

void dispPush_sceneSetSelectAp(void) {
    u8 a[4] = {3, 4, 5, 7};
    u8 b[4] = {10, 9, 11, 6};
    s32 i;

    if (data_ov016_021de60c > 3) {
        return;
    }
    DWCi_SCREENlCopyPalette((s32)data_ov016_021de610->unk_0, a[data_ov016_021de60c], a[data_ov016_021de60c]);
    if (data_ov016_021de60c == 3) {
        return;
    }
    i = DWCi_SETTINGlGetSetType(data_ov016_021de60c);
    if (i > 2) {
        i = 3;
    }
    DWCi_CELLlSetPalette((void *)data_ov016_021de610->unk_4[data_ov016_021de60c], -1, 0, b[i]);
}

