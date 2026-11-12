// libdwcutil's sceneSetEdit32.c (Black's ov016 0x021c6588-0x021c7088): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void procInit1_sceneSetEdit32(void);
void DWCi_KBlInit(void);
void procInit2_sceneSetEdit32(void);

// The object the pointer data_ov016_021de5e0 points to, by offset: a pointer to a display register (u32 *) at 4, then
// the data the state functions hand to the functions of their tables
typedef struct Ov016Work5e0 {
    NNSG2dCharCanvas *unk_0;
    u32 *unk_4;
    u8 unk_8[0x29 - 0x8];
    u8 unk_29;
    u8 unk_2a;
} Ov016Work5e0;

static Ov016Work5e0 *data_ov016_021de5e0;
void DWCi_SETTINGlSetWep(void *wep);
static void *data_ov016_021db450[2] = { DWCi_SETTINGlSetSsid, DWCi_SETTINGlSetWep };
void endScene3_sceneSetEdit32(void);
void procScene_sceneSetEdit32(void);
void input_sceneSetEdit32(void);
void draw_sceneSetEdit32(void);
void endScene1_sceneSetEdit32(void);
void endScene2_sceneSetEdit32(void);
void procError1_sceneSetEdit32(void);
extern s32 data_ov016_021da5a8;
extern s32 data_ov016_021da5c4;
static const u8 data_ov016_021da5b2[18] = { 8, 23, 38, 53, 68, 83, 98, 113, 128, 143, 158, 173, 188, 203, 218, 233, 0, 0 };
static const u8 data_ov016_021da5ac[6] = { 32, 49, 14, 0, 16, 0 };
void initGraphics_sceneSetEdit32(void);
void dispCaret_sceneSetEdit32(void);
void drawSsid(void);
void procInit_sceneSetEdit32(void);
void procError_sceneSetEdit32(void);
void procWin_sceneSetEdit32(void);
void endScene_sceneSetEdit32(void);
void procWin1_sceneSetEdit32(void);
s32 isCorrect_sceneSetEdit32(void);

void DWCi_SceneSetEdit32(void) {
    u8 MSG[2] = {0x3e, 0x3d};
    u32 prm;
    u32 sce;
    u16 *obj;

    data_ov016_021de5e0 = DWCi_HEAPlAllocEx(0x2C, 4);
    DWCi_GetParam2_(&prm, &sce);
    if (prm == 0) {
        DWCi_SETTINGlGetSsid(data_ov016_021de5e0->unk_8);
        data_ov016_021de5e0->unk_29 = DWCi_STRlNLen((s32)data_ov016_021de5e0->unk_8, 0x20);
    }
    initGraphics_sceneSetEdit32();
    DWCi_HLlLoad(prm + 9);
    if (sce == 1) {
        DWCi_COMMENTlDispEx(0x35, -1, 0);
    } else {
        DWCi_COMMENTlDispEx(MSG[prm], -1, 0);
    }
    DWCi_CMNlStep(2);
    data_ov016_021de5e0->unk_0 = DWCi_FNTlFormBgEx(0, 0);
    data_ov016_021de5e0->unk_4 = DWCi_CEINlSetExObj(NULL, 0x3E);
    obj = (u16 *)data_ov016_021de5e0->unk_4;
    obj[2] = (obj[2] & ~0xC00) | 0xC00;
    dispCaret_sceneSetEdit32();
    drawSsid();
    DWCi_ChangeScene((u32) procInit_sceneSetEdit32);
}

void initGraphics_sceneSetEdit32(void) {
    DWCi_CMNlLoadResource("char/ybObjKb.ncl.l", (u8 *) GX_LoadOBJPltt);
    DWCi_CMNlLoadResource("char/jbBgStep3.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/ybBgStep3.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/xb4Edit.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 2;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetEdit32(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetEdit32);
}

void procInit1_sceneSetEdit32(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_KBlInit();
    DWCi_SNDlPlay(0x14);
    if (data_ov016_021de5e0->unk_29 == 0) {
        DWCi_KBlSetErase(0);
    }
    if (data_ov016_021de5e0->unk_29 == 0x20) {
        DWCi_KBlSetInput(0);
    }
    DWCi_ChangeScene((u32)procInit2_sceneSetEdit32);
}

void procInit2_sceneSetEdit32(void) {
    if (DWCi_KBlGet() == 0xff) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneSetEdit32);
}

