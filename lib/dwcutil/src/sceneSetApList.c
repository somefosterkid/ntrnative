// libdwcutil's sceneSetApList.c (Black's ov016 0x021c50d4-0x021c6588): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "nitro/fx/fx_cp.h"

void DWCi_SBlInit(s32 param_1, u8 param_2, u32 param_3, s32 param_4, s32 param_5);
void func_ov016_021c5550(void);
void procInit2_sceneSetApList(void);
void procScene_sceneSetApList(void);
void input_sceneSetApList(void);
void draw_sceneSetApList(void);
void button_sceneSetApList(void);
void endScene1_sceneSetApList(void);
void endScene2_sceneSetApList(void);
void setOffset_sceneSetApList(void);
void drawApInfo(void);
void dispFocus_sceneSetApList(void);
void procError1_sceneSetApList(void);
void scrollDown(void);
void scrollUp_sceneSetApList(void);
void moveFocus_sceneSetApList(s32 param_1);
void initGraphics_sceneSetApList(void);
void initScrollBar(void);
void func_ov016_021c5388(void);
void taskVBlank_sceneSetApList(void);
void func_ov016_021c54e8(void);
void taskUp_sceneSetApList(u32 param_1);
extern s32 data_ov016_021da574;
extern u8 data_ov016_021da58c;
void taskDown_sceneSetApList(u32 param_1);

typedef struct Ov016Unk21de5d8Obj {
    u8 unk_0[4];
    u16 unk_4;
} Ov016Unk21de5d8Obj;

typedef struct Ov016Unk21de5d8Rec {
    u8 unk_0[0x28];
    u8 unk_28;
    u8 unk_29;
} Ov016Unk21de5d8Rec;

typedef struct Ov016Unk21de5d8Work {
    Ov016Unk21de5d8Rec *unk_0;
    u32 *unk_4;
    u32 *unk_8;
    NNSG2dCharCanvas *unk_c;
    Ov016Unk21de5d8Obj *unk_10[5];
    Ov016Unk21de5d8Obj *unk_24[5];
    s32 *unk_38;
    u8 unk_3c[4];
    u16 unk_40;
    u16 unk_42[3];
    u16 unk_48[4];
    u8 unk_50;
    u8 unk_51;
    u8 unk_52;
    u8 unk_53;
    u8 unk_54;
    u8 unk_55;
    u8 unk_56;
    u8 unk_57;
    u8 unk_58;
    u8 unk_59;
} Ov016Unk21de5d8Work;

typedef struct Ov016Unk21de5d8 {
    u8 unk_0;
    u16 unk_2;
    Ov016Unk21de5d8Work *work;
} Ov016Unk21de5d8;

Ov016Unk21de5d8 data_ov016_021de5d8;

#define OV016_DATA_21DE5D8 (&data_ov016_021de5d8)

const u8 data_ov016_021da51c[3] = { 0x2e, 0x2d, 0x33 };
const u8 data_ov016_021da51f[5] = { 0x18, 0x17, 0x16, 0x15, 0x00 };
void endScene_sceneSetApList(void);
void drapApObj(s32 param_1, s32 param_2);
void procError_sceneSetApList(void);


static const u16 data_ov016_021da524[20] = { 7, 0x32, 0xd0, 0x4c, 7, 0x4e, 0xd0, 0x68, 7, 0x6a, 0xd0, 0x84, 7, 0x86, 0xd0, 0xa0, 0x85, 0x1b, 0xfd, 0x2c };
static const u16 data_ov016_021da54c[20] = { 4, 0x2e, 219, 63, 4, 74, 219, 91, 4, 102, 219, 119, 4, 130, 219, 147, 130, 24, 240, 44 };

