// libdwcutil's sceneSetTestConfirm.c (Black's ov016 0x021cd80c-0x021cda88): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetTestConfirm(void);
void procInit_sceneSetTestConfirm(void);
void procInit1_sceneSetTestConfirm(void);
void input_sceneSetTestConfirm(void);
void draw_sceneSetTestConfirm(void);
void endScene_sceneSetTestConfirm(void);
void endScene1_sceneSetTestConfirm(void);
extern u32 data_ov016_021da8ac;
void procScene_sceneSetTestConfirm(void);

void DWCi_SceneSetTestConfirm(void) {
    u32 auStack_10 [1];

    DWCi_SEARCHlEnd();
    DWCi_GetParam2_((u32 *)0x0, auStack_10);
    initGraphics_sceneSetTestConfirm();
    DWCi_COMMENTlDispEx(0x31, -1, 0);
    if (auStack_10[0] != 2) {
        DWCi_HLlLoadSet();
    }
    if (auStack_10[0] == 1) {
        DWCi_SETTINGlSet();
    }
    DWCi_ChangeScene((u32)procInit_sceneSetTestConfirm);
}

void initGraphics_sceneSetTestConfirm(void) {
    DWCi_CMNlLoadResource("char/xb4None.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetTestConfirm(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x14, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x14);
    DWCi_ChangeScene((u32)procInit1_sceneSetTestConfirm);
}

void procInit1_sceneSetTestConfirm(void) {
    s32 MSG[3] = { 43, 43, 73 };
    s32 prm;

    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_GetParam2_(0, &prm);
    DWCi_WINlInitEx(MSG[prm], 1, 1, -1, 0);
    DWCi_ChangeScene((u32)procScene_sceneSetTestConfirm);
}

void procScene_sceneSetTestConfirm(void) {
    input_sceneSetTestConfirm();
    draw_sceneSetTestConfirm();
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_SNDlPlay(6);
    DWCi_ChangeScene((u32)endScene_sceneSetTestConfirm);
}

void input_sceneSetTestConfirm(void) {
}

void draw_sceneSetTestConfirm(void) {
}

void endScene_sceneSetTestConfirm(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    func_ov016_021d55a4(3, 0, 0x14, 8);
    DWCi_ChangeScene((u32)endScene1_sceneSetTestConfirm);
}

void endScene1_sceneSetTestConfirm(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_GXlInvisible(0, 0x14);
    DWCi_SetParam_(0, 1);
    DWCi_ChangeScene((u32)DWCi_SceneSetTest);
}
