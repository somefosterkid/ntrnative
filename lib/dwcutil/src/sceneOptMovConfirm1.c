// libdwcutil's sceneOptMovConfirm1.c (Black's ov016 0x021c3824-0x021c3b8c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneOptMovConfirm1(void);
void procInit_sceneOptMovConfirm1(void);
static u8 data_ov016_021de5b8;
void procInit1_sceneOptMovConfirm1(void);
void procInit2_sceneOptMovConfirm1(void);
void procScene_sceneOptMovConfirm1(void);
void input_sceneOptMovConfirm1(void);
void draw_sceneOptMovConfirm1(void);
void button_sceneOptMovConfirm1(void);
void endScene1_sceneOptMovConfirm1(void);
void endScene2_sceneOptMovConfirm1(void);
void endScene_sceneOptMovConfirm1(void);

void DWCi_SceneOptMovConfirm1(void) {
    data_ov016_021de5b8 = 0;
    initGraphics_sceneOptMovConfirm1();
    DWCi_HLlLoad(0x13);
    DWCi_COMMENTlDispEx(0x3c, -1, 0);
    DWCi_CMNlMsgDraw(0x1b);
    DWCi_ChangeScene((u32)procInit_sceneOptMovConfirm1);
}

void initGraphics_sceneOptMovConfirm1(void) {
    DWCi_CMNlLoadResource("char/yb5Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneOptMovConfirm1(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneOptMovConfirm1);
}

void procInit1_sceneOptMovConfirm1(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_CMNlButton(3);
    DWCi_ChangeScene((u32)procInit2_sceneOptMovConfirm1);
}

void procInit2_sceneOptMovConfirm1(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneOptMovConfirm1);
}

void procScene_sceneOptMovConfirm1(void) {
    input_sceneOptMovConfirm1();
    draw_sceneOptMovConfirm1();
    button_sceneOptMovConfirm1();
}

void input_sceneOptMovConfirm1(void) {
    BOOL result;

    result = DWCi_IPTlCheckTrigger(1);
    if (result) {
        func_ov016_021bcb28(1);
    }
    result = DWCi_IPTlCheckTrigger(2);
    if (!result) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneOptMovConfirm1(void) {
}

void button_sceneOptMovConfirm1(void) {
    switch (DWCi_BTNlGet()) {
    case 0:
        DWCi_SNDlPlay(7);
        break;
    case 1:
        DWCi_SNDlPlay(6);
        data_ov016_021de5b8 = 1;
        break;
    default:
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneOptMovConfirm1);
}

void endScene_sceneOptMovConfirm1(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneOptMovConfirm1);
}

void endScene1_sceneOptMovConfirm1(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    if (data_ov016_021de5b8 == 0) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneOptMovConfirm1);
}

void endScene2_sceneOptMovConfirm1(void) {
    u8 result;

    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if ((data_ov016_021de5b8 == 0) && (result = DWCi_EFFlCheck(1), result != 0)) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    if (data_ov016_021de5b8 == 0) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1);
    }
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de5b8 == 0) {
        DWCi_SetParam_(0, 1);
        DWCi_ChangeScene((u32)DWCi_SceneOptMenu);
        return;
    }
    DWCi_SetParam_(0, 1);
    DWCi_ChangeScene((u32)DWCi_SceneOptMovConfirm);
}
