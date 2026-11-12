// libdwcutil's sceneSetEditAddress.c (Black's ov016 0x021c7088-0x021c7ce4): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"


// The object the pointer data_ov016_021de5e4 points to, by offset: a pointer to a display register (u32 *) at 4, then
// the data the state functions hand to the functions of their tables
typedef struct Ov016Work5e4 {
    NNSG2dCharCanvas *canvas;
    u32 *unk_4;
    u8 unk_8[0x14 - 0x8];
    u8 unk_14;
    u8 unk_15;
} Ov016Work5e4;

typedef struct tagCOORD {
    u16 x;
    u16 y;
} COORD;
typedef struct tagCOORD SIZE;

void procInit1_sceneSetEditAddress(void);
void procScene_sceneSetEditAddress(void);
void input_sceneSetEditAddress(void);
void draw_sceneSetEditAddress(void);
void endScene1_sceneSetEditAddress(void);
void endScene2_sceneSetEditAddress(void);
void endScene3_sceneSetEditAddress(void);
void procError_sceneSetEditAddress(void);
void procWin_sceneSetEditAddress(void);
// The prebuilt's names in the comments
static const u8 data_ov016_021da5d4[12] = {  // X_IPT: where each digit is drawn
    0x31, 0x3d, 0x49, 0x5a, 0x66, 0x72, 0x83, 0x8f, 0x9b, 0xac, 0xb8, 0xc4,
};
static const SIZE data_ov016_021da5d0 = { 11, 16 };  // S_IPT: the size of a digit
static Ov016Work5e4 *data_ov016_021de5e4;
void procError1_sceneSetEditAddress(void);
void DWCi_KB10lInit(void);
BOOL isOver(s32 param_1);
void procInit2_sceneSetEditAddress(void);
void drawAddress_sceneSetEditAddress(void);
void initGraphics_sceneSetEditAddress(void);
void procInit_sceneSetEditAddress(void);
void endScene_sceneSetEditAddress(void);
void dispCaret_sceneSetEditAddress(void);
void procWin1_sceneSetEditAddress(void);
void arrangeAddress(void);
BOOL isCorrect_sceneSetEditAddress(void);


void DWCi_SceneSetEditAddress(void) {
    void (*tbl[5])(u8 *) = {DWCi_SETTINGlGetIp, DWCi_SETTINGlGetNetMask, DWCi_SETTINGlGetGateway, DWCi_SETTINGlGetDnsP, DWCi_SETTINGlGetDnsS};
    u8 buf[5] = {0x3f, 0x40, 0x41, 0x42, 0x43};
    s32 idx;
    Ov016Work5e4 *w;

    data_ov016_021de5e4 = DWCi_HEAPlAllocEx(0x18, 4);
    DWCi_GetParam2_((u32 *)&idx, NULL);
    tbl[idx](data_ov016_021de5e4->unk_8);
    w = data_ov016_021de5e4;
    if (memcmp(w->unk_8, "  0", 3) == 0) {
        MI_CpuFill8(w->unk_8, 0, 12);
        data_ov016_021de5e4->unk_14 = 0;
    } else {
        w->unk_14 = 3;
    }
    initGraphics_sceneSetEditAddress();
    DWCi_HLlLoad(idx + 11);
    DWCi_COMMENTlDispEx(buf[idx], -1, 0);
    DWCi_CMNlStep(2);
    data_ov016_021de5e4->canvas = DWCi_FNTlFormBgEx(0, 0);
    data_ov016_021de5e4->unk_4 = DWCi_CEINlSetExObj(NULL, 0x3f);
    ((u16 *)data_ov016_021de5e4->unk_4)[2] = (((u16 *)data_ov016_021de5e4->unk_4)[2] & ~0xc00) | 0xc00;
    dispCaret_sceneSetEditAddress();
    drawAddress_sceneSetEditAddress();
    DWCi_ChangeScene((u32)procInit_sceneSetEditAddress);
}