void DWCi_SceneSetApList(void) {
    u32 sp0;

    *(s32 **)((u8 *)((void *)&data_ov016_021de5d8) + 4) = DWCi_HEAPlAllocEx(0x5CU, 4);
    DWCi_GetParam2_(&sp0, NULL);
    if (sp0 == 0) {
        *(u8 *)((void *)&data_ov016_021de5d8) = 0;
        *(s16 *)((u8 *)((void *)&data_ov016_021de5d8) + 2) = 0;
    }
    *(s8 *)((u8 *)(*(s32 **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x51) = (s8) func_ov016_021cf114(*(s32 **)((u8 *)((void *)&data_ov016_021de5d8) + 4));
    initGraphics_sceneSetApList();
    DWCi_HLlLoadSet();
    DWCi_COMMENTlDispEx(0x34U, -1, 0);
    DWCi_CMNlStep(2);
    initScrollBar();
    func_ov016_021c5388();
    *(NNSG2dCharCanvas **)((u8 *)(*(s32 **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0xC) = DWCi_FNTlFormBgEx(0, 0);
    *(s32 **)((u8 *)(*(s32 **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x3C) = DWCi_TSKlForm(1, (s32) taskVBlank_sceneSetApList, 0, 0x6EU);
    drawApInfo();
    dispFocus_sceneSetApList();
    DWCi_ChangeScene((u32) func_ov016_021c54e8);
}

void initGraphics_sceneSetApList(void) {
    u8 SCR_BACK[25] = "char/xb4ApListBack.nsc.l";
    u8 PLLT_PUSH[22] = "char/ybBgStep31.ncl.l";
    u8 *n;

    DWCi_CMNlLoadResource("char/jb4ApList.nsc.l", (u8 *)GX_LoadBG3Scr);
    n = DWCi_Char((s8 *)SCR_BACK);
    data_ov016_021de5d8.work->unk_4 = DWCi_ARClReadEx((s8 *)n, NULL, 4);
    DWCi_SCREENlInit(data_ov016_021de5d8.work->unk_4);
    DWCi_SCREENlRenew();
    n = DWCi_Char((s8 *)PLLT_PUSH);
    data_ov016_021de5d8.work->unk_8 = DWCi_ARClReadEx((s8 *)n, NULL, 4);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(2);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(2);
}

void initScrollBar(void) {
    s32 quotient;
    s32 val;

    quotient = 0;
    *(u16 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x40) = (*(u8 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x51) - 4) * 0x1c;
    if (*(u8 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x51) <= 4) {
        val = 0;
        *(u8 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x53) = 0;
    }
    else if (*(u8 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x51) <= 8) {
        *(u8 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x53) = 0x1f;
        val = 1;
    }
    else {
        *(u8 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x53) = 0x37;
        val = 2;
    }
    if (val != 0) {
        quotient = FX_DivS32((u32)((u16 *)&data_ov016_021de5d8)[1] * (u32)*(u8 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x53),
                                            (u32)*(u16 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x40));
    }
    DWCi_SBlInit(val, 0x55, 0xec, 0x3f, quotient);
}

void func_ov016_021c5388(void) {
    s32 n;
    s32 i;
    u32 m;
    u32 k;

    n = OV016_DATA_21DE5D8->work->unk_51;
    if (n > 5) {
        n = 5;
    }
    for (i = 0; i < n; i++) {
        OV016_DATA_21DE5D8->work->unk_10[i] = DWCi_CEINlSetExObj(NULL, 0x2e);
        OV016_DATA_21DE5D8->work->unk_24[i] = DWCi_CEINlSetExObj(NULL, 0x18);
    }
    for (k = 0; k < 3; k++) {
        func_ov016_021d52d4(0, data_ov016_021da51c[k], OV016_DATA_21DE5D8->work->unk_10[0]);
        OV016_DATA_21DE5D8->work->unk_42[k] = OV016_DATA_21DE5D8->work->unk_10[0]->unk_4 & 0x3ff;
    }
    for (m = 0; m < 4; m++) {
        func_ov016_021d52d4(0, data_ov016_021da51f[m], OV016_DATA_21DE5D8->work->unk_24[0]);
        OV016_DATA_21DE5D8->work->unk_48[m] = OV016_DATA_21DE5D8->work->unk_24[0]->unk_4 & 0x3ff;
    }
    for (i = 0; i < n; i++) {
        OV016_DATA_21DE5D8->work->unk_10[i]->unk_4 = (OV016_DATA_21DE5D8->work->unk_10[i]->unk_4 & ~0xc00) | 0xc00;
        OV016_DATA_21DE5D8->work->unk_24[i]->unk_4 = (OV016_DATA_21DE5D8->work->unk_24[i]->unk_4 & ~0xc00) | 0xc00;
    }
}


void func_ov016_021c54e8(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x1d, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x1d);
    DWCi_ChangeScene((u32)func_ov016_021c5550);
}

void func_ov016_021c5550(void) {
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
    DWCi_ChangeScene((u32)procInit2_sceneSetApList);
}

void procInit2_sceneSetApList(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetApList);
}

void procScene_sceneSetApList(void) {
    input_sceneSetApList();
    draw_sceneSetApList();
    button_sceneSetApList();
}

void input_sceneSetApList(void) {
    s32 i;
    s32 j;

    if ((*(s32 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x38) == 0) && (*(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x57) == 0)) {
        if (DWCi_TPlCheckTrigger((u16 *) (void *)&data_ov016_021daa7c) != 0) {
            *(s8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x50) = -1;
            i = 0;
loop_6:
            if (DWCi_TPlCheckTrigger((u16 *)((u8 *)&data_ov016_021da524 + i * 8)) != 0) {
                if (i < 4) {
                    *(s8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x50) = (s8) i;
                    goto block_11;
                }
                func_ov016_021bcb28(1U);
                *(u8 *)((void *)&data_ov016_021de5d8) = (u8) i;
                dispFocus_sceneSetApList();
                return;
            }
            i += 1;
            if ((u32) i >= 5U) {
                goto block_11;
            }
            goto loop_6;
        }
block_11:
        if (DWCi_TPlCheckRelease((u16 *) (void *)&data_ov016_021daa7c) != 0) {
            j = 0;
loop_13:
            if (DWCi_TPlCheckRelease((u16 *)((u8 *)&data_ov016_021da524 + j * 8)) != 0) {
                if (*(s8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x50) == j) {
                    if (j >= (s32) *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x51)) {
                        DWCi_SNDlPlay(9);
                        goto block_19;
                    }
                    func_ov016_021bcb28(1U);
                    *(u8 *)((void *)&data_ov016_021de5d8) = (u8) j;
                    dispFocus_sceneSetApList();
                    return;
                }
                goto block_19;
            }
            j += 1;
            if (j >= 4) {
                goto block_19;
            }
            goto loop_13;
        }
block_19:
        if (DWCi_IPTlCheckTrigger(1U) != 0) {
            func_ov016_021bcb28(1U);
            DWCi_SBlDisable();
            return;
        }
        if (DWCi_IPTlCheckTrigger(2U) != 0) {
            func_ov016_021bcb28(0U);
            return;
        }
        if (DWCi_IPTlCheckRepeat(0x200U) != 0) {
            scrollDown();
            return;
        }
        if (DWCi_IPTlCheckRelease(0x200U) != 0) {
            *(s8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x59) = 0;
            return;
        }
        if (DWCi_IPTlCheckRepeat(0x100U) != 0) {
            scrollUp_sceneSetApList();
            return;
        }
        if (DWCi_IPTlCheckRelease(0x100U) != 0) {
            *(s8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x59) = 0;
            return;
        }
        if (DWCi_IPTlCheckRepeat(0x40U) != 0) {
            moveFocus_sceneSetApList(1);
            return;
        }
        if (DWCi_IPTlCheckRelease(0x40U) != 0) {
            *(s8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x59) = 0;
            return;
        }
        if (DWCi_IPTlCheckRepeat(0x80U) != 0) {
            moveFocus_sceneSetApList(3);
            return;
        }
        if (DWCi_IPTlCheckRelease(0x80U) != 0) {
            *(s8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x59) = 0;
        }
    }
}

void draw_sceneSetApList(void) {
    s32 rest;

    if (OV016_DATA_21DE5D8->work->unk_38 != NULL) {
        return;
    }
    if (OV016_DATA_21DE5D8->work->unk_55 != 0) {
        OV016_DATA_21DE5D8->work->unk_55--;
    }
    switch (DWCi_SBlGetState()) {
    case 1:
        OV016_DATA_21DE5D8->work->unk_57 = 1;
        DWCi_BTNlDisable();
        break;
    case 2:
        if (OV016_DATA_21DE5D8->work->unk_55 != 0) {
            break;
        }
        DWCi_FOCUSlDelete();
        OV016_DATA_21DE5D8->unk_2 = FX_DivS32(OV016_DATA_21DE5D8->work->unk_40 * DWCi_SBlGet(), OV016_DATA_21DE5D8->work->unk_53);
        drawApInfo();
        OV016_DATA_21DE5D8->work->unk_55 = 4;
        break;
    case 3:
        OV016_DATA_21DE5D8->work->unk_57 = 0;
        DWCi_BTNlEnable();
        OV016_DATA_21DE5D8->unk_2 = FX_DivS32(OV016_DATA_21DE5D8->work->unk_40 * DWCi_SBlGet(), OV016_DATA_21DE5D8->work->unk_53);
        DWCi_SNDlPlay(0x13);
        drawApInfo();
        rest = FX_ModS32(OV016_DATA_21DE5D8->unk_2, 0x1C);
        if (rest == 0) {
            dispFocus_sceneSetApList();
        } else if (rest < 0xE) {
            OV016_DATA_21DE5D8->work->unk_38 = DWCi_TSKlForm(0, (s32)taskUp_sceneSetApList, 0, 0x78);
        } else {
            OV016_DATA_21DE5D8->work->unk_38 = DWCi_TSKlForm(0, (s32)taskDown_sceneSetApList, 0, 0x78);
        }
        break;
    case 4:
        if (OV016_DATA_21DE5D8->unk_2 == 0) {
            if (OV016_DATA_21DE5D8->work->unk_58 == 0) {
                DWCi_SNDlPlay(9);
                OV016_DATA_21DE5D8->work->unk_58 = 1;
            }
        } else {
            DWCi_SNDlPlay(0x13);
            OV016_DATA_21DE5D8->work->unk_38 = DWCi_TSKlForm(0, (s32)taskUp_sceneSetApList, 0, 0x78);
        }
        break;
    case 6:
        if (OV016_DATA_21DE5D8->work->unk_51 <= 4 || OV016_DATA_21DE5D8->unk_2 == OV016_DATA_21DE5D8->work->unk_40) {
            if (OV016_DATA_21DE5D8->work->unk_58 == 0) {
                DWCi_SNDlPlay(9);
                OV016_DATA_21DE5D8->work->unk_58 = 1;
            }
        } else {
            DWCi_SNDlPlay(0x13);
            OV016_DATA_21DE5D8->work->unk_38 = DWCi_TSKlForm(0, (s32)taskDown_sceneSetApList, 0, 0x78);
        }
        break;
    case 5:
    case 7:
        OV016_DATA_21DE5D8->work->unk_58 = 0;
        break;
    }
}

void dispPushButton_sceneSetApList(void);
void button_sceneSetApList(void) {
    Ov016Unk21de5d8Work *work = OV016_DATA_21DE5D8->work;
    s32 v;
    s32 sel;

    if (work->unk_38 != NULL || work->unk_57 != 0) {
        return;
    }
    switch (DWCi_BTNlGet()) {
    case 0:
        v = 7;
        break;
    case 1:
        if (OV016_DATA_21DE5D8->unk_0 == 4) {
            OV016_DATA_21DE5D8->work->unk_54 = 1;
            DWCi_SNDlPlay(6);
            dispPushButton_sceneSetApList();
            goto tail;
        }
        sel = OV016_DATA_21DE5D8->unk_0 + FX_DivS32(OV016_DATA_21DE5D8->unk_2, 0x1c);
        if (*(u8 *)(*(s32 *)OV016_DATA_21DE5D8->work + sel * 0x2a + 0x28) == 2) {
            DWCi_SNDlPlay(9);
            DWCi_SBlDisable();
            DWCi_BTNlDisable();
            DWCi_WINlInitEx(0xe, 1, 1, -1, 0);
            DWCi_ChangeScene((u32)procError_sceneSetApList);
            return;
        }
        OV016_DATA_21DE5D8->work->unk_54 = 1;
        v = 6;
        OV016_DATA_21DE5D8->work->unk_52 = sel;
        break;
    default:
        return;
    }
    DWCi_SNDlPlay(v);
tail:
    DWCi_ChangeScene((u32)endScene_sceneSetApList);
}


void endScene_sceneSetApList(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetApList);
}

void endScene1_sceneSetApList(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (*(u8 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x54) != 0) {
        DWCi_BTNlEnd();
    } else {
        DWCi_BTNlDisable();
    }
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x1d, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetApList);
}

void endScene2_sceneSetApList(void) {
    s32 i;
    u32 *ptr;
    u32 *ptr2;
    int val;
    void (*func)();

    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    i = 0;
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    if (DWCi_BTNlIsEnd() == 0) {
        return;
    }
    DWCi_TSKlDeleteEx(1, *(s32 **)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) + 4)) + 0x3C));
    do {
        ptr = *(u32 **)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) + 4) + i * 4) + 0x10);
        if (ptr != NULL) {
            DWCi_OBJlDelete(ptr);
        }
        ptr2 = *(u32 **)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) + 4) + i * 4) + 0x24);
        if (ptr2 != NULL) {
            DWCi_OBJlDelete(ptr2);
        }
        i += 1;
    } while (i < 5);
    DWCi_FNTlDeleteBg(*(s32 *)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) + 4)) + 0xC));
    DWCi_SBlEnd();
    DWCi_FOCUSlDelete();
    DWCi_COMMENTlDelete();
    DWCi_SCREENlEnd();
    DWCi_ARClRelease(*(u32 *)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) + 4)) + 4));
    DWCi_ARClRelease(*(u32 *)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) + 4)) + 8));
    DWCi_GXlInvisible(1, 1U);
    val = 4;
    DWCi_GXlInvisible(0, 0x1DU);
    *(s32 *)((void *)0x04000010) = 0;
    *(s32 *)((u8 *)((void *)0x04000010) + 8) = 0;
    if (*(u8 *)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) + 4)) + 0x54) == 0) {
        DWCi_SEARCHlEnd();
        DWCi_SETTINGlSetSetting((s32)(*(u8 *)((u8 *)((void *)DWCi_SETTIGNlGet()) + 0xF4)));
        DWCi_SetParam_(2U, 0U);
        func = DWCi_SceneSetSelectWay;
    } else if (*(u8 *)((void *)(&data_ov016_021de5d8)) == 4) {
        DWCi_SEARCHlEnd();
        DWCi_SetParam_(0U, 1U);
        func = DWCi_SceneSetSearch;
    } else {
        DWCi_SETTINGlSetSsid((void *)(*(u8 *)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) + 4)) +
                                             0x52) * 0x2A + *(s32 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) +
                                                                                4))));
        DWCi_SetParam_(0U, 0U);
        if (*(u8 *)((u8 *)(*(u8 *)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) + 4)) + 0x52) * 0x2A +
                           *(s32 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5d8)) + val))) + 0x28) != 0) {
            DWCi_SetParam_(0U, 1U);
            DWCi_SetParam2_(1U, 1U);
            func = DWCi_SceneSetEdit32;
        } else {
            DWCi_SetParam_(0U, 1U);
            DWCi_SetParam2_(0U, 1U);
            func = DWCi_SceneSetTestConfirm;
        }
    }
    DWCi_ChangeScene((u32)func);
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de5d8.work);
}

