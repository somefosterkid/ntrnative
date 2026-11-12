// libdwcutil's sceneSetList.c (Black's ov016 0x021c82fc-0x021ca5d0): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "nitro/fx/fx_cp.h"

void procInit1_sceneSetList(void);
void procScene_sceneSetList(void);
void input_sceneSetList(void);
void draw_sceneSetList(void);
u32 isPush(void);
BOOL isEnablePush(u32 param_1);
void applyForcus1(s32 param_1);
void pushButtonYesNo(void);
void pushWinButton(s32 focus);
void endScene1_sceneSetList(void);
void endScene2_sceneSetList(void);
void drawColumnChar(u32 param_1, s32 param_2);
void drawColumnObj(u32 param_1, s32 param_2);
void setOffset_sceneSetList(void);
void dispFocus_sceneSetList(void);
BOOL DWC_BACKUPlCheckIp(u8 *param_1, u8 *param_2);
void procError1_sceneSetList(void);
typedef struct SetListPushArea {
    u16 l;
    u16 t;
    u16 w;
    u16 h;
} SetListPushArea;
BOOL func_ov016_021d6b04(const SetListPushArea *a);
void drawWindow_sceneSetList(void);
void applyForcus(void);
u32 inputWinButton(void);
void endScene_sceneSetList(void);
void taskDown_sceneSetList(u32 param_1);
void taskUp_sceneSetList(u32 param_1);
void moveFocus_sceneSetList(s32 dir);
typedef struct SetListRect {
    u16 l;
    u16 t;
    u16 r;
    u16 b;
} SetListRect;
void pushIpAuto(BOOL arg0);
void pushDnsAuto(BOOL arg0);
void dispPushButton_sceneSetList(void);
typedef struct SceneSetListWork {
    u8 unk_00[0x40];
    u8 unk_40;
    u8 unk_41[0x7f];
    u8 unk_c0[4];
    u8 unk_c4[4];
    u8 unk_c8[4];
    u8 unk_cc[0x24];
    u8 unk_f0[5];
    u8 unk_f5;
    u8 unk_f6;
} SceneSetListWork;

u32 isCorrect_sceneSetList(void);
void procError_sceneSetList(void);
s32 calcColumnIdx(s32 param_1);
void dispObj(s32 kind, s32 st, s32 idx);
s32 calcIdxColumn(s32 param_1);
void calcParam(void);
void procInit_sceneSetList(void);
void drawString(u8 *txt, s32 idx);
void drawAddress_sceneSetList(const u8 *param_1, s32 param_2);
void taskVBlank_sceneSetList(void);
void taskPushCount(void);
void initGraphics_sceneSetList(void);
void DWCi_HLlLoadList(void);
void DWCi_SBlInit(u8 kind, u8 max, u16 x, u16 y, int pos);



typedef struct SetListSetting {
    u8 unk_00[0x40];
    u8 unk_40[0x80];
    u8 unk_c0[4];
    u8 unk_c4[4];
    u8 unk_c8[4];
    u8 unk_cc[4];
    u8 unk_d0;
    u8 unk_d1[0x15];
    u8 unk_e6_0 : 2;
    u8 unk_e6_2 : 6;
    u8 unk_e7;
    u8 unk_e8[8];
    u8 unk_f0[4];
    u8 unk_f4;
    u8 unk_f5;
    u8 unk_f6;
} SetListSetting;

typedef struct SetListObjs {
    s32 *unk_0;
    u8 unk_04[4];
    u32 *unk_08;
    u32 *unk_0c;
    u32 *unk_10;
    NNSG2dCharCanvas *unk_14;
    GXOamAttr *unk_18[7];
    GXOamAttr *unk_34;
    s32 *unk_38;
    s32 *unk_3c;
    u8 unk_40;
    u8 unk_41;
    u8 unk_42;
    u8 unk_43;
    u8 unk_44;
    u8 unk_45;
    u8 unk_46;
    u8 unk_47;
} SetListObjs;

typedef struct SetListState {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
    u8 unk_3;
    u16 unk_4;
    u16 unk_6;
    SetListObjs *unk_8;
} SetListState;

extern SetListState data_ov016_021de5ec;

void DWCi_SceneSetEditAddress(void);
u8 DWCi_BACKUPlConvMaskCidr(const u8 *mask);
void DWCi_SceneSetListSave(void);

// The prebuilt's tables (scenesetlist.o's DWARF), in one section, in the order MWCC lays them out: the palette rows
// of the three buttons, the title's label by language, where a row is touched (its field and the "No" and "Yes" of a
// row of DHCP), the focus of each row, how far down the labels' BG is by the top row, the row of each focus, the
// cells of the marks, where the screen behind the rows starts, where the buttons are touched and where the focus frames
// the row's field, the two answers and the three buttons
static const u8 PALETTE_NUM[3] = { 6, 8, 7 };
static const SetListPushArea A_NO = { 0x8f, 0x34, 0x2c, 0x18 };
static const SetListPushArea A_YES = { 0xc0, 0x34, 0x2c, 0x18 };
static const SetListPushArea A_EDIT = { 0xcc, 0x34, 0x1c, 0x18 };
static const u8 CLM2FCS_TBL[9] = { 0, 1, 0xe, 4, 5, 6, 0xe, 9, 0xa };
static const u8 OFS_BACK[9] = { 0, 5, 2, 7, 4, 1, 6, 3, 0 };
static const s8 MSG_IDX[7] = { -1, 0x23, 0x27, -1, 0x23, 0x2f, -1 };
static const u8 FCS2CLM_TBL[11] = { 0, 1, 2, 2, 3, 4, 5, 6, 6, 7, 8 };
static const u8 CELL_IDX[5][3] = {
    { 0, 0x29, 0x2c }, { 0x52, 0x53, 0x30 }, { 0, 0x2a, 0x30 }, { 0x54, 0x55, 0 }, { 0, 0x2b, 0 },
};
static const u16 BACK_POS[9] = { 0, 0x60, 0xe0, 0x140, 0x1c0, 0x240, 0x2a0, 0x320, 0x3a0 };
static const SetListRect R_BUTTON[3] = {
    { 0x84, 0x1b, 0xfc, 0x2c },
    { 0x84, 0xac, 0xfc, 0xbd },
    { 0x04, 0xac, 0x7c, 0xbd },
};
static const SetListRect R_FOCUS[6] = {
    { 0xc8, 0x31, 0xe0, 0x4d }, { 0xbc, 0x31, 0xe0, 0x4d }, { 0x8b, 0x31, 0xaf, 0x4d },
    { 0x82, 0x18, 0xee, 0x2c }, { 0x82, 0xa9, 0xee, 0xbd }, { 0x02, 0xa9, 0x6e, 0xbd },
};

