// libdwcutil's sceneOptMovComplete.c (Black's ov016 0x021c33cc-0x021c35f0): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneOptMovComplete(void);
void procInit_sceneOptMovComplete(void);
void procInit1_sceneOptMovComplete(void);
void procInit2_sceneOptMovComplete(void);
void procScene_sceneOptMovComplete(void);
void input_sceneOptMovComplete(void);
void draw_sceneOptMovComplete(void);
void button_sceneOptMovComplete(void);
void endScene_sceneOptMovComplete(void);
void func_ov016_021c3560(void);
void endScene2_sceneOptMovComplete(void);

void DWCi_SceneOptMovComplete(void) {
    initGraphics_sceneOptMovComplete();
    DWCi_CMNlMsgDraw(0x1f);
    DWCi_ChangeScene((u32)procInit_sceneOptMovComplete);
}

void initGraphics_sceneOptMovComplete(void) {
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneOptMovComplete(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneOptMovComplete);
}

void procInit1_sceneOptMovComplete(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_CMNlButton(5);
    DWCi_ChangeScene((u32)procInit2_sceneOptMovComplete);
}

void procInit2_sceneOptMovComplete(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneOptMovComplete);
}

void procScene_sceneOptMovComplete(void) {
    input_sceneOptMovComplete();
    draw_sceneOptMovComplete();
    button_sceneOptMovComplete();
}

void input_sceneOptMovComplete(void) {
    if (!DWCi_IPTlCheckTrigger(1)) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneOptMovComplete(void) {
}

void button_sceneOptMovComplete(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_ChangeScene((u32)endScene_sceneOptMovComplete);
}

void endScene_sceneOptMovComplete(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)func_ov016_021c3560);
}

void func_ov016_021c3560(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    func_ov016_021d55a4(3, 1, 0x3f, 0x40);
    func_ov016_021d55a4(3, 0, 0x3f, 0x40);
    DWCi_ChangeScene((u32)endScene2_sceneOptMovComplete);
}

void endScene2_sceneOptMovComplete(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    OS_SpinWaitSysCycles(0x800000);
    PM_ForceToPowerOff();
}
