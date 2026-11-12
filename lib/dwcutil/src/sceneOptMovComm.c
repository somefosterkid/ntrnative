// libdwcutil's sceneOptMovComm.c (Black's ov016 0x021c315c-0x021c33cc): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneOptMovComm(void);
void procInit_sceneOptMovComm(void);
void procInit1_sceneOptMovComm(void);
void cbMov_sceneOptMovComm(s32 param_1);
void procScene_sceneOptMovComm(void);
void input_sceneOptMovComm(void);
void draw_sceneOptMovComm(void);
void endScene1_sceneOptMovComm(void);
static u8 data_ov016_021de5b4;
void endScene_sceneOptMovComm(void);

void DWCi_SceneOptMovComm(void) {
    initGraphics_sceneOptMovComm();
    DWCi_CMNlMsgDraw(0x1e);
    DWCi_ANIMElInitEx(0);
    DWCi_ICONlInit(1);
    DWCi_SNDlPlay(0xb);
    DWCi_ChangeScene((u32)procInit_sceneOptMovComm);
}

void initGraphics_sceneOptMovComm(void) {
    DWCi_CMNlLoadResource("char/yb5Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneOptMovComm(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneOptMovComm);
}

void procInit1_sceneOptMovComm(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_MOVlChangeCallback((u32)cbMov_sceneOptMovComm);
    DWCi_ChangeScene((u32)procScene_sceneOptMovComm);
}

void procScene_sceneOptMovComm(void) {
    input_sceneOptMovComm();
    draw_sceneOptMovComm();
}

void input_sceneOptMovComm(void) {
}

void draw_sceneOptMovComm(void) {
}

void endScene_sceneOptMovComm(void) {
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene1_sceneOptMovComm);
}

void endScene1_sceneOptMovComm(void) {
    BOOL BVar2;

    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    BVar2 = DWCi_MOVlIsEnd();
    if (BVar2 == 0) {
        return;
    }
    DWCi_ICONlEnd();
    DWCi_ANIMElEnd();
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_GXlInvisible(0, 0x15);
    DWCi_SetParam_(0, 1);
    if (data_ov016_021de5b4 == 0) {
        DWCi_ChangeScene((u32)DWCi_SceneOptMovFailure);
        return;
    }
    if (data_ov016_021de5b4 == 2) {
        DWCi_ChangeScene((u32)DWCi_SceneOptMovDisconnect);
        return;
    }
    DWCi_ChangeScene((u32)DWCi_SceneOptMovComplete);
}

void cbMov_sceneOptMovComm(s32 state) {
    if (state == 2) {
        DWCi_SETTINGlEraseAll();
        data_ov016_021de5b4 = 1;
        DWCi_SNDlStop();
        DWCi_SNDlPlay(0x10);
    } else if (state == 3) {
        data_ov016_021de5b4 = 2;
        DWCi_SNDlStop();
        DWCi_SNDlPlay(0x12);
    } else {
        data_ov016_021de5b4 = 0;
        DWCi_SNDlStop();
        DWCi_SNDlPlay(0x12);
    }
    DWCi_MOVlChangeCallback(0);
    DWCi_MOVlEnd();
    DWCi_ChangeScene((u32)endScene_sceneOptMovComm);
}