void DWCi_SceneSetList(void) {
    SetListSetting *setting;
    s32 i;
    u32 id;

    setting = (SetListSetting *)DWCi_SETTIGNlGet();
    ((SetListState *)&data_ov016_021de5ec)->unk_8 = DWCi_HEAPlAllocEx(0x48, 4);
    ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_43 = setting->unk_f6;
    ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_42 = 0xc;
    calcParam();
    initGraphics_sceneSetList();
    if (((SetListState *)&data_ov016_021de5ec)->unk_1 == 0) {
        id = DWCi_Language();
        DWCi_COMMENTlDispEx(0x30, MSG_IDX[id], setting->unk_f4 + 1);
    } else {
        DWCi_COMMENTlDispEx(0x45, -1, 0);
    }
    DWCi_CMNlStep(1);
    DWCi_HLlLoadList();
    DWCi_SBlInit(2, 0x55, 0xf1, 0x41, (s32)((SetListState *)&data_ov016_021de5ec)->unk_4 * 0x37 / 0x91);
    ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_14 = DWCi_FNTlFormBgEx(0, 1);
    for (i = 0; i < 7; i++) {
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_18[i] = DWCi_CEINlSetExObj(NULL, 0x29);
    }
    switch (setting->unk_e7) {
    case 1:
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_34 = DWCi_CEINlSetExObj(NULL, 0x50);
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_34->attr2 = (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_34->attr2 & ~0xc00) | 0xc00;
        break;
    case 2:
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_34 = DWCi_CEINlSetExObj(NULL, 0x51);
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_34->attr2 = (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_34->attr2 & ~0xc00) | 0xc00;
        break;
    }
    ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_3c = DWCi_TSKlForm(1, (s32)taskVBlank_sceneSetList, 0, 0x6e);
    ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_0 = DWCi_TSKlForm(0, (s32)taskPushCount, 0, 0x78);
    drawWindow_sceneSetList();
    dispFocus_sceneSetList();
    DWCi_ChangeScene((u32)procInit_sceneSetList);
}

void initGraphics_sceneSetList(void) {
    SetListState *setListState;
    s8 name_a[23] = "char/jb3ListBack.nsc.l";
    s8 name_b[21] = "char/ybBgStep2.ncl.l";
    s8 name_c[22] = "char/ybBgStep21.ncl.l";
    u8 *ptr;

    G2_SetBG2ControlText(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, (GXBGScrBase)GX_BG_SCRBASE_0x7000,
                         (GXBGCharBase)GX_BG_CHARBASE_0x18000);
    DWCi_CMNlLoadResource("char/ybObjKb.ncl.l", (u8 *)GX_LoadOBJPltt);
    DWCi_CMNlLoadResource("char/jbBgStep2.ncg.l", (u8 *)GX_LoadBG3Char);
    DWCi_CMNlLoadResource("char/jbBgStep21.ncg.l", (u8 *)GX_LoadBG2Char);
    DWCi_CMNlLoadResource(name_b, (u8 *)GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/jb3List.nsc.l", (u8 *)GX_LoadBG3Scr);
    ((SetListState *)(&data_ov016_021de5ec))->unk_8->unk_08 = DWCi_ARClReadEx((s8 *)DWCi_Char(name_a),
                                                                                  NULL, 4);
    DWCi_SCREENlInit(((SetListState *)(&data_ov016_021de5ec))->unk_8->unk_08);
    DWCi_SCREENlRenew();
    /* FAKEMATCH(unused-assign): `ptr` is never read; pokebw2's LoadGraphics (no locals) and the same temporaries as
       statements are 24 lines (registers and order: the main BG0CNT address is computed before the sub screen's
       priority writes, the original after them). The prebuilt's line table and DWARF (`u8 *n`) give
       `n = DWCi_Char(name);` on its own line before each DWCi_ARClReadEx(n, NULL, 4): also 24 lines (round 37) */
    (setListState = (SetListState *)(&data_ov016_021de5ec))->unk_8->unk_0c = DWCi_ARClReadEx((s8 *)DWCi_Char(name_b), NULL, 4);
    setListState->unk_8->unk_10 = DWCi_ARClReadEx((s8 *)(ptr = DWCi_Char(name_c)), NULL, 4);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG0Priority(3);
    G2_SetBG1Priority(2);
    G2_SetBG2Priority(3);
    G2_SetBG3Priority(2);
}


void calcParam(void) {
    u32 auStack_10 [1];

    DWCi_SETTIGNlGet();
    DWCi_GetParam2_((u32 *)0x0, auStack_10);
    if (auStack_10[0] == 0) {
        ((u16 *)&data_ov016_021de5ec)[2] = 0;
        data_ov016_021de5ec.unk_0 = 0;
        ((u8 *)&data_ov016_021de5ec)[2] = 0;
    }
}

void procInit_sceneSetList(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x1d, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x1d);
    DWCi_ChangeScene((u32)procInit1_sceneSetList);
}

void procInit1_sceneSetList(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneSetList);
}

void procScene_sceneSetList(void) {
    input_sceneSetList();
    draw_sceneSetList();
}

