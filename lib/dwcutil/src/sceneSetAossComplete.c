// libdwcutil's sceneSetAossComplete.c (Black's ov016 0x021c4ec8-0x021c50d4): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetAossComplete(void);
void procInit_sceneSetAossComplete(void);
static u8 data_ov016_021de5d4;
void procInit1_sceneSetAossComplete(void);
void procScene_sceneSetAossComplete(void);
void wait_sceneSetAossComplete(void);
void draw_sceneSetAossComplete(void);
void endScene1_sceneSetAossComplete(void);
void endScene_sceneSetAossComplete(void);

void DWCi_SceneSetAossComplete(void) {
    data_ov016_021de5d4 = 0;
    initGraphics_sceneSetAossComplete();
    DWCi_HLlLoadSet();
    DWCi_CMNlMsgDraw(0x23);
    DWCi_SNDlPlay(0x10);
    DWCi_ChangeScene((u32)procInit_sceneSetAossComplete);
}

void initGraphics_sceneSetAossComplete(void) {
    DWCi_CMNlLoadResource("char/xb4Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetAossComplete(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetAossComplete);
}

void procInit1_sceneSetAossComplete(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneSetAossComplete);
}

void procScene_sceneSetAossComplete(void) {
    wait_sceneSetAossComplete();
    draw_sceneSetAossComplete();
}

void draw_sceneSetAossComplete(void) {
}

void endScene_sceneSetAossComplete(void) {
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene1_sceneSetAossComplete);
}

void endScene1_sceneSetAossComplete(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_COMMENTlDelete();
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x15);
    DWCi_SetParam_(0, 0);
    DWCi_SetParam2_(0, 1);
    DWCi_ChangeScene((u32)DWCi_SceneSetTestConfirm);
}

void wait_sceneSetAossComplete(void) {
    data_ov016_021de5d4 = data_ov016_021de5d4 + 1;
    if (data_ov016_021de5d4 < 0x78) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetAossComplete);
}