void initGraphics_sceneSetEditAddress(void) {
    DWCi_CMNlLoadResource("char/ybObjKb.ncl.l", (u8 *) GX_LoadOBJPltt);
    DWCi_CMNlLoadResource("char/jbBgStep3.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/ybBgStep3.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/xb4EditAddr.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 2;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetEditAddress(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetEditAddress);
}

void procInit1_sceneSetEditAddress(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_KB10lInit();
    DWCi_SNDlPlay(0x14);
    if (data_ov016_021de5e4->unk_14 == 0) {
        DWCi_KB10lSetErase(0);
        DWCi_KB10lSetShift(0);
    } else {
        if (isOver(0x1A) != 0) {
            DWCi_KB10lSetInput(0);
        }
        DWCi_KB10lSetShift(0);
    }
    DWCi_ChangeScene((u32) procInit2_sceneSetEditAddress);
}

void procInit2_sceneSetEditAddress(void) {
    if (DWCi_KB10lGet() == 0x1f) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneSetEditAddress);
}

void procScene_sceneSetEditAddress(void) {
    input_sceneSetEditAddress();
    draw_sceneSetEditAddress();
}

void input_sceneSetEditAddress(void) {
    s32 c = DWCi_KB10lGet();
    u8 *tbl;
    s32 i;

    switch (c) {
    case 0:
        return;
    case 16:
        if (data_ov016_021de5e4->unk_14 == 0 && data_ov016_021de5e4->unk_8[2] == 0) {
            break;
        }
        DWCi_SNDlPlay(3);
        if (data_ov016_021de5e4->unk_8[data_ov016_021de5e4->unk_14 * 3 + 2] == 0) {
            data_ov016_021de5e4->unk_14 = data_ov016_021de5e4->unk_14 - 1;
        }
        MI_CpuFill8(&data_ov016_021de5e4->unk_8[data_ov016_021de5e4->unk_14 * 3], 0, 3);
        if (data_ov016_021de5e4->unk_14 == 0 && data_ov016_021de5e4->unk_8[2] == 0) {
            DWCi_KB10lSetErase(0);
        }
        DWCi_KB10lSetInput(1);
        DWCi_KB10lSetShift(0);
        break;
    case 17:
        if (data_ov016_021de5e4->unk_14 >= 3 || data_ov016_021de5e4->unk_8[data_ov016_021de5e4->unk_14 * 3 + 2] == 0) {
            break;
        }
        DWCi_SNDlPlay(1);
        data_ov016_021de5e4->unk_14++;
        DWCi_KB10lSetShift(0);
        break;
    case 18:
        data_ov016_021de5e4->unk_15 = 0;
        DWCi_SNDlPlay(7);
        DWCi_ChangeScene((u32)endScene_sceneSetEditAddress);
        return;
    case 19:
        if (isCorrect_sceneSetEditAddress()) {
            DWCi_SNDlPlay(6);
            data_ov016_021de5e4->unk_15 = 1;
        } else {
            data_ov016_021de5e4->unk_15 = 2;
            DWCi_SNDlPlay(9);
        }
        data_ov016_021de5e4->unk_14 = 3;
        *data_ov016_021de5e4->unk_4 = (*data_ov016_021de5e4->unk_4 & 0xc1fffcff) | 0x200;
        dispCaret_sceneSetEditAddress();
        arrangeAddress();
        DWCi_ChangeScene((u32)endScene_sceneSetEditAddress);
        return;
    default:
        if (data_ov016_021de5e4->unk_14 == 3 && isOver(0x1a)) {
            break;
        }
        DWCi_SNDlPlay(1);
        tbl = data_ov016_021de5e4->unk_8;
        i = data_ov016_021de5e4->unk_14 * 3;
        if (tbl[i + 2] == 0) {
            tbl[i + 2] = c;
        } else {
            if (tbl[i + 1] == 0) {
                tbl[i + 1] = tbl[i + 2];
                tbl[i + 2] = c;
                if (!isOver(0x1a)) {
                    goto after;
                }
            } else {
                tbl[i] = tbl[i + 1];
                tbl[i + 1] = tbl[i + 2];
                tbl[i + 2] = c;
            }
            if (data_ov016_021de5e4->unk_14 < 3) {
                data_ov016_021de5e4->unk_14++;
            }
        }
    after:
        DWCi_KB10lSetErase(1);
        if (data_ov016_021de5e4->unk_14 < 3) {
            DWCi_KB10lSetShift(1);
        } else {
            DWCi_KB10lSetShift(0);
        }
        if (data_ov016_021de5e4->unk_14 == 3 && isOver(0x1a)) {
            DWCi_KB10lSetInput(0);
        }
        break;
    }
    drawAddress_sceneSetEditAddress();
    dispCaret_sceneSetEditAddress();
}