void input_sceneSetList(void) {
    if ((*(s32 **)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x38) == NULL) && (*(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x45) == 0)) {
        if (inputWinButton() != 0) {
            return;
        }
        if (DWCi_IPTlCheckTrigger(1U) != 0) {
            pushWinButton((u32) *(u8 *)((void *)&data_ov016_021de5ec));
            return;
        }
        if (DWCi_IPTlCheckTrigger(2U) != 0) {
            DWCi_SNDlPlay(7);
            *(s8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x40) = 0xD;
            DWCi_ChangeScene((u32) endScene_sceneSetList);
            return;
        }
        if (DWCi_IPTlCheckRepeat(0x200U) != 0) {
            if (*(u16 *)((u8 *)((void *)&data_ov016_021de5ec) + 4) == 0x91) {
                if (*(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x47) != 0) {
                    return;
                }
                DWCi_SNDlPlay(9);
                *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x47) = 1U;
                return;
            }
            DWCi_SNDlPlay(0x13);
            *(s32 **)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x38) = DWCi_TSKlForm(0, (s32) taskDown_sceneSetList, 0, 0x78U);
            return;
        }
        if (DWCi_IPTlCheckRelease(0x200U) != 0) {
            *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x47) = 0U;
            return;
        }
        if (DWCi_IPTlCheckRepeat(0x100U) != 0) {
            if (*(u16 *)((u8 *)((void *)&data_ov016_021de5ec) + 4) == 0) {
                if (*(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x47) != 0) {
                    return;
                }
                DWCi_SNDlPlay(9);
                *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x47) = 1U;
                return;
            }
            DWCi_SNDlPlay(0x13);
            *(s32 **)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x38) = DWCi_TSKlForm(0, (s32) taskUp_sceneSetList, 0, 0x78U);
            return;
        }
        if (DWCi_IPTlCheckRelease(0x100U) != 0) {
            *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x47) = 0U;
            return;
        }
        if (DWCi_IPTlCheckRepeat(0x40U) != 0) {
            moveFocus_sceneSetList(1);
            return;
        }
        if (DWCi_IPTlCheckRelease(0x40U) != 0) {
            *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x47) = 0U;
            return;
        }
        if (DWCi_IPTlCheckRepeat(0x80U) != 0) {
            moveFocus_sceneSetList(3);
            return;
        }
        if (DWCi_IPTlCheckRelease(0x80U) != 0) {
            *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x47) = 0U;
            return;
        }
        if (DWCi_IPTlCheckRepeat(0x20U) != 0) {
            moveFocus_sceneSetList(0);
            return;
        }
        if (DWCi_IPTlCheckRepeat(0x10U) == 0) {
            return;
        }
        moveFocus_sceneSetList(2);
    }
}

u32 inputWinButton(void) {
    u32 result;

    DWCi_SETTIGNlGet();
    result = isPush();
    if (result == 0xe) {
        return 0;
    }
    if (!isEnablePush(result)) {
        DWCi_SNDlPlay(9);
        return 1;
    }
    applyForcus1(result);
    switch(result) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        goto code_r0x021c8a58;
    case 3:
        goto code_r0x021c8a58;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        goto code_r0x021c8a58;
    case 8:
code_r0x021c8a58:
        pushButtonYesNo();
    }
    pushWinButton(result);
    return 1;
}

void pushWinButton(s32 focus) {
    if (!isEnablePush(focus)) {
        DWCi_SNDlPlay(9);
        return;
    }
    switch (focus) {
    case 2:
    case 3:
        DWCi_SNDlPlay(6);
        pushIpAuto(((focus - 2) ^ 1) != 0);
        return;
    case 7:
    case 8:
        DWCi_SNDlPlay(6);
        pushDnsAuto(((focus - 7) ^ 1) != 0);
        return;
    }
    ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_40 = focus;
    if (focus == 0xb || focus == 0xc) {
        dispPushButton_sceneSetList();
        if (!isCorrect_sceneSetList()) {
            DWCi_SNDlPlay(9);
            DWCi_WINlInitEx(6, 1, 1, -1, 0);
            DWCi_ChangeScene((u32)procError_sceneSetList);
            return;
        }
        if (focus == 0xb) {
            DWCi_SNDlPlay(6);
        } else {
            DWCi_SNDlPlay(0xe);
        }
    } else if (focus == 0xd) {
        dispPushButton_sceneSetList();
        DWCi_SNDlPlay(7);
    } else {
        DWCi_SNDlPlay(6);
        dispObj(0, 1, calcColumnIdx((u32)FCS2CLM_TBL[focus]));
        setOffset_sceneSetList();
    }
    DWCi_SBlDisable();
    DWCi_ChangeScene((u32)endScene_sceneSetList);
}

void pushIpAuto(BOOL yes) {
    u8 flag;
    void *ptr;

    ptr = (void *) DWCi_SETTIGNlGet();
    if (*(u8 *)((u8 *)(u8 *)ptr + 0xF5) == yes) {
        return;
    }
    if (yes != 0) {
        flag = 1;
        if (*(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x43) == 0) {
            flag = 0;
        }
    } else {
        flag = 0;
        *(u8 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x43) = (u8) *(u8 *)((u8 *)(u8 *)ptr + 0xF6);
    }
    DWCi_SETTINGlSetIpAuto(yes);
    DWCi_SETTINGlSetDnsAuto(flag);
    drawWindow_sceneSetList();
}

void pushDnsAuto(BOOL yes) {
    s32 result;

    result = DWCi_SETTIGNlGet();
    if (*(u8 *)(result + 0xf6) == yes) {
        return;
    }
    *(u8 *)(((s32 *)&data_ov016_021de5ec)[2] + 0x43) = yes;
    DWCi_SETTINGlSetDnsAuto(yes);
    drawWindow_sceneSetList();
}

