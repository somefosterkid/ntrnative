// libdwcutil's sceneMenu.c (Black's ov016 0x021c1984-0x021c1f1c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneMenu(void);
void func_ov016_021c1ae8(void);
static u8 data_ov016_021de59c[8];
void func_ov016_021c1b50(void);
void procInit2_sceneMenu(void);
void procScene_sceneMenu(void);
void input_sceneMenu(void);
void draw_sceneMenu(void);
void button_sceneMenu(void);
void dispPushButton_sceneMenu(void);
void endScene_sceneMenu(void);
void endScene1_sceneMenu(void);
void endScene2_sceneMenu(void);
void moveFocus_sceneMenu(s32 param_1);

const u8 data_ov016_021da488[16] = {0x06,0x00,0x1e,0x00,0x9e,0x00,0x92,0x00,0xb2,0x00,0x1e,0x00,0xea,0x00,0x92,0x00};
const u8 data_ov016_021da478[16] = {0x08,0x00,0x20,0x00,0xac,0x00,0xa0,0x00,0xb4,0x00,0x20,0x00,0xf8,0x00,0xa0,0x00};

void DWCi_SceneMenu(void) {
    s32 val;

    initGraphics_sceneMenu();
    DWCi_HLlLoad(0);
    DWCi_COMMENTlDispEx(0x2e, -1, 0);
    DWCi_STEPlChangeScr(4);
    val = (u32)data_ov016_021de59c[0] * 8;
    DWCi_FOCUSlDisp4((u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + val),
                                            (u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + 4 + val),
                                            (u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + 2 + val),
                                            (u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + 6 + val));
    DWCi_ChangeScene((u32)func_ov016_021c1ae8);
}

void initGraphics_sceneMenu(void) {
    char name[] = "char/ybBgStep11.ncl.l";

    DWCi_CMNlLoadResource((u8 *)"char/jbBgStep1.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource((u8 *)"char/jbBgStep1.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource((u8 *)"char/jb2Menu.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u32 **)((u8 *)((void *)data_ov016_021de59c) + 4) = DWCi_ARClReadEx((s8 *) DWCi_Char(name), NULL, 4);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void func_ov016_021c1ae8(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x14, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x14);
    DWCi_ChangeScene((u32)func_ov016_021c1b50);
}

void func_ov016_021c1b50(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_CMNlButton(0);
    DWCi_ChangeScene((u32)procInit2_sceneMenu);
}

void procInit2_sceneMenu(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneMenu);
}

void procScene_sceneMenu(void) {
    input_sceneMenu();
    draw_sceneMenu();
    button_sceneMenu();
}

void input_sceneMenu(void) {
    s32 val;
    BOOL BVar2;
    u32 i;

    i = 0;
    do {
        BVar2 = DWCi_TPlCheckTrigger((u16 *)((void *)(u8 *)data_ov016_021da478 + i * 8));
        if (BVar2 != 0) {
            func_ov016_021bcb28(1);
                        val = (i & 0xff) * (s64)8;
            data_ov016_021de59c[0] = (u8)i;
            DWCi_FOCUSlDisp4((u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + val),
                    (u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + 4 + val),
                    (u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + 2 + val),
                    (u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + 6 + val));
            return;
        }
        i = i + 1;
    } while (i < 2);
    BVar2 = DWCi_IPTlCheckTrigger(1);
    if (BVar2 != 0) {
        func_ov016_021bcb28(1);
        return;
    }
    BVar2 = DWCi_IPTlCheckTrigger(2);
    if (BVar2 != 0) {
        func_ov016_021bcb28(0);
        return;
    }
    BVar2 = DWCi_IPTlCheckRepeat(0x40);
    if (BVar2 != 0) {
        moveFocus_sceneMenu(1);
        return;
    }
    BVar2 = DWCi_IPTlCheckRepeat(0x80);
    if (BVar2 != 0) {
        moveFocus_sceneMenu(3);
        return;
    }
    BVar2 = DWCi_IPTlCheckRepeat(0x20);
    if (BVar2 != 0) {
        moveFocus_sceneMenu(0);
        return;
    }
    BVar2 = DWCi_IPTlCheckRepeat(0x10);
    if (BVar2 == 0) {
        return;
    }
    moveFocus_sceneMenu(2);
}

void draw_sceneMenu(void) {
}

void button_sceneMenu(void) {
    switch (DWCi_BTNlGet()) {
    case 0:
        DWCi_SNDlPlay(7);
        DWCi_ChangeScene((u32)DWCi_SceneEnd);
        return;
    case 1:
        DWCi_SNDlPlay(6);
        dispPushButton_sceneMenu();
        DWCi_ChangeScene((u32)endScene_sceneMenu);
        return;
    default:
        return;
    }
}

void endScene_sceneMenu(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneMenu);
}

void endScene1_sceneMenu(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x16, 8);
    DWCi_ChangeScene((u32)endScene2_sceneMenu);
}

void endScene2_sceneMenu(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_FOCUSlDelete();
    DWCi_COMMENTlDelete();
    DWCi_ARClRelease(*(u32 *)((u8 *)((void *)data_ov016_021de59c) + 4));
    DWCi_GXlInvisible(1, 1U);
    DWCi_GXlInvisible(0, 0x14U);
    switch (*(u8 *)((void *)data_ov016_021de59c)) {
    case 0:
        DWCi_SetParam_(1U, 0U);
        DWCi_ChangeScene((u32) DWCi_SceneSetSelectAp);
        return;
    case 1:
        DWCi_SetParam_(0U, 0U);
        DWCi_ChangeScene((u32) DWCi_SceneOptMenu);
        return;
    }
}

void moveFocus_sceneMenu(s32 dir) {
    s32 val;

    if (dir == 1 || dir == 3) {
        return;
    }
    data_ov016_021de59c[0] = data_ov016_021de59c[0] ^ 1;
    DWCi_SNDlPlay(8);
    val = (u32)data_ov016_021de59c[0] * 8;
    DWCi_FOCUSlDisp4((u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + val),
                                            (u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + 4 + val),
                                            (u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + 2 + val),
                                            (u32)*(u16 *)((void *)(u8 *)data_ov016_021da488 + 6 + val));
}

void dispPushButton_sceneMenu(void) {
    u8 PALETTE[2] = {1, 2};

    DWCi_SCREENlCopyPalette(*(s32 *)(data_ov016_021de59c + 4), PALETTE[data_ov016_021de59c[0]], PALETTE[data_ov016_021de59c[0]]);
}
