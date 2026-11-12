// libdwcutil's sceneOptMovDisconnect.c (Black's ov016 0x021c3b8c-0x021c3d8c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneOptMovDisconnect(void);
void procInit_sceneOptMovDisconnect(void);
static u8 data_ov016_021de5bc;
void procInit1_sceneOptMovDisconnect(void);
void procScene_sceneOptMovDisconnect(void);
void wait_sceneOptMovDisconnect(void);
void draw_sceneOptMovDisconnect(void);
void endScene1_sceneOptMovDisconnect(void);
void endScene_sceneOptMovDisconnect(void);

void DWCi_SceneOptMovDisconnect(void) {
    data_ov016_021de5bc = 0;
    initGraphics_sceneOptMovDisconnect();
    DWCi_CMNlMsgDraw(0x21);
    DWCi_ChangeScene((u32)procInit_sceneOptMovDisconnect);
}

void initGraphics_sceneOptMovDisconnect(void) {
    DWCi_CMNlLoadResource("char/yb5Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneOptMovDisconnect(void) {
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneOptMovDisconnect);
}

void procInit1_sceneOptMovDisconnect(void) {
    BOOL BVar2;

    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    BVar2 = DWCi_MOVlIsEnd();
    if (BVar2 == 0) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneOptMovDisconnect);
}

void procScene_sceneOptMovDisconnect(void) {
    wait_sceneOptMovDisconnect();
    draw_sceneOptMovDisconnect();
}

void draw_sceneOptMovDisconnect(void) {
}

void endScene_sceneOptMovDisconnect(void) {
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene1_sceneOptMovDisconnect);
}

void endScene1_sceneOptMovDisconnect(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_COMMENTlDelete();
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x15);
    DWCi_SetParam_(0, 1);
    DWCi_ChangeScene((u32)DWCi_SceneOptMenu);
}

void wait_sceneOptMovDisconnect(void) {
    data_ov016_021de5bc = data_ov016_021de5bc + 1;
    if (data_ov016_021de5bc < 0x78) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneOptMovDisconnect);
}