void draw_sceneSetList(void) {
    s32 def;

    if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_38 != NULL) {
        return;
    }
    if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_41 != 0) {
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_41 = ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_41 - 1;
    }
    switch (DWCi_SBlGetState()) {
    case 1:
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_45 = 1;
        break;
    case 2:
        if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_41 != 0) {
            break;
        }
        DWCi_FOCUSlDelete();
        ((SetListState *)&data_ov016_021de5ec)->unk_4 = DWCi_SBlGet() * 0x91 / 0x37;
        drawWindow_sceneSetList();
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_41 = 4;
        break;
    case 3:
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_45 = 0;
        ((SetListState *)&data_ov016_021de5ec)->unk_4 = DWCi_SBlGet() * 0x91 / 0x37;
        DWCi_SNDlPlay(0x13);
        drawWindow_sceneSetList();
        def = FX_ModS32(((SetListState *)&data_ov016_021de5ec)->unk_4, 0x1d);
        if (def == 0) {
            applyForcus();
        } else if (def < 0x10) {
            ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_38 = DWCi_TSKlForm(0, (s32)taskUp_sceneSetList, 0, 0x78);
        } else {
            ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_38 = DWCi_TSKlForm(0, (s32)taskDown_sceneSetList, 0, 0x78);
        }
        break;
    case 4:
        if (((SetListState *)&data_ov016_021de5ec)->unk_4 == 0) {
            if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_46 == 0) {
                DWCi_SNDlPlay(9);
                ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_46 = 1;
            }
        } else {
            DWCi_SNDlPlay(0x13);
            ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_38 = DWCi_TSKlForm(0, (s32)taskUp_sceneSetList, 0, 0x78);
        }
        break;
    case 6:
        if (((SetListState *)&data_ov016_021de5ec)->unk_4 == 0x91) {
            if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_46 == 0) {
                DWCi_SNDlPlay(9);
                ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_46 = 1;
            }
        } else {
            DWCi_SNDlPlay(0x13);
            ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_38 = DWCi_TSKlForm(0, (s32)taskDown_sceneSetList, 0, 0x78);
        }
        break;
    case 5:
    case 7:
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_46 = 0;
        break;
    }
}

void endScene_sceneSetList(void) {
    DWCi_SBlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetList);
}

void endScene1_sceneSetList(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x1d, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetList);
}

extern u32 data_ov016_021de5f4;
void endScene2_sceneSetList(void) {
    s32 i;
    s32 j;
    s32 k;
    SetListSetting *setting;
    u32 *bgofs;
    void (*next)(void);
    u32 val;

    k = 0;
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    j = 0;
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_TSKlDeleteEx(0, ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_0);
    DWCi_TSKlDeleteEx(1, ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_3c);
    for (i = 0; i < 7; i++) {
        DWCi_OBJlDelete((u32 *)((SetListState *)&data_ov016_021de5ec)->unk_8->unk_18[i]);
    }
    if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_34 != NULL) {
        DWCi_OBJlDelete((u32 *)((SetListState *)&data_ov016_021de5ec)->unk_8->unk_34);
    }
    DWCi_SBlEnd();
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_FOCUSlDelete();
    if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_40 != 0xc) {
        DWCi_COMMENTlDelete();
    }
    DWCi_SCREENlEnd();
    DWCi_ARClRelease((u32)((SetListState *)&data_ov016_021de5ec)->unk_8->unk_08);
    for (i = 0; i < 2; i++) {
        DWCi_ARClRelease((u32)(&((SetListState *)&data_ov016_021de5ec)->unk_8->unk_08)[1 + i]);
    }
    DWCi_CMNlLoadResource("char/ybObjMain.ncl.l", (u8 *)GX_LoadOBJPltt);
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x1d);
    bgofs = (u32 *)0x04000010;
    bgofs[0] = 0;
    bgofs[2] = 0;
    reg_G2_BG2CNT = (u16)((reg_G2_BG2CNT & 0x43) | 0xe10);
    val = ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_40;
    switch (val) {
    case 0:
    case 1:
        DWCi_SetParam2_(val, 0);
        DWCi_SetParam_(2, 0);
        next = DWCi_SceneSetEdit32;
        DWCi_ChangeScene((u32)next);
        break;
    case 4:
    case 5:
    case 6:
    case 9:
    case 10:
        j = val - 4;
        if (val >= 9) {
            j -= 2;
        }
        DWCi_SetParam_(2, 0);
        DWCi_SetParam2_(j, 0);
        next = DWCi_SceneSetEditAddress;
        DWCi_ChangeScene((u32)next);
        break;
    case 11:
        setting = (SetListSetting *)DWCi_SETTIGNlGet();
        setting->unk_d0 = DWCi_BACKUPlConvMaskCidr(setting->unk_f0);
        if (setting->unk_f5 != 0) {
            MI_CpuFill8(setting->unk_c0, 0, 4);
            MI_CpuFill8(setting->unk_c4, 0, 4);
            MI_CpuFill8(setting->unk_f0, 0, 4);
            setting->unk_d0 = 0;
        }
        if (setting->unk_f6 != 0) {
            MI_CpuFill8(setting->unk_c8, 0, 8);
        }
        DWCi_SetParam_(2, 0);
        DWCi_SetParam2_(0, 0);
        next = DWCi_SceneSetTest;
        DWCi_ChangeScene((u32)next);
        break;
    case 12:
        DWCi_SetParam_(0, 0);
        next = DWCi_SceneSetListSave;
        DWCi_ChangeScene((u32)next);
        break;
    case 13:
        if (((SetListState *)&data_ov016_021de5ec)->unk_1 == 0) {
            DWCi_SetParam_(2, 1);
            next = DWCi_SceneSetSelectAp;
        } else {
            DWCi_SETTINGlSetSetting(((SetListSetting *)DWCi_SETTIGNlGet())->unk_f4);
            DWCi_SetParam_(0, 1);
            next = DWCi_SceneSetSelectWay;
        }
        DWCi_ChangeScene((u32)next);
        break;
    default:
        break;
    }
    DWCi_HEAPlFree_(&data_ov016_021de5f4);
}


void drawWindow_sceneSetList(void) {
    s32 quotient;
    s32 i;
    s32 j;

    quotient = FX_DivS32(data_ov016_021de5ec.unk_4, 0x1d);
    j = 0;
    DWCi_FNTlClear(data_ov016_021de5ec.unk_8->unk_14, 0);
    i = quotient;
    do {
        drawColumnChar(i, j);
        j = j + 1;
        i = i + 1;
    } while (j < 5);
    j = 0;
    DWCi_SCREENlCopy((void *)(*(s32 *)(((s32 *)&data_ov016_021de5ec)[2] + 8) +
                                                            (u32)BACK_POS[quotient] * 2), 0, 0x1e, 0x13);
    do {
        drawColumnObj(quotient, j);
        j = j + 1;
        quotient = quotient + 1;
    } while (j < 5);
    DWCi_SCREENlRenew();
    DWCi_FNTlRenewBg(*(s32 *)(((s32 *)&data_ov016_021de5ec)[2] + 0x14));
    setOffset_sceneSetList();
}

