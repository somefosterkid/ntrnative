// libdwcutil's sceneSetListSave.c (Black's ov016 0x021ca5d0-0x021ca818): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetListSave(void);
void procInit_sceneSetListSave(void);
void procInit1_sceneSetListSave(void);
void procScene_sceneSetListSave(void);
void input_sceneSetListSave(void);
void draw_sceneSetListSave(void);
void endScene_sceneSetListSave(void);
void endScene1_sceneSetListSave(void);
void DWCi_SceneSetListSave(void);

void DWCi_SceneSetListSave(void) {
    initGraphics_sceneSetListSave();
    DWCi_HLlLoadSet();
    DWCi_SETTINGlSet();
    DWCi_ChangeScene((u32)procInit_sceneSetListSave);
}

void initGraphics_sceneSetListSave(void) {
    DWCi_CMNlLoadResource("char/jbBgStep3.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/ybBgStep3.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/xb4None.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetListSave(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x14, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x14);
    DWCi_ChangeScene((u32)procInit1_sceneSetListSave);
}

void procInit1_sceneSetListSave(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_WINlInitEx(0x44, 5, 1, -1, 0);
    DWCi_ChangeScene((u32)procScene_sceneSetListSave);
}

void procScene_sceneSetListSave(void) {
    BOOL BVar1;

    input_sceneSetListSave();
    draw_sceneSetListSave();
    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetListSave);
}

void input_sceneSetListSave(void) {
}

void draw_sceneSetListSave(void) {
}

void endScene_sceneSetListSave(void) {
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x14, 8);
    DWCi_ChangeScene((u32)endScene1_sceneSetListSave);
}

void endScene1_sceneSetListSave(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_COMMENTlDelete();
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x14);
    DWCi_SetParam_(2, 1);
    DWCi_ChangeScene((u32)DWCi_SceneSetSelectAp);
}
