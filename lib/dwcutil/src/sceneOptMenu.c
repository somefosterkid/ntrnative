// libdwcutil's sceneOptMenu.c (Black's ov016 0x021c2a3c-0x021c315c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "nitro/fx/fx_cp.h"

void procInit1_sceneOptMenu(void);
void procInit2_sceneOptMenu(void);
void procScene_sceneOptMenu(void);
void input_sceneOptMenu(void);
void draw_sceneOptMenu(void);
void button_sceneOptMenu(void);
void endScene1_sceneOptMenu(void);
// The prebuilt's statics (their names in the comments); PALETTE_NUM is defined before dispPush, its one user, which
// gives Black's layout of MWCC's size sort (R_TOUCH before R_FOCUS)
static u8 data_ov016_021de5a8;  // FocusPos
static u8 data_ov016_021de5a9;  // Choose
static void *data_ov016_021de5ac;  // Palette
static GXOamAttr *data_ov016_021de5b0;  // Icon

typedef struct tagCOORD {
    u16 x;
    u16 y;
} COORD;

typedef struct tagRECT {
    u16 l;
    u16 t;
    u16 r;
    u16 b;
} tagRECT;

static const COORD data_ov016_021da4b4 = { 0xe0, 0x84 };  // C_ICON
static const tagRECT data_ov016_021da4b8[3] = {  // R_TOUCH
    { 0x08, 0x24, 0xf8, 0x44 }, { 0x08, 0x50, 0xf8, 0x70 }, { 0x08, 0x7c, 0xf8, 0x9c },
};
static const tagRECT data_ov016_021da4d0[3] = {  // R_FOCUS
    { 0x06, 0x22, 0xea, 0x36 }, { 0x06, 0x4e, 0xea, 0x62 }, { 0x06, 0x7a, 0xea, 0x8e },
};
void endScene2_sceneOptMenu(void);
void moveFocus_sceneOptMenu(s32 param_1);
void initGraphics_sceneOptMenu(void);
void procInit_sceneOptMenu(void);
void dispPush_sceneOptMenu(void);
void endScene_sceneOptMenu(void);

void DWCi_SceneOptMenu(void) {
    data_ov016_021de5a9 = 0;
    initGraphics_sceneOptMenu();
    DWCi_HLlLoad(0x10);
    DWCi_STEPlChangeScr(3);
    DWCi_COMMENTlDispEx(0x39U, -1, 0);
    data_ov016_021de5b0 = DWCi_CEINlSetExObj(NULL, 0x5B);
    G2_SetOBJPosition(data_ov016_021de5b0, data_ov016_021da4b4.x, data_ov016_021da4b4.y);
    G2_SetOBJPriority(data_ov016_021de5b0, 3);
    DWCi_FOCUSlDisp4(data_ov016_021da4d0[data_ov016_021de5a8].l, data_ov016_021da4d0[data_ov016_021de5a8].r,
                     data_ov016_021da4d0[data_ov016_021de5a8].t, data_ov016_021da4d0[data_ov016_021de5a8].b);
    DWCi_ChangeScene((u32)procInit_sceneOptMenu);
}

void initGraphics_sceneOptMenu(void) {
    u8 PLLT0[22] = "char/ybBgOption.ncl.l";
    u8 PLLT1[23] = "char/ybBgOption1.ncl.l";
    u8 *src;
    u8 *dst;
    u8 *n;
    DWCWiFiInfo info;
    s32 i;

    DWCi_CMNlLoadResource("char/jbBgOption.ncg.l", (u8 *)GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/jb5OptMenu.nsc.l", (u8 *)GX_LoadBG2Scr);
    n = DWCi_Char((s8 *)PLLT1);
    data_ov016_021de5ac = DWCi_ARClReadEx((s8 *)n, NULL, 4);
    n = DWCi_Char((s8 *)PLLT0);
    n = (u8 *)DWCi_ARClReadEx((s8 *)n, NULL, 4);
    DWCi_BM_GetWiFiInfo(&info);
    if (info.attestedUserId == 0) {
        src = n + 0xC0;
        dst = n + 0x40;
        for (i = 0; i < 2; i++) {
            MI_CpuCopy8(src, dst, 0x20);
            src += 0x20;
            dst += 0x20;
        }
    }
    DC_FlushRange(n, 0x200);
    GX_LoadBGPltt(n, 0, 0x200);
    DWCi_ARClRelease((u32)n);
    G2S_SetBG0Priority(3);
    G2S_SetBG1Priority(3);
    G2_SetBG1Priority(3);
    G2_SetBG2Priority(3);
}


void procInit_sceneOptMenu(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x14, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x14);
    DWCi_ChangeScene((u32)procInit1_sceneOptMenu);
}

void procInit1_sceneOptMenu(void) {
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
    DWCi_ChangeScene((u32)procInit2_sceneOptMenu);
}

