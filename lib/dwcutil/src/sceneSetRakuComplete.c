// libdwcutil's sceneSetRakuComplete.c (Black's ov016 0x021cb0a8-0x021cb2c8): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetRakuComplete(void);
void procInit_sceneSetRakuComplete(void);
static u8 data_ov016_021de600;
void procInit1_sceneSetRakuComplete(void);
void procScene_sceneSetRakuComplete(void);
void wait_sceneSetRakuComplete(void);
void draw_sceneSetRakuComplete(void);
void endScene1_sceneSetRakuComplete(void);
void endScene_sceneSetRakuComplete(void);

void DWCi_SceneSetRakuComplete(void) {
    data_ov016_021de600 = 0;
    initGraphics_sceneSetRakuComplete();
    DWCi_HLlLoadSet();
    DWCi_CMNlMsgDraw(0x26);
    DWCi_SNDlPlay(0x10);
    DWCi_ChangeScene((u32)procInit_sceneSetRakuComplete);
}

void initGraphics_sceneSetRakuComplete(void) {
    DWCi_CMNlLoadResource("char/xb4Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetRakuComplete(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetRakuComplete);
}

void procInit1_sceneSetRakuComplete(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneSetRakuComplete);
}

void procScene_sceneSetRakuComplete(void) {
    wait_sceneSetRakuComplete();
    draw_sceneSetRakuComplete();
}

void draw_sceneSetRakuComplete(void) {
}

void endScene_sceneSetRakuComplete(void) {
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene1_sceneSetRakuComplete);
}

void endScene1_sceneSetRakuComplete(void) {
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

void wait_sceneSetRakuComplete(void) {
    data_ov016_021de600 = data_ov016_021de600 + 1;
    if (data_ov016_021de600 < 0x78) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetRakuComplete);
}
