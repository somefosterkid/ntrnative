// libdwcutil's sceneOptMovUser.c (Black's ov016 0x021c4110-0x021c44ec): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneOptMovUser(void);
void cbMov_sceneOptMovUser(void);
void procInit_sceneOptMovUser(void);
static u8 data_ov016_021de5c4[4];
void procInit1_sceneOptMovUser(void);
void procInit2_sceneOptMovUser(void);
void procScene_sceneOptMovUser(void);
void endScene1_sceneOptMovUser(void);
void endScene2_sceneOptMovUser(void);
void input_sceneOptMovUser(void);
void draw_sceneOptMovUser(void);
void endScene_sceneOptMovUser(void);

void DWCi_SceneOptMovUser(void) {
    s32 result;
    NNSG2dCharCanvas *pNVar2;
    u8 auStack_30 [24];

    result = DWCi_MOVlGetChild();
    pNVar2 = DWCi_FNTlFormBgEx(0, 0);
    data_ov016_021de5c4[1] = 0;
    data_ov016_021de5c4[0] = 0;
    initGraphics_sceneOptMovUser();
    MIi_CpuClear16(0, auStack_30, 0x16);
    MIi_CpuCopy16((void *)(result + 2), auStack_30, (u32)*(u8 *)(result + 1) << 1);
    DWCi_FNTlDrawRect(pNVar2, 8, 0x35, 0xf0, 0x1c, 2, 0x480, auStack_30);
    DWCi_FNTlRenewBg((s32)pNVar2);
    DWCi_MOVlChangeCallback((u32)cbMov_sceneOptMovUser);
    DWCi_WINlInitEx(0x1d, 4, 0, -1, 0);
    DWCi_ChangeScene((u32)procInit_sceneOptMovUser);
}

void initGraphics_sceneOptMovUser(void) {
    DWCi_CMNlLoadResource("char/jb5Move.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneOptMovUser(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneOptMovUser);
}

void procInit1_sceneOptMovUser(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_ChangeScene((u32)procInit2_sceneOptMovUser);
}

void procInit2_sceneOptMovUser(void) {
    if (DWCi_WINlGet() == -2) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneOptMovUser);
}

void procScene_sceneOptMovUser(void) {
    s32 win;

    input_sceneOptMovUser();
    draw_sceneOptMovUser();
    win = DWCi_WINlGet();
    switch (win) {
    case 0:
        data_ov016_021de5c4[1] = 0;
        DWCi_SNDlPlay(7);
        break;
    case 1:
        data_ov016_021de5c4[1] = 1;
        DWCi_SNDlPlay(14);
        break;
    default:
        return;
    }
    DWCi_MOVlChangeCallback(0);
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)endScene_sceneOptMovUser);
}


void input_sceneOptMovUser(void) {
}

void draw_sceneOptMovUser(void) {
}

void endScene_sceneOptMovUser(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    if (data_ov016_021de5c4[1] == 0) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene1_sceneOptMovUser);
}

void endScene1_sceneOptMovUser(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    if ((*(u8 *)((void *)data_ov016_021de5c4) == 0) && (*(u8 *)((u8 *)((void *)data_ov016_021de5c4) + 1) == 1)) {
        DWCi_MOVlAdmitChild();
    } else {
        DWCi_MOVlEnd();
    }
    DWCi_ChangeScene((u32) endScene2_sceneOptMovUser);
}

void endScene2_sceneOptMovUser(void) {
    BOOL BVar1;

    if (((data_ov016_021de5c4[0] != 0) || (data_ov016_021de5c4[1] == 0)) &&
          (BVar1 = DWCi_MOVlIsEnd(), BVar1 == 0)) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de5c4[1] == 0) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1);
    }
    DWCi_SetParam_(0, 1);
    if (data_ov016_021de5c4[0] != 0) {
        DWCi_ChangeScene((u32)DWCi_SceneOptMovFailure);
        return;
    }
    if (data_ov016_021de5c4[1] == 0) {
        DWCi_GXlInvisible(1, 1);
        DWCi_ChangeScene((u32)DWCi_SceneOptMenu);
        return;
    }
    DWCi_ChangeScene((u32)DWCi_SceneOptMovComm);
}

void cbMov_sceneOptMovUser(void) {
    data_ov016_021de5c4[0] = 1;
}