void drawColumnChar(u32 column, s32 idx) {
    SetListSetting *setting;
    s32 size;
    u8 buf[0x21];

    setting = (SetListSetting *)DWCi_SETTIGNlGet();
    switch (column) {
    case 0:
        drawString(setting->unk_40, idx);
        return;
    case 1:
        switch (setting->unk_e6_0) {
        case 0:
            return;
        case 1:
            size = 10;
            break;
        case 2:
            size = 0x1a;
            break;
        case 3:
            size = 0x20;
            break;
        }
        if (setting->unk_e6_2 == 1) {
            size = size / 2;
        }
        MI_CpuFill8(buf, 0, 0x21);
        memset(buf, 0x2a, size);
        drawString((u8 *)buf, idx);
        return;
    case 2:
        break;
    case 3:
        if (setting->unk_f5 != 0) {
            return;
        }
        drawAddress_sceneSetList(setting->unk_c0, idx);
        return;
    case 4:
        if (setting->unk_f5 != 0) {
            return;
        }
        drawAddress_sceneSetList(setting->unk_f0, idx);
        return;
    case 5:
        if (setting->unk_f5 != 0) {
            return;
        }
        drawAddress_sceneSetList(setting->unk_c4, idx);
        return;
    case 6:
        break;
    case 7:
        if (setting->unk_f6 != 0) {
            return;
        }
        drawAddress_sceneSetList(setting->unk_c8, idx);
        return;
    case 8:
        if (setting->unk_f6 != 0) {
            return;
        }
        drawAddress_sceneSetList(setting->unk_cc, idx);
    }
}


void drawColumnObj(u32 column, s32 idx) {
    SetListSetting *info;
    s32 kind, kind1;
    s32 st, st1;

    info = (SetListSetting *)DWCi_SETTIGNlGet();
    switch (column) {
    case 0:
    case 1:
        kind = 0;
        st = 0;
        if (!isEnablePush(0)) {
            st = 2;
        }
        break;
    case 2:
        st = st1 = 0;
        if (info->unk_f5) {
            kind = 1;
            kind1 = 4;
        } else {
            kind = 2;
            kind1 = 3;
        }
        if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_04[0]) {
            st = 1;
        }
        if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_04[1]) {
            st1 = 1;
        }
        dispObj(kind1, st1, idx);
        break;
    case 3:
    case 4:
    case 5:
        kind = 0;
        if (info->unk_f5) {
            st = 2;
        } else {
            st = 0;
        }
        break;
    case 6:
        st = st1 = 0;
        if (info->unk_f6) {
            kind = 1;
            kind1 = 4;
        } else {
            if (!info->unk_f5) {
                st = 2;
            }
            kind = 2;
            kind1 = 3;
        }
        if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_04[2]) {
            st = 1;
        }
        if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_04[3]) {
            st1 = 1;
        }
        dispObj(kind1, st1, idx);
        break;
    case 7:
    case 8:
        kind = 0;
        if (info->unk_f6) {
            st = 2;
        } else {
            st = 0;
        }
        break;
    default:
        kind = 0;
        st = 2;
        break;
    }
    dispObj(kind, st, idx);
}

BOOL isEnablePush(u32 param_1) {
    SetListSetting *info;
    u8 rs;

    info = (SetListSetting *)DWCi_SETTIGNlGet();
    rs = TRUE;
    switch (param_1) {
    case 7:
        if (!info->unk_f5) {
            rs = FALSE;
        }
        break;
    case 0:
    case 1:
        if (info->unk_e7 == 1 || info->unk_e7 == 2) {
            rs = FALSE;
        }
        break;
    case 4:
    case 5:
    case 6:
        if (info->unk_f5) {
            rs = FALSE;
        }
        break;
    case 9:
    case 10:
        if (info->unk_f6) {
            rs = FALSE;
        }
        break;
    }
    return rs;
}

void dispObj(s32 kind, s32 st, s32 idx) {
    u16 X[5];
    GXOamAttr **ob;

    X[0] = 0xcc;
    X[1] = 0xc0;
    X[2] = 0xc0;
    X[3] = 0x8f;
    X[4] = 0x8f;
    ob = &((SetListState *)&data_ov016_021de5ec)->unk_8->unk_18[idx];
    if (kind == 1 || kind == 2) {
        if (calcIdxColumn(idx) == 2) {
            ob = &((SetListState *)&data_ov016_021de5ec)->unk_8->unk_18[5];
        } else {
            ob = &((SetListState *)&data_ov016_021de5ec)->unk_8->unk_18[6];
        }
    }
    if (CELL_IDX[kind][st] != 0) {
        func_ov016_021d52d4(0, CELL_IDX[kind][st], *ob);
        G2_SetOBJPosition(*ob, X[kind], 0);
        G2_SetOBJPriority(*ob, 3);
    } else {
        G2_SetOBJPosition(*ob, 0x100, 0);
    }
}

void drawString(u8 *txt, s32 idx) {
    s32 len;
    s32 y;
    s32 i;
    s32 n;
    u16 buf[17];

    MI_CpuFill8(buf, 0, 0x22);
    len = DWCi_STRlNLen((s32)txt, 0x20);
    n = len;
    if (n > 0x10) {
        n = 0x10;
    }
    for (i = 0; i < n; i++) {
        if (txt[i] == 0x20) {
            buf[i] = 0xe01d;
        } else {
            buf[i] = txt[i];
        }
    }
    y = idx * 0x1d + 2;
    if (len <= 0x10) {
        y += 5;
    }
    DWCi_FNTlDrawStringEx(((SetListState *)&data_ov016_021de5ec)->unk_8->unk_14, 0x48, y, 2, 8, buf, 1);
    if (len > 0x10) {
        n = 0;
        MI_CpuFill8(buf, 0, 0x22);
        len -= 0x10;
        for (; n < len; n++) {
            if (txt[n + 0x10] == 0x20) {
                buf[n] = 0xe01d;
            } else {
                buf[n] = txt[n + 0x10];
            }
        }
        DWCi_FNTlDrawStringEx(((SetListState *)&data_ov016_021de5ec)->unk_8->unk_14, 0x48, y + 0xc, 2, 8, buf, 1);
    }
}