void procScene_sceneSetEdit32(void) {
    input_sceneSetEdit32();
    draw_sceneSetEdit32();
}

void input_sceneSetEdit32(void) {
    s32 key = DWCi_KBlGet();

    switch (key) {
    case 0x80:
        if (data_ov016_021de5e0->unk_29 == 0) {
            break;
        }
        DWCi_SNDlPlay(3);
        data_ov016_021de5e0->unk_29 -= 1;
        data_ov016_021de5e0->unk_8[data_ov016_021de5e0->unk_29] = 0;
        if (data_ov016_021de5e0->unk_29 == 0) {
            DWCi_KBlSetErase(0);
        }
        DWCi_KBlSetInput(1);
        break;
    case 0x82:
        DWCi_SNDlPlay(7);
        data_ov016_021de5e0->unk_2a = 0;
        DWCi_ChangeScene((u32)endScene_sceneSetEdit32);
        return;
    case 0x83:
        if (isCorrect_sceneSetEdit32() != 0) {
            DWCi_SNDlPlay(6);
            data_ov016_021de5e0->unk_2a = 1;
        } else {
            data_ov016_021de5e0->unk_2a = 2;
            DWCi_SNDlPlay(9);
        }
        *data_ov016_021de5e0->unk_4 = (*data_ov016_021de5e0->unk_4 & 0xc1fffcff) | 0x200;
        DWCi_ChangeScene((u32)endScene_sceneSetEdit32);
        return;
    case 0:
        break;
    case 0xe01d:
    default:
        if (data_ov016_021de5e0->unk_29 == 0x20) {
            break;
        }
        DWCi_SNDlPlay(1);
        data_ov016_021de5e0->unk_8[data_ov016_021de5e0->unk_29] = key;
        data_ov016_021de5e0->unk_29 += 1;
        DWCi_KBlSetErase(1);
        if (data_ov016_021de5e0->unk_29 == 0x20) {
            DWCi_KBlSetInput(0);
        }
        break;
    }
    drawSsid();
    dispCaret_sceneSetEdit32();
}


void draw_sceneSetEdit32(void) {
}

void endScene_sceneSetEdit32(void) {
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetEdit32);
}

void endScene1_sceneSetEdit32(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_KBlEnd();
    DWCi_SNDlPlay(0x15);
    DWCi_ChangeScene((u32)endScene2_sceneSetEdit32);
}

void endScene2_sceneSetEdit32(void) {
    s32 MSG[2] = {71, 72};
    s32 sce;

    if (DWCi_KBlIsInit() != 0) {
        return;
    }
    if (data_ov016_021de5e0->unk_2a == 0) {
        DWCi_ChangeScene((u32) endScene3_sceneSetEdit32);
        return;
    }
    if (data_ov016_021de5e0->unk_2a == 2) {
        DWCi_WINlInitEx(6, 3, 1, -1, 0);
        DWCi_ChangeScene((u32) procError_sceneSetEdit32);
        return;
    }
    DWCi_GetParam2_(0, &sce);
    DWCi_WINlInitEx(MSG[sce], 2, 1, -1, 0);
    DWCi_ChangeScene((u32) procWin_sceneSetEdit32);
}

void endScene3_sceneSetEdit32(void) {
    u32 sp4;
    u32 sp0;
    void (*fn)(void);

    DWCi_COMMENTlDelete();
    DWCi_OBJlDelete(data_ov016_021de5e0->unk_4);
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_CMNlLoadResource("char/ybObjMain.ncl.l", (u8 *)GX_LoadOBJPltt);
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x15);
    DWCi_GetParam2_(&sp4, &sp0);
    if (sp0 == 0) {
        DWCi_SetParam_(2, 1);
        DWCi_SetParam2_(0, sp4);
        fn = DWCi_SceneSetList;
    } else if (data_ov016_021de5e0->unk_2a == 0) {
        DWCi_SetParam_(0, 1);
        DWCi_SetParam2_(1, 0);
        fn = DWCi_SceneSetApList;
    } else {
        DWCi_SetParam_(0, 0);
        DWCi_SetParam2_(0, 1);
        fn = DWCi_SceneSetTestConfirm;
    }
    DWCi_ChangeScene((u32)fn);
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de5e0);
}