BOOL isOver(s32 val) {
    u8 buf[4];
    s32 i;
    u8 *p = &data_ov016_021de5e4->unk_8[data_ov016_021de5e4->unk_14 * 3];

    if (*p != 0 && *p != 0x20) {
        return TRUE;
    }
    MI_CpuCopy8(p, buf, 3);
    buf[3] = 0;
    for (i = 0; i < 3; i++) {
        if (buf[i] != 0) {
            break;
        }
        buf[i] = ' ';
    }
    return atol((char *)buf) >= val;
}


void draw_sceneSetEditAddress(void) {
}

void endScene_sceneSetEditAddress(void) {
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetEditAddress);
}

void endScene1_sceneSetEditAddress(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_KB10lEnd();
    DWCi_SNDlPlay(0x15);
    DWCi_ChangeScene((u32)endScene2_sceneSetEditAddress);
}

void endScene2_sceneSetEditAddress(void) {
    BOOL BVar1;

    BVar1 = DWCi_KB10lIsInit();
    if (BVar1 != 0) {
        return;
    }
    if (data_ov016_021de5e4->unk_15 == 0) {
        DWCi_ChangeScene((u32)endScene3_sceneSetEditAddress);
        return;
    }
    if (data_ov016_021de5e4->unk_15 == 2) {
        DWCi_WINlInitEx(6, 3, 1, -1, 0);
        DWCi_ChangeScene((u32)procError_sceneSetEditAddress);
        return;
    }
    DWCi_WINlInitEx(0x47, 2, 1, -1, 0);
    DWCi_ChangeScene((u32)procWin_sceneSetEditAddress);
}

void endScene3_sceneSetEditAddress(void) {
    s32 sp0;
    s32 v;

    DWCi_COMMENTlDelete();
    DWCi_OBJlDelete(data_ov016_021de5e4->unk_4);
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_CMNlLoadResource("char/ybObjMain.ncl.l", (u8 *)GX_LoadOBJPltt);
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x15);
    DWCi_GetParam2_((u32 *)&sp0, NULL);
    v = sp0;
    if (v >= 3) {
        v++;
        sp0 = v;
    }
    DWCi_SetParam_(2, 1);
    DWCi_SetParam2_(0, v + 3);
    DWCi_ChangeScene((u32)DWCi_SceneSetList);
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de5e4);
}


typedef struct tagAREA {
    u16 l;
    u16 t;
    u16 w;
    u16 h;
} tagAREA;

void drawAddress_sceneSetEditAddress(void) {
    tagAREA a = {0, 41};
    s32 i = 0;
    u16 buf[2];

    a.w = data_ov016_021da5d0.x;
    a.h = data_ov016_021da5d0.y;
    DWCi_FNTlClear(data_ov016_021de5e4->canvas, 0);
    buf[1] = 0;
    for (; i < 12; i++) {
        buf[0] = data_ov016_021de5e4->unk_8[i];
        a.l = data_ov016_021da5d4[i];
        DWCi_FNTlDrawRect(data_ov016_021de5e4->canvas, a.l, a.t, a.w, a.h, 2, 0x480, buf);
    }
    DWCi_FNTlRenewBg((s32)data_ov016_021de5e4->canvas);
}

