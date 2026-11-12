// libdwcutil's sceneSetUsbReady1.c (Black's ov016 0x021ce050-0x021ce668): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetUsbReady1(void);
u32 DWCi_USBAPlBegin(u32 param_1);
void cbUsb(u32 param_1);
void procInit_sceneSetUsbReady1(void);
static u8 data_ov016_021de62c;
void procInit1_sceneSetUsbReady1(void);
void procInit2_sceneSetUsbReady1(void);
void procScene_sceneSetUsbReady1(void);
void input_sceneSetUsbReady1(void);
void draw_sceneSetUsbReady1(void);
void button_sceneSetUsbReady1(void);
void endScene_sceneSetUsbReady1(void);
void endScene1_sceneSetUsbReady1(void);
void endScene2_sceneSetUsbReady1(void);
u32 DWCi_USBAPlEnd(void);
void procNotFound(void);
void procBoth(void);
void procNotFound1(void);
BOOL DWCi_IsFold_sceneSetUsbReady1(void);
void procBoth1(void);

BOOL DWCi_IsFold_sceneSetUsbReady1(void) {
    return (u32)((s32)(*(u16 *)0x02ffffa8 & 0x8000) >> 0xf != 0);
}

void DWCi_SceneSetUsbReady1(void) {
    u8 auStack_78 [22];
    OSOwnerInfo OStack_62;

    data_ov016_021de62c = 0;
    initGraphics_sceneSetUsbReady1();
    DWCi_HLlLoad(8);
    DWCi_CMNlStep(2);
    OS_GetOwnerInfo(&OStack_62);
    MIi_CpuClear16(0, auStack_78, 0x16);
    MIi_CpuCopy16(OStack_62.nickName, auStack_78, (u32)OStack_62.nickNameLength << 1);
    DWCi_CMNlMsgNameDraw(auStack_78, 0x28);
    DWCi_ANIMElInitEx(0);
    DWCi_USBAPlBegin((u32)cbUsb);
    DWCi_SNDlPlay(0xb);
    DWCi_ChangeScene((u32)procInit_sceneSetUsbReady1);
}

void initGraphics_sceneSetUsbReady1(void) {
    DWCi_CMNlLoadResource("char/jbBgStep3.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/ybBgStep3.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/jb4Usb.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetUsbReady1(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetUsbReady1);
}

void procInit1_sceneSetUsbReady1(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_CMNlButton(2);
    DWCi_ChangeScene((u32)procInit2_sceneSetUsbReady1);
}

void procInit2_sceneSetUsbReady1(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetUsbReady1);
}

void procScene_sceneSetUsbReady1(void) {
    input_sceneSetUsbReady1();
    draw_sceneSetUsbReady1();
    button_sceneSetUsbReady1();
}

void input_sceneSetUsbReady1(void) {
    BOOL BVar2;

    if (DWCi_IPTlCheckTrigger(2)) {
        func_ov016_021bcb28(0);
        return;
    }
    BVar2 = DWCi_IsFold_sceneSetUsbReady1();
    if (BVar2 == 0) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneSetUsbReady1(void) {
}

void button_sceneSetUsbReady1(void) {
    if (data_ov016_021de62c != 0) {
        return;
    }
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    data_ov016_021de62c = 2;
    DWCi_SNDlStop();
    DWCi_SNDlPlay(7);
    DWCi_ChangeScene((u32)endScene_sceneSetUsbReady1);
}

void endScene_sceneSetUsbReady1(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetUsbReady1);
}

void endScene1_sceneSetUsbReady1(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    if ((data_ov016_021de62c == 2 || data_ov016_021de62c == 3)) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetUsbReady1);
}

void endScene2_sceneSetUsbReady1(void) {
    u8 result;
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if ((data_ov016_021de62c == 2) && (result = DWCi_EFFlCheck(1), result != 0)) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_USBAPlEnd();
    DWCi_ANIMElEnd();
    DWCi_FNTlDeleteBgLcd(0);
    if ((data_ov016_021de62c == 2 || data_ov016_021de62c == 3)) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1);
    }
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de62c == 2) {
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetSelectAp);
        return;
    }
    if (data_ov016_021de62c == 3) {
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetUsbReady);
        return;
    }
    DWCi_SetParam_(0, 0);
    DWCi_ChangeScene((u32)DWCi_SceneSetUsbComplete);
}

void cbUsb(u32 state) {
    if (data_ov016_021de62c != 0) {
        return;
    }
    switch(state) {
    case 0:
        data_ov016_021de62c = 3;
        DWCi_SNDlStop();
        DWCi_SNDlPlay(0x12);
        DWCi_WINlInitEx(0x10, 1, 1, -1, 0);
        DWCi_BTNlDisable();
        DWCi_ChangeScene((u32)procNotFound);
        return;
    case 1:
        data_ov016_021de62c = 1;
        DWCi_SNDlStop();
        DWCi_ChangeScene((u32)endScene_sceneSetUsbReady1);
        return;
    case 2:
        DWCi_SNDlStop();
        DWCi_USBAPlSetCallback(0);
        DWCi_WINlInitEx(0x11, 0, 1, -1, 0);
        DWCi_BTNlDisable();
        DWCi_ChangeScene((u32)procBoth);
        return;
    case 3:
        data_ov016_021de62c = 2;
        DWCi_SNDlStop();
        DWCi_SNDlPlay(9);
        DWCi_ChangeScene((u32)endScene_sceneSetUsbReady1);
    }
}

void procNotFound(void) {
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procNotFound1);
}

void procNotFound1(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetUsbReady1);
}

#pragma thumb off
void procBoth(void) {
    switch (DWCi_WINlGet()) {
    default:
        return;
    case 1:
        data_ov016_021de62c = 3;
        DWCi_SNDlPlay(6);
        break;
    case 0:
        data_ov016_021de62c = 1;
        DWCi_SNDlPlay(7);
        break;
    }
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procBoth1);
}
#pragma thumb reset


void procBoth1(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    if (data_ov016_021de62c == 1) {
        DWCi_ChangeScene((u32)endScene_sceneSetUsbReady1);
        return;
    }
    DWCi_SNDlPlay(0xb);
    DWCi_BTNlEnable();
    data_ov016_021de62c = 0;
    DWCi_USBAPlSetCallback((u32)cbUsb);
    DWCi_ChangeScene((u32)procScene_sceneSetUsbReady1);
}
