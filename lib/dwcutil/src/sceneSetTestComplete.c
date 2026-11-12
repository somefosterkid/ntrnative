// libdwcutil's sceneSetTestComplete.c (Black's ov016 0x021cd580-0x021cd80c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetTestComplete(void);
void procInit_sceneSetTestComplete(void);
static u8 data_ov016_021de620;
void procInit1_sceneSetTestComplete(void);
void procScene_sceneSetTestComplete(void);
void wait_sceneSetTestComplete(void);
void draw_sceneSetTestComplete(void);
void func_ov016_021cd6f0(void);
void endScene_sceneSetTestComplete(void);

void DWCi_SceneSetTestCompletet(void) {
    data_ov016_021de620 = 0;
    initGraphics_sceneSetTestComplete();
    DWCi_CMNlMsgDraw(0x2d);
    DWCi_ChangeScene((u32)procInit_sceneSetTestComplete);
}

void initGraphics_sceneSetTestComplete(void) {
    DWCi_CMNlLoadResource("char/xb4Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetTestComplete(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetTestComplete);
}

void procInit1_sceneSetTestComplete(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneSetTestComplete);
}

void procScene_sceneSetTestComplete(void) {
    wait_sceneSetTestComplete();
    draw_sceneSetTestComplete();
}

void draw_sceneSetTestComplete(void) {
}

void endScene_sceneSetTestComplete(void) {
    u32 uStack_10;

    DWCi_GetParam2_((u32 *)0x0, &uStack_10);
    if (uStack_10 != 0) {
        func_ov016_021d0580();
    }
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)func_ov016_021cd6f0);
}

void func_ov016_021cd6f0(void) {
    u8 result;
    u32 uStack_10;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if (!DWCi_STEPlIsEnd()) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_COMMENTlDelete();
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x15);
    DWCi_SetParam_(2, 1);
    DWCi_GetParam2_((u32 *)0x0, &uStack_10);
    if (uStack_10 == 0) {
        DWCi_SetParam2_(0, 0);
        DWCi_ChangeScene((u32)DWCi_SceneSetList);
        return;
    }
    DWCi_ChangeScene((u32)DWCi_SceneMenu);
}

void wait_sceneSetTestComplete(void) {
    u32 sp0;
    u32 result;

    if ((u32) ((data_ov016_021de620 = data_ov016_021de620 + 1)) < 0xB4U) {
        return;
    }
    DWCi_GetParam2_(NULL, &sp0);
    result = DWCi_Entry();
    if ((result != 0) && (result == 1) && (sp0 != 0)) {
        DWCi_ChangeScene((u32) DWCi_SceneEnd);
        return;
    }
    DWCi_ChangeScene((u32) endScene_sceneSetTestComplete);
}