void dispCaret_sceneSetEditAddress(void) {
    s32 group = data_ov016_021de5e4->unk_14;

    if (group > 3) {
        group = 3;
    }
    G2_SetOBJPosition((GXOamAttr *)data_ov016_021de5e4->unk_4, data_ov016_021da5d4[group * 3 + 2], 0x28);
}

void procWin_sceneSetEditAddress(void) {
    data_ov016_021de5e4->unk_15 = DWCi_WINlGet();
    switch (data_ov016_021de5e4->unk_15) {
    case 0:
        DWCi_SNDlPlay(7);
        break;
    case 1:
        DWCi_SNDlPlay(14);
        break;
    default:
        return;
    }
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procWin1_sceneSetEditAddress);
}

void procWin1_sceneSetEditAddress(void)
{
    void (*FUNC[5])(const u8 *) = {DWCi_SETTINGlSetIp, DWCi_SETTINGlSetNetMask, DWCi_SETTINGlSetGateway, DWCi_SETTINGlSetDnsP, DWCi_SETTINGlSetDnsS};
    u32 prm;

    if (DWCi_WINlIsInit() != FALSE) {
        return;
    }
    if (data_ov016_021de5e4->unk_15 == 0) {
        *data_ov016_021de5e4->unk_4 = *data_ov016_021de5e4->unk_4 & 0xc1fffcff;
        DWCi_ChangeScene((u32)procInit1_sceneSetEditAddress);
    } else {
        DWCi_GetParam2_(&prm, NULL);
        FUNC[prm](data_ov016_021de5e4->unk_8);
        DWCi_ChangeScene((u32)endScene3_sceneSetEditAddress);
    }
}

void arrangeAddress(void) {
    u8 *c;
    s32 i;
    s32 j;

    for (i = 0; i < 4; i++) {
        c = data_ov016_021de5e4->unk_8 + i * 3;
        for (j = 0; j < 3; j++) {
            if (c[j] != '0' && c[j] != ' ' && c[j] != 0) {
                break;
            }
            c[j] = (j == 2) ? '0' : ' ';
        }
    }
    drawAddress_sceneSetEditAddress();
}

BOOL isCorrect_sceneSetEditAddress(void) {
    u8 VAL[4] = {0x32, 0x35, 0x35, 0};
    u8 *c;
    s32 prm;
    s32 i;
    s32 j;
    u8 buf[4];
    u8 b;

    for (i = 0; i < 4; i++) {
        c = data_ov016_021de5e4->unk_8 + i * 3;
        if (c[0] != ' ') {
            for (j = 0; j < 3; j++) {
                if (c[j] > VAL[j]) {
                    return FALSE;
                }
                if (c[j] < VAL[j]) {
                    break;
                }
            }
        }
    }
    DWCi_SETTINGlConvAddress(data_ov016_021de5e4->unk_8, buf);
    b = FALSE;
    DWCi_GetParam2_((u32 *)&prm, NULL);
    if (prm == 1) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 8; j++) {
                if (b) {
                    if (buf[i] & (1 << (7 - j))) {
                        return FALSE;
                    }
                } else if (!(buf[i] & (1 << (7 - j)))) {
                    b = TRUE;
                }
            }
        }
        return TRUE;
    }
    if (DWC_BACKUPlCheckAddress(buf)) {
        b = TRUE;
    }
    return b;
}


void procError_sceneSetEditAddress(void) {
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procError1_sceneSetEditAddress);
}

void procError1_sceneSetEditAddress(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    *data_ov016_021de5e4->unk_4 = *data_ov016_021de5e4->unk_4 & 0xc1fffcff;
    DWCi_ChangeScene((u32)procInit1_sceneSetEditAddress);
}