void drawAp(s32 param_1, s32 param_2);
void drawApInfo(void) {
    s32 n;
    s32 idx;
    s32 i;
    s32 j;
    s32 base;

    base = FX_DivS32(OV016_DATA_21DE5D8->unk_2, 0x1c);
    n = OV016_DATA_21DE5D8->work->unk_51;
    DWCi_FNTlClear(*(NNSG2dCharCanvas **)((u8 *)OV016_DATA_21DE5D8->work + 0xc), 0);
    if (n > 5) {
        n = 5;
    }
    idx = base;
    i = 0;
    if (n > 0) {
        do {
            drawAp(idx, i);
            i++;
            idx++;
        } while (i < n);
    }
    j = 0;
    if (n > 0) {
        do {
            drapApObj(base, j);
            j++;
            base++;
        } while (j < n);
    }
    DWCi_FNTlRenewBg(*(s32 *)((u8 *)OV016_DATA_21DE5D8->work + 0xc));
    setOffset_sceneSetApList();
}

void drawAp(s32 ap, s32 column) {
    u16 buf[17];
    s32 len;
    s32 y;
    s32 n;
    long i;

    len = DWCi_STRlNLen((s32)&OV016_DATA_21DE5D8->work->unk_0[ap], 0x20);
    y = column * 0x1c;
    if (ap >= OV016_DATA_21DE5D8->work->unk_51) {
        return;
    }
    if (len <= 0x10) {
        y += 6;
    }
    MI_CpuFill8(buf, 0, 0x22);
    n = len;
    if (n > 0x10) {
        n = 0x10;
    }
    for (i = 0; i < n; i++) {
        buf[i] = OV016_DATA_21DE5D8->work->unk_0[ap].unk_0[i];
    }
    DWCi_FNTlDrawStringEx(OV016_DATA_21DE5D8->work->unk_c, 10, y, 2, 10, buf, 1);
    if (len > 0x10) {
        MI_CpuFill8(buf, 0, 0x22);
        for (n = 0; n < len - 0x10; n++) {
            buf[n] = OV016_DATA_21DE5D8->work->unk_0[ap].unk_0[n + 0x10];
        }
        DWCi_FNTlDrawStringEx(OV016_DATA_21DE5D8->work->unk_c, 10, y + 0xc, 2, 10, buf, 1);
    }
}



