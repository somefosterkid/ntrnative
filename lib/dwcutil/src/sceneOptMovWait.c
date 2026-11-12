// libdwcutil's sceneOptMovWait.c (Black's ov016 0x021c44ec-0x021c48e4): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void cbMov_sceneOptMovWait(s32 param_1);
void initGraphics_sceneOptMovWait(void);
void procInit_sceneOptMovWait(void);
static u8 data_ov016_021de5c8;
void procInit1_sceneOptMovWait(void);
void procInit2_sceneOptMovWait(void);
void procScene_sceneOptMovWait(void);
void input_sceneOptMovWait(void);
void draw_sceneOptMovWait(void);
void button_sceneOptMovWait(void);
void endScene_sceneOptMovWait(void);
void endScene1_sceneOptMovWait(void);
void endScene2_sceneOptMovWait(void);
BOOL DWCi_IsFold_sceneOptMovWait(void);

BOOL DWCi_IsFold_sceneOptMovWait(void) {
    return (u32)((s32)(*(u16 *)0x02ffffa8 & 0x8000) >> 0xf != 0);
}

void DWCi_SceneOptMovWait(void) {
    DWCi_MOVlInit((u32)cbMov_sceneOptMovWait);
    data_ov016_021de5c8 = 0;
    initGraphics_sceneOptMovWait();
    DWCi_CMNlMsgDraw(0x1c);
    DWCi_ANIMElInitEx(0);
    DWCi_SNDlPlay(0xb);
    DWCi_ChangeScene((u32)procInit_sceneOptMovWait);
}

void initGraphics_sceneOptMovWait(void) {
    DWCi_CMNlLoadResource("char/yb5Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneOptMovWait(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneOptMovWait);
}

void procInit1_sceneOptMovWait(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_CMNlButton(1);
    DWCi_ChangeScene((u32)procInit2_sceneOptMovWait);
}

void procInit2_sceneOptMovWait(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneOptMovWait);
}

void procScene_sceneOptMovWait(void) {
    input_sceneOptMovWait();
    draw_sceneOptMovWait();
    button_sceneOptMovWait();
}

void input_sceneOptMovWait(void) {
    BOOL BVar2;

    if (DWCi_IPTlCheckTrigger(2)) {
        func_ov016_021bcb28(0);
        return;
    }
    BVar2 = DWCi_IsFold_sceneOptMovWait();
    if (BVar2 == 0) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneOptMovWait(void) {
}

void button_sceneOptMovWait(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    DWCi_SNDlStop();
    DWCi_SNDlPlay(7);
    DWCi_ChangeScene((u32)endScene_sceneOptMovWait);
}

void endScene_sceneOptMovWait(void) {
    if (data_ov016_021de5c8 == 0) {
        DWCi_MOVlEnd();
    }
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneOptMovWait);
}

void endScene1_sceneOptMovWait(void) {
    BOOL BVar2;

    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if ((data_ov016_021de5c8 == 0) && (BVar2 = DWCi_MOVlIsEnd(), BVar2 == 0)) {
        return;
    }
    DWCi_BTNlEnd();
    if (data_ov016_021de5c8 == 0) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneOptMovWait);
}

void endScene2_sceneOptMovWait(void) {
    u8 result;
    BOOL BVar3;

    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if ((data_ov016_021de5c8 == 0) && (result = DWCi_EFFlCheck(1), result != 0)) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    if ((data_ov016_021de5c8 == 0) && (BVar3 = DWCi_MOVlIsEnd(), BVar3 == 0)) {
        return;
    }
    DWCi_ANIMElEnd();
    DWCi_FNTlDeleteBgLcd(0);
    if (data_ov016_021de5c8 == 0) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1);
    }
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de5c8 == 0) {
        DWCi_SetParam_(0, 1);
        DWCi_ChangeScene((u32)DWCi_SceneOptMenu);
        return;
    }
    if (data_ov016_021de5c8 == 2) {
        DWCi_SetParam_(0, 1);
        DWCi_ChangeScene((u32)DWCi_SceneOptMovFailure);
        return;
    }
    DWCi_SetParam_(0, 1);
    DWCi_ChangeScene((u32)DWCi_SceneOptMovUser);
}

void cbMov_sceneOptMovWait(s32 state) {
    DWCi_SNDlStop();
    if (state == 0) {
        data_ov016_021de5c8 = 1;
        DWCi_SNDlPlay(0x10);
    } else {
        data_ov016_021de5c8 = 2;
        DWCi_SNDlPlay(0x12);
    }
    DWCi_MOVlChangeCallback(0);
    DWCi_ChangeScene((u32)endScene_sceneOptMovWait);
}
