// libdwcutil's sceneSetRakuButton.c (Black's ov016 0x021ca818-0x021cac44): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetRakuButton(void);
void procInit_sceneSetRakuButton(void);
static u8 data_ov016_021de5f8;
void procInit1_sceneSetRakuButton(void);
void procInit2_sceneSetRakuButton(void);
void procScene_sceneSetRakuButton(void);
void endScene_sceneSetRakuButton(void);
void endScene1_sceneSetRakuButton(void);
void endScene2_sceneSetRakuButton(void);
void procError_sceneSetRakuButton(void);
void procError1_sceneSetRakuButton(void);
void checkRaku_sceneSetRakuButton(void);
void input_sceneSetRakuButton(void);
void draw_sceneSetRakuButton(void);
void button_sceneSetRakuButton(void);
BOOL DWCi_IsFold_sceneSetRakuButton(void);

BOOL DWCi_IsFold_sceneSetRakuButton(void) {
    return (u32)((s32)(*(u16 *)0x02ffffa8 & 0x8000) >> 0xf != 0);
}

void DWCi_SceneSetRakuButton(void) {
    data_ov016_021de5f8 = 0;
    initGraphics_sceneSetRakuButton();
    DWCi_HLlLoadSet();
    DWCi_CMNlMsgDraw(0x25);
    DWCi_ANIMElInitEx(1);
    DWCi_ChangeScene((u32)procInit_sceneSetRakuButton);
}

void initGraphics_sceneSetRakuButton(void) {
    DWCi_CMNlLoadResource("char/xb4Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetRakuButton(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetRakuButton);
}

void procInit1_sceneSetRakuButton(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_CMNlButton(1);
    DWCi_ChangeScene((u32)procInit2_sceneSetRakuButton);
}

void procInit2_sceneSetRakuButton(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetRakuButton);
}

void procScene_sceneSetRakuButton(void) {
    OS_Sleep(10);
    checkRaku_sceneSetRakuButton();
    input_sceneSetRakuButton();
    draw_sceneSetRakuButton();
    button_sceneSetRakuButton();
}

void input_sceneSetRakuButton(void) {
    BOOL BVar2;

    if (DWCi_IPTlCheckTrigger(2)) {
        func_ov016_021bcb28(0);
        return;
    }
    BVar2 = DWCi_IsFold_sceneSetRakuButton();
    if (BVar2 == 0) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneSetRakuButton(void) {
}

void button_sceneSetRakuButton(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    DWCi_SNDlStop();
    DWCi_SNDlPlay(7);
    DWCi_ChangeScene((u32)endScene_sceneSetRakuButton);
}

void endScene_sceneSetRakuButton(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetRakuButton);
}

void endScene1_sceneSetRakuButton(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (data_ov016_021de5f8 == 0) {
        DWCi_BTNlEnd();
    }
    if (data_ov016_021de5f8 == 0) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetRakuButton);
}

void endScene2_sceneSetRakuButton(void) {
    u8 result;

    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if ((data_ov016_021de5f8 == 0) && (result = DWCi_EFFlCheck(1), result != 0)) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_ANIMElEnd();
    DWCi_FNTlDeleteBgLcd(0);
    if (data_ov016_021de5f8 == 0) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1);
    }
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de5f8 == 0) {
        DWCi_RAKUlEnd();
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetSelectWay);
        return;
    }
    DWCi_SetParam_(0, 0);
    DWCi_ChangeScene((u32)DWCi_SceneSetRakuButton1);
}

void checkRaku_sceneSetRakuButton(void) {
    switch (DWCi_RAKUlGetState()) {
    case 2:
        data_ov016_021de5f8 = 1;
        DWCi_ChangeScene((u32)endScene_sceneSetRakuButton);
        return;
    case 4:
        data_ov016_021de5f8 = 0;
        DWCi_SNDlStop();
        DWCi_SNDlPlay(9);
        DWCi_WINlInitEx(0xd, 1, 1, -1, 0);
        DWCi_BTNlDisable();
        DWCi_ChangeScene((u32)procError_sceneSetRakuButton);
        return;
    default:
        return;
    }
}

void procError_sceneSetRakuButton(void) {
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procError1_sceneSetRakuButton);
}

void procError1_sceneSetRakuButton(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetRakuButton);
}
