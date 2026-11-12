// libdwcutil's sceneSetUsbComplete.c (Black's ov016 0x021cda88-0x021cdcac): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetUsbComplete(void);
void procInit_sceneSetUsbComplete(void);
static u8 data_ov016_021de624;
void procInit1_sceneSetUsbComplete(void);
void procScene_sceneSetUsbComplete(void);
void wait_sceneSetUsbComplete(void);
void draw_sceneSetUsbComplete(void);
void endScene1_sceneSetUsbComplete(void);
void endScene_sceneSetUsbComplete(void);

void DWCi_SceneSetUsbComplete(void) {
    data_ov016_021de624 = 0;
    initGraphics_sceneSetUsbComplete();
    DWCi_HLlLoad(8);
    DWCi_CMNlMsgDraw(0x29);
    DWCi_SNDlPlay(0x10);
    DWCi_ChangeScene((u32)procInit_sceneSetUsbComplete);
}

void initGraphics_sceneSetUsbComplete(void) {
    DWCi_CMNlLoadResource("char/xb4Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetUsbComplete(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetUsbComplete);
}

void procInit1_sceneSetUsbComplete(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneSetUsbComplete);
}

void procScene_sceneSetUsbComplete(void) {
    wait_sceneSetUsbComplete();
    draw_sceneSetUsbComplete();
}

void draw_sceneSetUsbComplete(void) {
}

void endScene_sceneSetUsbComplete(void) {
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene1_sceneSetUsbComplete);
}

void endScene1_sceneSetUsbComplete(void) {
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
    DWCi_SetParam2_(0, 2);
    DWCi_ChangeScene((u32)DWCi_SceneSetTestConfirm);
}

void wait_sceneSetUsbComplete(void) {
    data_ov016_021de624 = data_ov016_021de624 + 1;
    if (data_ov016_021de624 < 0x78) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetUsbComplete);
}
