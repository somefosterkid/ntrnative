// libdwcutil's sceneOptErase1.c (Black's ov016 0x021c231c-0x021c255c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneOptErase1(void);
void procInit_sceneOptErase1(void);
void procInit1_sceneOptErase1(void);
void procInit2_sceneOptErase1(void);
void procScene_sceneOptErase1(void);
void input_sceneOptErase1(void);
void draw_sceneOptErase1(void);
void button_sceneOptErase1(void);
void endScene_sceneOptErase1(void);
void func_ov016_021c24cc(void);
void endScene2_sceneOptErase1(void);

void DWCi_SceneOptErase1(void) {
    initGraphics_sceneOptErase1();
    DWCi_CMNlMsgDraw(0x19);
    DWCi_SETTINGlEraseAll();
    DWCi_ChangeScene((u32)procInit_sceneOptErase1);
}

void initGraphics_sceneOptErase1(void) {
    DWCi_CMNlLoadResource("char/yb5Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneOptErase1(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneOptErase1);
}

void procInit1_sceneOptErase1(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_CMNlButton(5);
    DWCi_ChangeScene((u32)procInit2_sceneOptErase1);
}

void procInit2_sceneOptErase1(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneOptErase1);
}

void procScene_sceneOptErase1(void) {
    input_sceneOptErase1();
    draw_sceneOptErase1();
    button_sceneOptErase1();
}

void input_sceneOptErase1(void) {
    if (!DWCi_IPTlCheckTrigger(1)) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneOptErase1(void) {
}

void button_sceneOptErase1(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_ChangeScene((u32)endScene_sceneOptErase1);
}

void endScene_sceneOptErase1(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)func_ov016_021c24cc);
}

void func_ov016_021c24cc(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    func_ov016_021d55a4(3, 1, 0x3f, 0x40);
    func_ov016_021d55a4(3, 0, 0x3f, 0x40);
    DWCi_ChangeScene((u32)endScene2_sceneOptErase1);
}

void endScene2_sceneOptErase1(void) {
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
