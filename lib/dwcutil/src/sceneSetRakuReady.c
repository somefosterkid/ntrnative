// libdwcutil's sceneSetRakuReady.c (Black's ov016 0x021cb2c8-0x021cb780): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetRakuReady(void);
void procInit_sceneSetRakuReady(void);
static u8 data_ov016_021de604;
void procInit1_sceneSetRakuReady(void);
void procInit2_sceneSetRakuReady(void);
void procScene_sceneSetRakuReady(void);
void endScene_sceneSetRakuReady(void);
void endScene1_sceneSetRakuReady(void);
void endScene2_sceneSetRakuReady(void);
void procError_sceneSetRakuReady(void);
void procError1_sceneSetRakuReady(void);
void checkRaku_sceneSetRakuReady(void);
void input_sceneSetRakuReady(void);
void draw_sceneSetRakuReady(void);
void button_sceneSetRakuReady(void);
BOOL DWCi_IsFold_sceneSetRakuReady(void);
void DWCi_SceneSetRakuReady(void);

BOOL DWCi_IsFold_sceneSetRakuReady(void) {
    return (u32)((s32)(*(u16 *)0x02ffffa8 & 0x8000) >> 0xf != 0);
}

void DWCi_SceneSetRakuReady(void) {
    data_ov016_021de604 = 0;
    initGraphics_sceneSetRakuReady();
    DWCi_HLlLoadSet();
    DWCi_COMMENTlDispEx(0x37, -1, 0);
    DWCi_CMNlStep(2);
    DWCi_CMNlMsgDraw(0x24);
    DWCi_ANIMElInitEx(0);
    DWCi_SNDlPlay(0xb);
    DWCi_ChangeScene((u32)procInit_sceneSetRakuReady);
}

void initGraphics_sceneSetRakuReady(void) {
    DWCi_CMNlLoadResource("char/jbBgStep3.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/ybBgStep3.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/xb4Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetRakuReady(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetRakuReady);
}

void procInit1_sceneSetRakuReady(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_RAKUlInit();
    DWCi_CMNlButton(1);
    DWCi_ChangeScene((u32)procInit2_sceneSetRakuReady);
}

void procInit2_sceneSetRakuReady(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetRakuReady);
}

void procScene_sceneSetRakuReady(void) {
    OS_Sleep(10);
    checkRaku_sceneSetRakuReady();
    input_sceneSetRakuReady();
    draw_sceneSetRakuReady();
    button_sceneSetRakuReady();
}

void input_sceneSetRakuReady(void) {
    BOOL BVar2;

    if (DWCi_IPTlCheckTrigger(2)) {
        func_ov016_021bcb28(0);
        return;
    }
    BVar2 = DWCi_IsFold_sceneSetRakuReady();
    if (BVar2 == 0) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneSetRakuReady(void) {
}

void button_sceneSetRakuReady(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    DWCi_SNDlStop();
    DWCi_SNDlPlay(7);
    DWCi_ChangeScene((u32)endScene_sceneSetRakuReady);
}

void endScene_sceneSetRakuReady(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetRakuReady);
}

void endScene1_sceneSetRakuReady(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (data_ov016_021de604 == 0) {
        DWCi_BTNlEnd();
    }
    if (data_ov016_021de604 == 0) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x14, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetRakuReady);
}

void endScene2_sceneSetRakuReady(void) {
    u8 result;

    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if ((data_ov016_021de604 == 0) && (result = DWCi_EFFlCheck(1), result != 0)) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_ANIMElEnd();
    DWCi_FNTlDeleteBgLcd(0);
    if (data_ov016_021de604 == 0) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1);
    }
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de604 == 0) {
        DWCi_RAKUlEnd();
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetSelectWay);
        return;
    }
    DWCi_SetParam_(0, 0);
    DWCi_ChangeScene((u32)DWCi_SceneSetRakuButton);
}

void checkRaku_sceneSetRakuReady(void) {
    switch (DWCi_RAKUlGetState()) {
    case 1:
        data_ov016_021de604 = 1;
        DWCi_ChangeScene((u32)endScene_sceneSetRakuReady);
        return;
    case 4:
        data_ov016_021de604 = 0;
        DWCi_SNDlStop();
        DWCi_SNDlPlay(9);
        DWCi_WINlInitEx(0xd, 1, 1, -1, 0);
        DWCi_BTNlDisable();
        DWCi_ChangeScene((u32)procError_sceneSetRakuReady);
        return;
    default:
        return;
    }
}

void procError_sceneSetRakuReady(void) {
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procError1_sceneSetRakuReady);
}

void procError1_sceneSetRakuReady(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetRakuReady);
}
