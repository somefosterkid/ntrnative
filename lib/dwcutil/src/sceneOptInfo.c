// libdwcutil's sceneOptInfo.c (Black's ov016 0x021c255c-0x021c2a3c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneOptInfo(void);
void drawInfo(void);
void procInit_sceneOptInfo(void);
void procInit1_sceneOptInfo(void);
void procInit2_sceneOptInfo(void);
void procScene_sceneOptInfo(void);
void input_sceneOptInfo(void);
void draw_sceneOptInfo(void);
void button_sceneOptInfo(void);
void endScene_sceneOptInfo(void);
void endScene1_sceneOptInfo(void);
void endScene2_sceneOptInfo(void);

void DWCi_SceneOptInfo(void) {
    initGraphics_sceneOptInfo();
    DWCi_HLlLoad(0x11);
    DWCi_COMMENTlDispEx(0x3a, -1, 0);
    drawInfo();
    DWCi_ChangeScene((u32)procInit_sceneOptInfo);
}

void initGraphics_sceneOptInfo(void) {
    DWCi_CMNlLoadResource("char/jb5Info.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void drawInfo(void) {
    DWCWiFiInfo info;
    u64 id;
    s32 tmp[4];
    s32 i;
    u16 wide[21];
    char buf[20];
    u8 mac[6];
    NNSG2dCharCanvas *canvas;

    canvas = DWCi_FNTlFormBgEx(0, 0);
    OS_GetMacAddress(mac);
    OS_SNPrintf(buf, 20, "%02X-%02X-%02X-%02X-%02X-%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    func_ov016_021d722c(wide, buf, 20);
    DWCi_FNTlDrawRect(canvas, 8, 0x40, 0xf0, 0x1c, 2, 0x480, wide);
    DWCi_BM_GetWiFiInfo(&info);
    id = info.attestedUserId;
    if (id != 0) {
        tmp[3] = (id % 10) * 1000;
        id /= 10;
        for (i = 0; i < 3; i++) {
            tmp[2 - i] = id % 10000;
            id /= 10000;
        }
        OS_SNPrintf(buf, 20, "%04d-%04d-%04d-%04d", tmp[0], tmp[1], tmp[2], tmp[3]);
        func_ov016_021d722c(wide, buf, 20);
    } else {
        OS_SNPrintf(buf, 20, "-------------------");
        func_ov016_021d722c(wide, buf, 20);
    }
    DWCi_FNTlDrawRect(canvas, 8, 0x78, 0xf0, 0x1c, 2, 0x480, wide);
    DWCi_FNTlRenewBg((s32)canvas);
}


void procInit_sceneOptInfo(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneOptInfo);
}

void procInit1_sceneOptInfo(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_CMNlButton(2);
    DWCi_ChangeScene((u32)procInit2_sceneOptInfo);
}

void procInit2_sceneOptInfo(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneOptInfo);
}

void procScene_sceneOptInfo(void) {
    input_sceneOptInfo();
    draw_sceneOptInfo();
    button_sceneOptInfo();
}

void input_sceneOptInfo(void) {
    if (!DWCi_IPTlCheckTrigger(2)) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneOptInfo(void) {
}

void button_sceneOptInfo(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(7);
    DWCi_ChangeScene((u32)endScene_sceneOptInfo);
}

void endScene_sceneOptInfo(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneOptInfo);
}

void endScene1_sceneOptInfo(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneOptInfo);
}

void endScene2_sceneOptInfo(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    if (!DWCi_BTNlIsEnd()) {
        return;
    }
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_COMMENTlDelete();
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x15);
    DWCi_SetParam_(0, 1);
    DWCi_ChangeScene((u32)DWCi_SceneOptMenu);
}