typedef struct tagAREA {
    u16 l;
    u16 t;
    u16 w;
    u16 h;
} tagAREA;
void drawSsid(void) {
    tagAREA a = {0};
    s32 x;
    s32 i = 0;
    u16 buf[2];
    u8 c;

    a.t = 0x20;
    a.w = 0xe;
    a.h = 0x10;
    DWCi_FNTlClear(data_ov016_021de5e0->unk_0, 0);
    x = 0;
    buf[1] = 0;
    for (; i < 0x20; i++, x++) {
        if (i == 0x10) {
            x = 0;
            a.t = 0x31;
        }
        c = data_ov016_021de5e0->unk_8[i];
        if (c == 0x20) {
            buf[0] = 0xe01d;
        } else {
            buf[0] = c;
        }
        a.l = data_ov016_021da5b2[x];
        DWCi_FNTlDrawRect(data_ov016_021de5e0->unk_0, a.l, a.t, a.w, a.h, 2, 0x480, buf);
    }
    DWCi_FNTlRenewBg((s32)data_ov016_021de5e0->unk_0);
}

void dispCaret_sceneSetEdit32(void) {
    u8 val;
    u32 *ptr;
    s32 val2;
    s32 val3;


    val = data_ov016_021de5e0->unk_29;
    ptr = data_ov016_021de5e0->unk_4;
    val2 = val & 0xF;
    val3 = (s32) val >> 4;
    if ((u32) val >= 0x20U) {
        val3 = 1;
        val2 = 0xF;
    }
    *ptr = (*ptr & 0xFE00FF00) | data_ov016_021da5ac[val3] | ((u32) (data_ov016_021da5b2[val2] << 0x17) >> 7);
}

void procWin_sceneSetEdit32(void) {
    data_ov016_021de5e0->unk_2a = DWCi_WINlGet();
    switch (data_ov016_021de5e0->unk_2a) {
    case 0:
        DWCi_SNDlPlay(7);
        break;
    case 1:
        DWCi_SNDlPlay(0xe);
        break;
    default:
        return;
    }
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procWin1_sceneSetEdit32);
}


void procWin1_sceneSetEdit32(void)
{
    void *auStack_14[2];
    u32 uStack_18;
    void **pStack;

    auStack_14[0] = data_ov016_021db450[0];
    auStack_14[1] = data_ov016_021db450[1];
    pStack = auStack_14;
    if (DWCi_WINlIsInit() != FALSE) {
        return;
    }
    if (data_ov016_021de5e0->unk_2a == 0) {
        *data_ov016_021de5e0->unk_4 = *data_ov016_021de5e0->unk_4 & 0xc1fffcff;
        DWCi_ChangeScene((u32)procInit1_sceneSetEdit32);
    } else {
        DWCi_GetParam2_(&uStack_18, NULL);
        ((void (*)(void *))pStack[uStack_18])(data_ov016_021de5e0->unk_8);
        DWCi_ChangeScene((u32)endScene3_sceneSetEdit32);
    }
}

s32 isCorrect_sceneSetEdit32(void) {
    s32 prm;
    s32 sce;
    s32 i;
    u8 c;
    s32 len;

    DWCi_GetParam2_((u32 *)&prm, (u32 *)&sce);
    if (sce == 1) {
        DWCi_COMMENTlDispEx(0x35, -1, 0);
    }
    DWCi_GetParam2_((u32 *)&prm, (u32 *)&sce);
    if (prm == 0) {
        return data_ov016_021de5e0->unk_8[0] != 0;
    }
    if (sce == 1 && data_ov016_021de5e0->unk_8[0] == 0) {
        return 0;
    }
    len = DWCi_STRlNLen((s32)data_ov016_021de5e0->unk_8, 0x20);
    switch (len) {
    case 0:
    case 5:
    case 13:
    case 16:
        return 1;
    case 10:
    case 26:
    case 32:
        for (i = 0; i < len; i++) {
            c = data_ov016_021de5e0->unk_8[i];
            if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))) {
                return 0;
            }
        }
        return 1;
    default:
        return 0;
    }
}

void procError_sceneSetEdit32(void) {
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procError1_sceneSetEdit32);
}

void procError1_sceneSetEdit32(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    *data_ov016_021de5e0->unk_4 = *data_ov016_021de5e0->unk_4 & 0xc1fffcff;
    DWCi_ChangeScene((u32)procInit1_sceneSetEdit32);
}