void drawAddress_sceneSetList(const u8 *addr, s32 idx) {
    u16 buf[16];
    char str[16];

    OS_SNPrintf(str, 16, "%3d.%3d.%3d.%3d", addr[0], addr[1], addr[2], addr[3]);
    func_ov016_021d722c(buf, str, 16);
    DWCi_FNTlDrawStringEx(((SetListState *)&data_ov016_021de5ec)->unk_8->unk_14, 0x5f, idx * 0x1d + 8, 2, 7, buf, 1);
}

void setOffset_sceneSetList(void) {
    s32 cell;
    s32 dif;
    s32 cy;
    s32 x;
    s32 y;
    s32 i;

    cell = FX_DivS32(data_ov016_021de5ec.unk_4, 0x1d);
    dif = 0x34 - FX_ModS32(data_ov016_021de5ec.unk_4, 0x1d);
    if (data_ov016_021de5ec.unk_8->unk_34 != NULL) {
        if (cell == 0) {
            x = 0x26;
        } else {
            x = 0x100;
        }
        G2_SetOBJPosition(data_ov016_021de5ec.unk_8->unk_34, x, dif);
    }
    cy = dif;
    for (i = 0; i < 5; i++) {
        G2_GetOBJPosition(data_ov016_021de5ec.unk_8->unk_18[i], (u32 *)&x, (u32 *)&y);
        G2_SetOBJPosition(data_ov016_021de5ec.unk_8->unk_18[i], x, cy);
        cy += 0x1d;
    }
    if (cell <= 2) {
        G2_GetOBJPosition(data_ov016_021de5ec.unk_8->unk_18[5], (u32 *)&x, (u32 *)&y);
        G2_SetOBJPosition(data_ov016_021de5ec.unk_8->unk_18[5], x, (2 - cell) * 0x1d + dif);
    } else {
        G2_SetOBJPosition(data_ov016_021de5ec.unk_8->unk_18[5], 0x100, 0);
    }
    if (cell >= 2 && cell <= 6) {
        G2_GetOBJPosition(data_ov016_021de5ec.unk_8->unk_18[6], (u32 *)&x, (u32 *)&y);
        G2_SetOBJPosition(data_ov016_021de5ec.unk_8->unk_18[6], x, (6 - cell) * 0x1d + dif);
    } else {
        G2_SetOBJPosition(data_ov016_021de5ec.unk_8->unk_18[6], 0x100, 0);
    }
    data_ov016_021de5ec.unk_8->unk_44 = 1;
}

u32 isPush(void) {
    SetListPushArea a;
    s32 cell;
    s32 i;

    if (!DWCi_TPlCheckTrigger((u16 *)&data_ov016_021daa7c)) {
        return 0xe;
    }
    cell = FX_DivS32(((SetListState *)&data_ov016_021de5ec)->unk_4, 0x1d);
    a = A_EDIT;
    for (i = 0; i < 4; i++) {
        if (cell != 2 && cell != 6 && func_ov016_021d6b04(&a)) {
            return CLM2FCS_TBL[cell];
        }
        cell++;
        a.t += 0x1d;
    }
    cell = FX_DivS32(((SetListState *)&data_ov016_021de5ec)->unk_4, 0x1d);
    for (i = 0; i < 4; i++) {
        if (cell == 2) {
            a = A_YES;
            a.t += i * 0x1d;
            if (func_ov016_021d6b04(&a)) {
                return 2;
            }
            a = A_NO;
            a.t += i * 0x1d;
            if (func_ov016_021d6b04(&a)) {
                return 3;
            }
            break;
        }
        cell++;
    }
    cell = FX_DivS32(((SetListState *)&data_ov016_021de5ec)->unk_4, 0x1d);
    for (i = 0; i < 4; i++) {
        if (cell == 6) {
            a = A_YES;
            a.t += i * 0x1d;
            if (func_ov016_021d6b04(&a)) {
                return 7;
            }
            a = A_NO;
            a.t += i * 0x1d;
            if (func_ov016_021d6b04(&a)) {
                return 8;
            }
            break;
        }
        cell++;
    }
    for (i = 0; i < 3; i++) {
        if (DWCi_TPlCheckTrigger((u16 *)&R_BUTTON[i])) {
            return i + 0xb;
        }
    }
    return 0xe;
}



s32 calcColumnIdx(s32 clm) {
    s32 quotient;
    s32 i;

    quotient = FX_DivS32((u32)((u16 *)&data_ov016_021de5ec)[2], 0x1d);
    i = 0;
    do {
        if (quotient == clm) {
            return i;
        }
        i = i + 1;
        quotient = quotient + 1;
    } while (i < 4);
    return -1;
}

s32 calcIdxColumn(s32 idx) {
    s32 quotient;

    quotient = FX_DivS32((u32)((u16 *)&data_ov016_021de5ec)[2], 0x1d);
    return quotient + idx;
}

void taskUp_sceneSetList(u32 task) {
    s32 def;

    DWCi_SBlDisable();
    DWCi_FOCUSlDelete();
    if (((SetListState *)&data_ov016_021de5ec)->unk_4 > 6) {
        ((SetListState *)&data_ov016_021de5ec)->unk_4 = ((SetListState *)&data_ov016_021de5ec)->unk_4 - 6;
    } else {
        ((SetListState *)&data_ov016_021de5ec)->unk_4 = 0;
    }
    def = FX_ModS32(((SetListState *)&data_ov016_021de5ec)->unk_4, 0x1d);
    if (def == 0x17) {
        drawWindow_sceneSetList();
        return;
    }
    if (def > 0x17) {
        ((SetListState *)&data_ov016_021de5ec)->unk_4 += 0x1d - def;
        def = 0;
    }
    setOffset_sceneSetList();
    if (def == 0) {
        DWCi_SBlSet(((SetListState *)&data_ov016_021de5ec)->unk_4 * 0x37 / 0x91);
        DWCi_SBlEnable();
        applyForcus();
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_38 = NULL;
        DWCi_TSKlDelete(0, task);
    }
}