void drapApObj(s32 ap, s32 column) {
    s32 val;
    void *ptr;
    void *ptr2;

    if (ap >= (s32) *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x51)) {
        return;
    }
    val = ap * 0x2A;
    ptr2 = *(void **)((u8 *)((*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4) + (column * 4))) + 0x10);
    *(u16 *)((u8 *)(u8 *)ptr2 + 4) = (u16) ((*(u16 *)((u8 *)(u8 *)ptr2 + 4) & ~0x3FF) | *(u16 *)((u8 *)((*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4) + (*(u8 *)((u8 *)((*(s32 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + val)) + 0x28) * 2))) + 0x42));
    ptr = *(void **)((u8 *)((*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4) + (column * 4))) + 0x24);
    *(u16 *)((u8 *)(u8 *)ptr + 4) = (u16) ((*(u16 *)((u8 *)(u8 *)ptr + 4) & ~0x3FF) | *(u16 *)((u8 *)((*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4) + (*(u16 *)((u8 *)((*(s32 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + val)) + 0x26) * 2))) + 0x48));
}

void setOffset_sceneSetApList(void) {
    s32 *ptr;
    s32 *ptr2;
    int result;
    u64 val;
    s32 i;

    result = 0x36 - FX_ModS32((s32) *(u16 *)((u8 *)((void *)&data_ov016_021de5d8) + 2), 0x1C);
    val = *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x51);
    i = 0;
    if ((s32) val > 5) {
        val = 5;
    }
    if ((s32) val > 0) {
        do {
            ptr2 = *(s32 **)((u8 *)((*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4) + (i * 4))) + 0x10);
            *ptr2 = (*ptr2 & 0xFE00FF00) | (u8) (result - 2) | 0xB30000;
            ptr = *(s32 **)((u8 *)((*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4) + (i * 4))) + 0x24);
            i += 1;
            *ptr = (*ptr & 0xFE00FF00) | (u8) (result + 1) | 0xD20000;
            result += 0x1C;
        } while (i < (s32) val);
    }
    *(s8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x56) = 1;
}

