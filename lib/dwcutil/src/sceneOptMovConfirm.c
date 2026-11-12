// libdwcutil's sceneOptMovConfirm.c (Black's ov016 0x021c35f0-0x021c3824): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneOptMovConfirm(void);
void procInit_sceneOptMovConfirm(void);
void procInit1_sceneOptMovConfirm(void);
void procInit2_sceneOptMovConfirm(void);
void procScene_sceneOptMovConfirm(void);
void input_sceneOptMovConfirm(void);
void draw_sceneOptMovConfirm(void);
void button_sceneOptMovConfirm(void);
void endScene_sceneOptMovConfirm(void);
void endScene1_sceneOptMovConfirm(void);
void endScene2_sceneOptMovConfirm(void);

void DWCi_SceneOptMovConfirm(void) {
    initGraphics_sceneOptMovConfirm();
    DWCi_CMNlMsgDraw(0x1a);
    DWCi_ChangeScene((u32)procInit_sceneOptMovConfirm);
}

void initGraphics_sceneOptMovConfirm(void) {
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneOptMovConfirm(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneOptMovConfirm);
}

void procInit1_sceneOptMovConfirm(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_CMNlButton(4);
    DWCi_ChangeScene((u32)procInit2_sceneOptMovConfirm);
}

void procInit2_sceneOptMovConfirm(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneOptMovConfirm);
}

void procScene_sceneOptMovConfirm(void) {
    input_sceneOptMovConfirm();
    draw_sceneOptMovConfirm();
    button_sceneOptMovConfirm();
}

void input_sceneOptMovConfirm(void) {
    if (!DWCi_IPTlCheckTrigger(1)) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneOptMovConfirm(void) {
}

void button_sceneOptMovConfirm(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_ChangeScene((u32)endScene_sceneOptMovConfirm);
}

void endScene_sceneOptMovConfirm(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneOptMovConfirm);
}

void endScene1_sceneOptMovConfirm(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneOptMovConfirm);
}

void endScene2_sceneOptMovConfirm(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_GXlInvisible(0, 0x15);
    DWCi_SetParam_(0, 1);
    DWCi_ChangeScene((u32)DWCi_SceneOptMovWait);
}
