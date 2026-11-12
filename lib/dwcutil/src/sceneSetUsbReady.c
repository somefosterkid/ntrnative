// libdwcutil's sceneSetUsbReady.c (Black's ov016 0x021cdcac-0x021ce050): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetUsbReady(void);
void procInit_sceneSetUsbReady(void);
static u8 data_ov016_021de628;
void procInit1_sceneSetUsbReady(void);
void procInit2_sceneSetUsbReady(void);
void procScene_sceneSetUsbReady(void);
void input_sceneSetUsbReady(void);
void draw_sceneSetUsbReady(void);
void button_sceneSetUsbReady(void);
void endScene1_sceneSetUsbReady(void);
void endScene2_sceneSetUsbReady(void);
void endScene_sceneSetUsbReady(void);

void DWCi_SceneSetUsbReady(void) {
    data_ov016_021de628 = 0;
    initGraphics_sceneSetUsbReady();
    DWCi_HLlLoad(8);
    DWCi_COMMENTlDispEx(0x38, -1, 0);
    DWCi_CMNlStep(1);
    DWCi_CMNlMsgDraw(0x27);
    DWCi_ChangeScene((u32)procInit_sceneSetUsbReady);
}

void initGraphics_sceneSetUsbReady(void) {
    DWCi_CMNlLoadResource("char/jbBgStep2.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/ybBgStep2.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/xb3Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetUsbReady(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetUsbReady);
}

void procInit1_sceneSetUsbReady(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_CMNlButton(6);
    DWCi_ChangeScene((u32)procInit2_sceneSetUsbReady);
}

void procInit2_sceneSetUsbReady(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetUsbReady);
}

void procScene_sceneSetUsbReady(void) {
    input_sceneSetUsbReady();
    draw_sceneSetUsbReady();
    button_sceneSetUsbReady();
}

void input_sceneSetUsbReady(void) {
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

void draw_sceneSetUsbReady(void) {
}

void button_sceneSetUsbReady(void) {
    switch (DWCi_BTNlGet()) {
    case 0:
        DWCi_SNDlPlay(7);
        break;
    case 1:
        DWCi_SNDlPlay(6);
        data_ov016_021de628 = 1;
        break;
    default:
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetUsbReady);
}

void endScene_sceneSetUsbReady(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetUsbReady);
}

void endScene1_sceneSetUsbReady(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    if (data_ov016_021de628 == 0) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetUsbReady);
}

void endScene2_sceneSetUsbReady(void) {
    u8 result;

    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if ((data_ov016_021de628 == 0) && (result = DWCi_EFFlCheck(1), result != 0)) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    if (data_ov016_021de628 == 0) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1);
    }
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de628 == 0) {
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetSelectAp);
        return;
    }
    DWCi_SetParam_(2, 1);
    DWCi_ChangeScene((u32)DWCi_SceneSetUsbReady1);
}