void taskUp_sceneSetApList(u32 task) {
    s32 def;

    DWCi_SBlDisable();
    DWCi_FOCUSlDelete();
    if (data_ov016_021de5d8.unk_2 > 4) {
        data_ov016_021de5d8.unk_2 = data_ov016_021de5d8.unk_2 - 4;
    } else {
        data_ov016_021de5d8.unk_2 = 0;
    }
    def = FX_ModS32(data_ov016_021de5d8.unk_2, 0x1c);
    if (def == 0x18) {
        drawApInfo();
        return;
    }
    if (def > 0x18) {
        data_ov016_021de5d8.unk_2 = data_ov016_021de5d8.unk_2 + (u16)(0x1c - def);
        def = 0;
    }
    setOffset_sceneSetApList();
    if (def == 0) {
        DWCi_SBlSet(FX_DivS32(data_ov016_021de5d8.unk_2 * data_ov016_021de5d8.work->unk_53, data_ov016_021de5d8.work->unk_40));
        DWCi_SBlEnable();
        dispFocus_sceneSetApList();
        data_ov016_021de5d8.work->unk_38 = 0;
        DWCi_TSKlDelete(0, task);
    }
}

void taskDown_sceneSetApList(u32 task) {
    s32 remainder;

    DWCi_SBlDisable();
    DWCi_FOCUSlDelete();
    ((u16 *)&data_ov016_021de5d8)[1] = ((u16 *)&data_ov016_021de5d8)[1] + 4;
    remainder = FX_ModS32((u32)((u16 *)&data_ov016_021de5d8)[1], 0x1c);
    if (remainder >= 4) {
        setOffset_sceneSetApList();
        return;
    }
    ((u16 *)&data_ov016_021de5d8)[1] = ((u16 *)&data_ov016_021de5d8)[1] - (u16)remainder;
    drawApInfo();
    remainder = FX_DivS32((u32)((u16 *)&data_ov016_021de5d8)[1] * (u32)*(u8 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x53),
                                        (u32)*(u16 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x40));
    DWCi_SBlSet(remainder);
    DWCi_SBlEnable();
    dispFocus_sceneSetApList();
    *(u32 *)(((s32 *)&data_ov016_021de5d8)[1] + 0x38) = 0;
    DWCi_TSKlDelete(0, task);
}