void procInit2_sceneOptMenu(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneOptMenu);
}

void procScene_sceneOptMenu(void) {
    input_sceneOptMenu();
    draw_sceneOptMenu();
    button_sceneOptMenu();
}

void input_sceneOptMenu(void) {
    BOOL BVar2;
    u32 i;

    i = 0;
    do {
        BVar2 = DWCi_TPlCheckTrigger((u16 *)&data_ov016_021da4b8[i]);
        if (BVar2 != 0) {
            func_ov016_021bcb28(1);
            data_ov016_021de5a8 = i;
            DWCi_FOCUSlDisp4(data_ov016_021da4d0[data_ov016_021de5a8].l, data_ov016_021da4d0[data_ov016_021de5a8].r,
                             data_ov016_021da4d0[data_ov016_021de5a8].t, data_ov016_021da4d0[data_ov016_021de5a8].b);
            return;
        }
        i = i + 1;
    } while (i < 3);
    BVar2 = DWCi_IPTlCheckTrigger(1);
    if (BVar2 != 0) {
        func_ov016_021bcb28(1);
        return;
    }
    BVar2 = DWCi_IPTlCheckTrigger(2);
    if (BVar2 != 0) {
        func_ov016_021bcb28(0);
        return;
    }
    BVar2 = DWCi_IPTlCheckRepeat(0x40);
    if (BVar2 != 0) {
        moveFocus_sceneOptMenu(1);
        return;
    }
    BVar2 = DWCi_IPTlCheckRepeat(0x80);
    if (BVar2 == 0) {
        return;
    }
    moveFocus_sceneOptMenu(3);
}

void draw_sceneOptMenu(void) {
}

void button_sceneOptMenu(void) {
    DWCWiFiInfo info;

    switch (DWCi_BTNlGet()) {
    case 0:
        DWCi_SNDlPlay(7);
        break;
    case 1:
        DWCi_BM_GetWiFiInfo(&info);
        if (data_ov016_021de5a8 != 0 && info.attestedUserId == 0) {
            DWCi_SNDlPlay(9);
            DWCi_BTNlSetForce(-1);
            return;
        }
        DWCi_SNDlPlay(6);
        dispPush_sceneOptMenu();
        data_ov016_021de5a9 = 1;
        break;
    default:
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneOptMenu);
}

void endScene_sceneOptMenu(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneOptMenu);
}

void endScene1_sceneOptMenu(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (data_ov016_021de5a9 != 0) {
        DWCi_BTNlEnd();
    }
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x14, 8);
    DWCi_ChangeScene((u32)endScene2_sceneOptMenu);
}

void endScene2_sceneOptMenu(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    if (DWCi_BTNlIsEnd() == 0) {
        return;
    }
    DWCi_OBJlDelete((u32 *)data_ov016_021de5b0);
    DWCi_FOCUSlDelete();
    DWCi_COMMENTlDelete();
    DWCi_ARClRelease((u32)data_ov016_021de5ac);
    DWCi_GXlInvisible(1, 1U);
    DWCi_GXlInvisible(0, 0x14U);
    if (data_ov016_021de5a9 == 0) {
        DWCi_SetParam_(0U, 0U);
        DWCi_ChangeScene((u32) DWCi_SceneMenu);
        return;
    }
    switch (data_ov016_021de5a8) {
    case 0:
        DWCi_SetParam_(0U, 1U);
        DWCi_ChangeScene((u32) DWCi_SceneOptInfo);
        return;
    case 1:
        DWCi_SetParam_(0U, 1U);
        DWCi_ChangeScene((u32) DWCi_SceneOptErase);
        return;
    case 2:
        DWCi_SetParam_(0U, 1U);
        DWCi_ChangeScene((u32) DWCi_SceneOptMovConfirm1);
        return;
    }
}

void moveFocus_sceneOptMenu(s32 dir) {
    if (dir == 1) {
        data_ov016_021de5a8 = FX_ModS32(data_ov016_021de5a8 + 2, 3);
    } else {
        data_ov016_021de5a8 = FX_ModS32(data_ov016_021de5a8 + 1, 3);
    }
    DWCi_SNDlPlay(8);
    DWCi_FOCUSlDisp4(data_ov016_021da4d0[data_ov016_021de5a8].l, data_ov016_021da4d0[data_ov016_021de5a8].r,
                     data_ov016_021da4d0[data_ov016_021de5a8].t, data_ov016_021da4d0[data_ov016_021de5a8].b);
}

static const u8 data_ov016_021da4b0[3] = { 1, 2, 3 };  // PALETTE_NUM
void dispPush_sceneOptMenu(void) {
    DWCi_SCREENlCopyPalette((s32)data_ov016_021de5ac, data_ov016_021da4b0[data_ov016_021de5a8],
                            data_ov016_021da4b0[data_ov016_021de5a8]);
}
