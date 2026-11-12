// libdwcutil's sceneSetRakuButton1.c (Black's ov016 0x021cac44-0x021cb0a8): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetRakuButton1(void);
void procInit_sceneSetRakuButton1(void);
static u8 data_ov016_021de5fc;
void procInit1_sceneSetRakuButton1(void);
void procInit2_sceneSetRakuButton1(void);
void procScene_sceneSetRakuButton1(void);
void endScene_sceneSetRakuButton1(void);
void endScene1_sceneSetRakuButton1(void);
void endScene2_sceneSetRakuButton1(void);
void procError1_sceneSetRakuButton1(void);
void checkRaku_sceneSetRakuButton1(void);
void input_sceneSetRakuButton1(void);
void draw_sceneSetRakuButton1(void);
void button_sceneSetRakuButton1(void);
BOOL DWCi_IsFold_sceneSetRakuButton1(void);
void procError_sceneSetRakuButton1(void);
void DWCi_RAKUlSetResult(void);

BOOL DWCi_IsFold_sceneSetRakuButton1(void) {
    return (u32)((s32)(*(u16 *)0x02ffffa8 & 0x8000) >> 0xf != 0);
}

void DWCi_SceneSetRakuButton1(void) {
    data_ov016_021de5fc = 0;
    initGraphics_sceneSetRakuButton1();
    DWCi_HLlLoadSet();
    DWCi_CMNlMsgDraw(0x2a);
    DWCi_ANIMElInitEx(2);
    DWCi_ChangeScene((u32)procInit_sceneSetRakuButton1);
}

void initGraphics_sceneSetRakuButton1(void) {
    DWCi_CMNlLoadResource("char/xb4Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetRakuButton1(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetRakuButton1);
}

void procInit1_sceneSetRakuButton1(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_CMNlButton(1);
    DWCi_ChangeScene((u32)procInit2_sceneSetRakuButton1);
}

void procInit2_sceneSetRakuButton1(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetRakuButton1);
}

void procScene_sceneSetRakuButton1(void) {
    OS_Sleep(0xAU);
    checkRaku_sceneSetRakuButton1();
    input_sceneSetRakuButton1();
    draw_sceneSetRakuButton1();
    button_sceneSetRakuButton1();
}

void input_sceneSetRakuButton1(void) {
    BOOL BVar2;

    if (DWCi_IPTlCheckTrigger(2)) {
        func_ov016_021bcb28(0);
        return;
    }
    BVar2 = DWCi_IsFold_sceneSetRakuButton1();
    if (BVar2 == 0) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneSetRakuButton1(void) {
}

void button_sceneSetRakuButton1(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    DWCi_SNDlStop();
    DWCi_SNDlPlay(7);
    DWCi_ChangeScene((u32)endScene_sceneSetRakuButton1);
}

void endScene_sceneSetRakuButton1(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetRakuButton1);
}

void endScene1_sceneSetRakuButton1(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    if (data_ov016_021de5fc == 0) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetRakuButton1);
}

void endScene2_sceneSetRakuButton1(void) {
    u8 result;

    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if ((data_ov016_021de5fc == 0) && (result = DWCi_EFFlCheck(1), result != 0)) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_ANIMElEnd();
    DWCi_FNTlDeleteBgLcd(0);
    if (data_ov016_021de5fc == 0) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1);
    }
    DWCi_RAKUlEnd();
    if (data_ov016_021de5fc == 0) {
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetSelectWay);
        return;
    }
    DWCi_SetParam_(0, 0);
    DWCi_ChangeScene((u32)DWCi_SceneSetRakuComplete);
}

void checkRaku_sceneSetRakuButton1(void) {
    switch (DWCi_RAKUlGetState()) {
    case 3:
        data_ov016_021de5fc = 1;
        DWCi_SNDlStop();
        DWCi_RAKUlSetResult();
        DWCi_ChangeScene((u32)endScene_sceneSetRakuButton1);
        break;
    case 4:
        data_ov016_021de5fc = 0;
        DWCi_SNDlStop();
        DWCi_SNDlPlay(9);
        DWCi_WINlInitEx(0xd, 1, 1, -1, 0);
        DWCi_BTNlDisable();
        DWCi_ChangeScene((u32)procError_sceneSetRakuButton1);
        break;
    case 5:
        data_ov016_021de5fc = 0;
        DWCi_SNDlStop();
        DWCi_SNDlPlay(0x12);
        DWCi_WINlInitEx(0xe, 1, 1, -1, 0);
        DWCi_BTNlDisable();
        DWCi_ChangeScene((u32)procError_sceneSetRakuButton1);
        break;
    }
}


void procError_sceneSetRakuButton1(void) {
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procError1_sceneSetRakuButton1);
}

void procError1_sceneSetRakuButton1(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetRakuButton1);
}