void taskVBlank_sceneSetApList(void) {
    s32 result;

    if (*(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x56) == 0) {
        return;
    }
    result = 0x01FF0000 & ((FX_ModS32((s32) *(u16 *)((u8 *)((void *)&data_ov016_021de5d8) + 2), 0x1C) - 0x32) << 0x10);
    *(s32 *)((void *)0x04000010) = result;
    *(s32 *)((u8 *)((void *)0x04000010) + 8) = result;
    *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x56) = 0U;
}

void dispFocus_sceneSetApList(void) {
    s32 val;
        char val2;
        u16 val3;

    val2 = 2;
    val = (u32)data_ov016_021de5d8.unk_0 * 8;
        val3 = *(u16 *)(((void *)(&data_ov016_021da54c)) + val);
    if (data_ov016_021de5d8.unk_0 >= 4) {
        val2 = 3;
    }
        DWCi_FOCUSlDisp(val2, (u32)val3, (u32)(*(u16 *)((void *)((u8 *)data_ov016_021da54c + 4) + val)),
                (u32)(*(u16 *)((void *)((u8 *)data_ov016_021da54c + 2) + val)));
}

void moveFocus_sceneSetApList(s32 dir) {
    u8 move = TRUE;

    switch (data_ov016_021de5d8.unk_0) {
    case 0:
        if (dir == 1) {
            if (data_ov016_021de5d8.unk_2 == 0) {
                data_ov016_021de5d8.unk_0 = 4;
            } else {
                DWCi_SNDlPlay(0x13);
                data_ov016_021de5d8.work->unk_38 = DWCi_TSKlForm(0, (s32)taskUp_sceneSetApList, 0, 0x78);
                return;
            }
        } else if (data_ov016_021de5d8.work->unk_51 > 1) {
            data_ov016_021de5d8.unk_0++;
        } else {
            move = FALSE;
        }
        break;
    case 1:
    case 2:
        if (dir == 1) {
            data_ov016_021de5d8.unk_0--;
        } else if (data_ov016_021de5d8.work->unk_51 > data_ov016_021de5d8.unk_0 + 1) {
            data_ov016_021de5d8.unk_0++;
        } else {
            move = FALSE;
        }
        break;
    case 3:
        if (dir == 1) {
            data_ov016_021de5d8.unk_0--;
        } else {
            scrollDown();
            return;
        }
        break;
    case 4:
        if (dir == 1) {
            move = FALSE;
        } else {
            data_ov016_021de5d8.unk_2 = 0;
            data_ov016_021de5d8.unk_0 = 0;
            drawApInfo();
            DWCi_SBlSet(0);
        }
        break;
    }
    if (!move) {
        if (data_ov016_021de5d8.work->unk_59 == 0) {
            DWCi_SNDlPlay(9);
            data_ov016_021de5d8.work->unk_59 = 1;
        }
    } else {
        DWCi_SNDlPlay(8);
        dispFocus_sceneSetApList();
    }
}