void taskDown_sceneSetList(u32 task) {
    s32 remainder;

    DWCi_SBlDisable();
    DWCi_FOCUSlDelete();
    ((u16 *)&data_ov016_021de5ec)[2] = ((u16 *)&data_ov016_021de5ec)[2] + 6;
    remainder = FX_ModS32((u32)((u16 *)&data_ov016_021de5ec)[2], 0x1d);
    if (remainder >= 6) {
        setOffset_sceneSetList();
        return;
    }
    ((u16 *)&data_ov016_021de5ec)[2] = ((u16 *)&data_ov016_021de5ec)[2] - (u16)remainder;
    drawWindow_sceneSetList();
        DWCi_SBlSet((remainder = (u32)((u16 *)(&data_ov016_021de5ec))[2]) * 0x37 / 0x91);
    DWCi_SBlEnable();
    applyForcus();
    *(u32 *)(((s32 *)&data_ov016_021de5ec)[2] + 0x38) = 0;
    DWCi_TSKlDelete(0, task);
}

void taskVBlank_sceneSetList(void) {
    s32 val;
    s32 result;
    s32 quotient;
    u8 *ptr;

    if (*(u8 *)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5ec)) + 8)) + 0x44) == 0) {
        return;
    }
    quotient = FX_DivS32((s32)(*(u16 *)((u8 *)((void *)(&data_ov016_021de5ec)) + 4)), 0x1D);
    result = FX_ModS32((s32)(*(u16 *)((u8 *)((void *)(&data_ov016_021de5ec)) + 4)), 0x1D) - 0x33;
    ptr = (u8 *)OFS_BACK + quotient;
    val = *ptr;
    *(s32 *)((void *)0x04000010) = (s32)(0x01FF0000 & (result << 0x10));
    *(s32 *)((u8 *)((void *)0x04000010) + 8) = (s32)(0x01FF0000 & ((result + val) << 0x10));
    *(u8 *)((u8 *)(*(void **)((u8 *)((void *)(&data_ov016_021de5ec)) + 8)) + 0x44) = 0U;
}

void dispFocus_sceneSetList(void) {
    u8 TYEP[14] = {0, 0, 1, 2, 0, 0, 0, 1, 2, 0, 0, 3, 4, 5};
    SetListRect r;
    s32 type;

    type = TYEP[((SetListState *)&data_ov016_021de5ec)->unk_0];
    if (type >= 3) {
        DWCi_FOCUSlDisp(3, R_FOCUS[type].l, R_FOCUS[type].r, R_FOCUS[type].t);
    } else {
        r = R_FOCUS[type];
        r.t += ((SetListState *)&data_ov016_021de5ec)->unk_2 * 0x1d;
        DWCi_FOCUSlDisp(1, r.l, r.r, r.t);
    }
}

void applyForcus(void) {
    SetListSetting *info;
    s32 c;

    if (((SetListState *)&data_ov016_021de5ec)->unk_0 == 0xb || ((SetListState *)&data_ov016_021de5ec)->unk_0 == 0xc
            || ((SetListState *)&data_ov016_021de5ec)->unk_0 == 0xd) {
        dispFocus_sceneSetList();
        return;
    }
    info = (SetListSetting *)DWCi_SETTIGNlGet();
    c = ((SetListState *)&data_ov016_021de5ec)->unk_2 + FX_DivS32(((SetListState *)&data_ov016_021de5ec)->unk_4, 0x1d);
    switch (c) {
    case 2:
        if (info->unk_f5) {
            ((SetListState *)&data_ov016_021de5ec)->unk_0 = 2;
        } else {
            ((SetListState *)&data_ov016_021de5ec)->unk_0 = 3;
        }
        break;
    case 6:
        if (info->unk_f6) {
            ((SetListState *)&data_ov016_021de5ec)->unk_0 = 7;
        } else {
            ((SetListState *)&data_ov016_021de5ec)->unk_0 = 8;
        }
        break;
    default:
        ((SetListState *)&data_ov016_021de5ec)->unk_0 = CLM2FCS_TBL[c];
        break;
    }
    dispFocus_sceneSetList();
}

void applyForcus1(s32 focus) {
    data_ov016_021de5ec.unk_0 = (u8)focus;
    ((u8 *)&data_ov016_021de5ec)[2] = (u8)calcColumnIdx((u32)FCS2CLM_TBL[focus]);
    dispFocus_sceneSetList();
}

