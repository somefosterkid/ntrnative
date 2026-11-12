// libdwcutil's sceneOptMovFailure.c (Black's ov016 0x021c3d8c-0x021c4110): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneOptMovFailure(void);
void procInit_sceneOptMovFailure(void);
static u8 data_ov016_021de5c0[4];
void procInit1_sceneOptMovFailure(void);
void procInit2_sceneOptMovFailure(void);
void procScene_sceneOptMovFailure(void);
void input_sceneOptMovFailure(void);
void draw_sceneOptMovFailure(void);
void button_sceneOptMovFailure(void);
void endScene1_sceneOptMovFailure(void);
void endScene2_sceneOptMovFailure(void);
void endScene_sceneOptMovFailure(void);

void DWCi_SceneOptMovFailure(void) {
    initGraphics_sceneOptMovFailure();
    DWCi_CMNlMsgDraw(0x20);
    data_ov016_021de5c0[0] = (u8)DWCi_COMMENTlDispEx(0x3c, -1, 0);
    DWCi_ChangeScene((u32)procInit_sceneOptMovFailure);
}

void initGraphics_sceneOptMovFailure(void) {
    DWCi_CMNlLoadResource("char/yb5Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneOptMovFailure(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    if (data_ov016_021de5c0[0] != 0) {
        func_ov016_021d55a4(2, 1, 1, 8);
        DWCi_GXlVisible(1, 1);
    }
    DWCi_ChangeScene((u32)procInit1_sceneOptMovFailure);
}

void procInit1_sceneOptMovFailure(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_CMNlButton(3);
    DWCi_ChangeScene((u32)procInit2_sceneOptMovFailure);
}

void procInit2_sceneOptMovFailure(void) {
    BOOL BVar2;

    if (DWCi_BTNlGet() == -2) {
        return;
    }
    BVar2 = DWCi_MOVlIsEnd();
    if (BVar2 == 0) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneOptMovFailure);
}

void procScene_sceneOptMovFailure(void) {
    input_sceneOptMovFailure();
    draw_sceneOptMovFailure();
    button_sceneOptMovFailure();
}

void input_sceneOptMovFailure(void) {
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

void draw_sceneOptMovFailure(void) {
}

void button_sceneOptMovFailure(void) {
    switch (DWCi_BTNlGet()) {
    case 1:
        data_ov016_021de5c0[1] = 1;
        DWCi_SNDlPlay(6);
        break;
    case 0:
        data_ov016_021de5c0[1] = 0;
        DWCi_SNDlPlay(7);
        break;
    default:
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneOptMovFailure);
}


void endScene_sceneOptMovFailure(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneOptMovFailure);
}

void endScene1_sceneOptMovFailure(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    if (data_ov016_021de5c0[1] == 0) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneOptMovFailure);
}

void endScene2_sceneOptMovFailure(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    if (data_ov016_021de5c0[1] == 0) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1);
    }
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de5c0[1] == 0) {
        DWCi_SetParam_(0, 1);
        DWCi_ChangeScene((u32)DWCi_SceneOptMenu);
        return;
    }
    DWCi_SetParam_(0, 1);
    DWCi_ChangeScene((u32)DWCi_SceneOptMovWait);
}