void scrollUp_sceneSetApList(void) {
    if (*(u16 *)((u8 *)((void *)&data_ov016_021de5d8) + 2) == 0) {
        if (*(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x59) != 0) {
            return;
        }
        DWCi_SNDlPlay(9);
        *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x59) = 1U;
        return;
    }
    DWCi_SNDlPlay(0x13);
    *(s32 **)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x38) = DWCi_TSKlForm(0, (s32) taskUp_sceneSetApList, 0, 0x78U);
}

void scrollDown(void) {
    if ((*(u16 *)((u8 *)((void *)&data_ov016_021de5d8) + 2) == *(u16 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x40)) || ((u32) *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x51) <= 4U)) {
        if (*(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x59) != 0) {
            return;
        }
        DWCi_SNDlPlay(9);
        *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x59) = 1U;
        return;
    }
    DWCi_SNDlPlay(0x13);
    *(s32 **)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5d8) + 4)) + 0x38) = DWCi_TSKlForm(0, (s32) taskDown_sceneSetApList, 0, 0x78U);
}

void dispPushButton_sceneSetApList(void) {
    u8 PALETTE[1] = {2};

    DWCi_SCREENlCopyPalette((s32)data_ov016_021de5d8.work->unk_8, PALETTE[0], PALETTE[0]);
}


void procError_sceneSetApList(void) {
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procError1_sceneSetApList);
}

void procError1_sceneSetApList(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_SBlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetApList);
}