void moveFocus_sceneSetList(s32 dir) {
    SetListSetting *setting;
    s32 move;
    s32 cur;

    setting = (SetListSetting *)DWCi_SETTIGNlGet();
    move = 0;
    cur = ((SetListState *)&data_ov016_021de5ec)->unk_0;
    if (cur == 8 && setting->unk_f5 == 0 && (dir == 0 || dir == 2)) {
        return;
    }
    switch (cur) {
    case 0:
        if (dir == 1) {
            ((SetListState *)&data_ov016_021de5ec)->unk_0 = 0xb;
        } else {
            if (dir == 3) {
                ((SetListState *)&data_ov016_021de5ec)->unk_2 = ((SetListState *)&data_ov016_021de5ec)->unk_2 + 1;
            } else {
                move = 2;
            }
        }
        break;
    case 10:
        if (dir == 1) {
            ((SetListState *)&data_ov016_021de5ec)->unk_2 = ((SetListState *)&data_ov016_021de5ec)->unk_2 - 1;
        } else {
            if (dir == 3) {
                ((SetListState *)&data_ov016_021de5ec)->unk_0 = ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_42;
            } else {
                move = 2;
            }
        }
        break;
    case 11:
        if (dir == 1) {
            if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_47 != 0) {
                return;
            }
            DWCi_SNDlPlay(9);
            ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_47 = 1;
            return;
        }
        if (dir == 3) {
            ((SetListState *)&data_ov016_021de5ec)->unk_0 = 0;
            ((SetListState *)&data_ov016_021de5ec)->unk_2 = 0;
            ((SetListState *)&data_ov016_021de5ec)->unk_4 = 0;
            drawWindow_sceneSetList();
            DWCi_SBlSet(0);
        } else {
            move = 2;
        }
        break;
    case 12:
    case 13:
        ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_42 = cur;
        if (dir == 1) {
            ((SetListState *)&data_ov016_021de5ec)->unk_0 = 10;
            ((SetListState *)&data_ov016_021de5ec)->unk_2 = 3;
            ((SetListState *)&data_ov016_021de5ec)->unk_4 = 0x91;
            drawWindow_sceneSetList();
            DWCi_SBlSet(0x37);
        } else if (dir == 3) {
            if (((SetListState *)&data_ov016_021de5ec)->unk_8->unk_47 != 0) {
                return;
            }
            DWCi_SNDlPlay(9);
            ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_47 = 1;
            return;
        } else if (cur == 12) {
            ((SetListState *)&data_ov016_021de5ec)->unk_0 = 13;
        } else {
            ((SetListState *)&data_ov016_021de5ec)->unk_0 = 12;
        }
        break;
    default:
        if (dir == 1) {
            if (((SetListState *)&data_ov016_021de5ec)->unk_2 != 0) {
                ((SetListState *)&data_ov016_021de5ec)->unk_2 = ((SetListState *)&data_ov016_021de5ec)->unk_2 - 1;
            } else {
                DWCi_SNDlPlay(0x13);
                ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_38 = DWCi_TSKlForm(0, (s32)taskUp_sceneSetList, 0, 0x78);
                return;
            }
        } else if (dir == 3) {
            if (((SetListState *)&data_ov016_021de5ec)->unk_2 < 3) {
                ((SetListState *)&data_ov016_021de5ec)->unk_2 = ((SetListState *)&data_ov016_021de5ec)->unk_2 + 1;
            } else {
                DWCi_SNDlPlay(0x13);
                ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_38 = DWCi_TSKlForm(0, (s32)taskDown_sceneSetList, 0, 0x78);
                return;
            }
        } else {
            move = 2;
            if (cur == 2) {
                ((SetListState *)&data_ov016_021de5ec)->unk_0 = 3;
            } else if (cur == 3) {
                ((SetListState *)&data_ov016_021de5ec)->unk_0 = 2;
            } else if (cur == 7) {
                ((SetListState *)&data_ov016_021de5ec)->unk_0 = 8;
            } else if (cur == 8) {
                ((SetListState *)&data_ov016_021de5ec)->unk_0 = 7;
            } else {
                break;
            }
            DWCi_SNDlPlay(8);
            dispFocus_sceneSetList();
        }
        break;
    }
    if (move == 2) {
        return;
    }
    DWCi_SNDlPlay(8);
    if (move == 0) {
        applyForcus();
    }
}


void dispPushButton_sceneSetList(void) {
    u8 val;

    val = PALETTE_NUM[*(u8 *)((void *)&data_ov016_021de5ec) - 0xB];
    DWCi_SCREENlCopyPalette(*(s32 *)((u8 *)(*(void **)((u8 *)((void *)&data_ov016_021de5ec) + 8)) + 0x10), (s32) val, (s32) val);
}

void taskPushCount(void) {
    u8 val;
    s32 i;

    i = 0;
    do {
        val = *(u8 *)(((s32 *)&data_ov016_021de5ec)[2] + i + 4);
        if ((val != 0) &&
              (*(u8 *)(((s32 *)&data_ov016_021de5ec)[2] + i + 4) = val + -1,
              *(u8 *)(((s32 *)&data_ov016_021de5ec)[2] + i + 4) == 0)) {
            drawWindow_sceneSetList();
        }
        i = i + 1;
    } while (i < 4);
}

void pushButtonYesNo(void) {
    u8 FCS[4] = {2, 3, 7, 8};
    s32 i;

    for (i = 0; i < 4; i++) {
        if (((SetListState *)&data_ov016_021de5ec)->unk_0 == FCS[i]) {
            ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_04[i] = 0x14;
            if (i & 1) {
                ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_04[i - 1] = 0;
                return;
            } else {
                ((SetListState *)&data_ov016_021de5ec)->unk_8->unk_04[i + 1] = 0;
                return;
            }
        }
    }
}

u32 isCorrect_sceneSetList(void) {
    SceneSetListWork *result;
    BOOL BVar2;

    result = (SceneSetListWork *)DWCi_SETTIGNlGet();
    if (result->unk_40 == 0) {
        return 0;
    }
    if (((result->unk_f6 == 0) &&
            (BVar2 = DWC_BACKUPlCheckAddress((const u8 *)result->unk_c8), BVar2 == 0)) &&
          (BVar2 = DWC_BACKUPlCheckAddress((const u8 *)result->unk_cc), BVar2 == 0)) {
        return 0;
    }
    if (result->unk_f5 == 0) {
        BVar2 = DWC_BACKUPlCheckAddress((const u8 *)result->unk_c0);
        if (BVar2 == 0) {
            return 0;
        }
        BVar2 = DWC_BACKUPlCheckAddress((const u8 *)result->unk_c4);
        if (BVar2 == 0) {
            return 0;
        }
        BVar2 = DWC_BACKUPlCheckIp((u8 *)result->unk_c0, (u8 *)result->unk_f0);
        if (BVar2 == 0) {
            return 0;
        }
    }
    return 1;
}

void procError_sceneSetList(void) {
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_SCREENlSetPalette(*(u32 *)(((s32 *)&data_ov016_021de5ec)[2] + 0xc));
    DWCi_ChangeScene((u32)procError1_sceneSetList);
}

void procError1_sceneSetList(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneSetList);
}

void DWCi_SetListReturn(u8 scene) {
    ((u8 *)&data_ov016_021de5ec)[1] = scene;
}
