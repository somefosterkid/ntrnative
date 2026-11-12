// libdwcutil's sceneOptErase.c (Black's ov016 0x021c1f1c-0x021c231c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneOptErase(void);
void procInit_sceneOptErase(void);
static u8 data_ov016_021de5a4;
void procInit1_sceneOptErase(void);
void procInit2_sceneOptErase(void);
void procScene_sceneOptErase(void);
void input_sceneOptErase(void);
void draw_sceneOptErase(void);
void button_sceneOptErase(void);
void endScene_sceneOptErase(void);
void procConfirm(void);
void endScene1_sceneOptErase(void);
void endScene2_sceneOptErase(void);
void procConfirm1(void);

void DWCi_SceneOptErase(void) {
    data_ov016_021de5a4 = 0;
    initGraphics_sceneOptErase();
    DWCi_HLlLoad(0x12);
    DWCi_COMMENTlDispEx(0x3b, -1, 0);
    DWCi_CMNlMsgDraw(0x17);
    DWCi_ChangeScene((u32)procInit_sceneOptErase);
}

void initGraphics_sceneOptErase(void) {
    DWCi_CMNlLoadResource("char/yb5Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneOptErase(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneOptErase);
}

void procInit1_sceneOptErase(void) {
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
    DWCi_ChangeScene((u32)procInit2_sceneOptErase);
}

void procInit2_sceneOptErase(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneOptErase);
}

void procScene_sceneOptErase(void) {
    input_sceneOptErase();
    draw_sceneOptErase();
    button_sceneOptErase();
}

void input_sceneOptErase(void) {
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

void draw_sceneOptErase(void) {
}

void button_sceneOptErase(void) {
    switch (DWCi_BTNlGet()) {
    case 0:
        DWCi_SNDlPlay(7);
        DWCi_ChangeScene((u32)endScene_sceneOptErase);
        return;
    case 1:
        DWCi_SNDlPlay(6);
        DWCi_WINlInitEx(0x18, 0, 1, -1, 0);
        DWCi_BTNlDisable();
        DWCi_ChangeScene((u32)procConfirm);
        return;
    default:
        return;
    }
}

void endScene_sceneOptErase(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneOptErase);
}

void endScene1_sceneOptErase(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    if (data_ov016_021de5a4 == 0) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneOptErase);
}

void endScene2_sceneOptErase(void) {
    u8 result;

    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if ((data_ov016_021de5a4 == 0) && (result = DWCi_EFFlCheck(1), result != 0)) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    if (data_ov016_021de5a4 == 0) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1);
    }
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de5a4 == 0) {
        DWCi_SetParam_(0, 1);
        DWCi_ChangeScene((u32)DWCi_SceneOptMenu);
        return;
    }
    DWCi_SetParam_(0, 1);
    DWCi_ChangeScene((u32)DWCi_SceneOptErase1);
}

void procConfirm(void) {
    s32 result;
    result = DWCi_WINlGet();
    if (result != 0) {
        if (result != 1) {
            return;
        }
        DWCi_SNDlPlay(0xe);
        data_ov016_021de5a4 = 1;
    } else {
        DWCi_SNDlPlay(7);
    }
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procConfirm1);
}

void procConfirm1(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneOptErase);
}
