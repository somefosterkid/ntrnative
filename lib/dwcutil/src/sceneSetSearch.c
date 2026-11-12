// libdwcutil's sceneSetSearch.c (Black's ov016 0x021cb780-0x021cbc90): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void initGraphics_sceneSetSearch(void);
void procInit_sceneSetSearch(void);
static u8 data_ov016_021de608;
void procInit1_sceneSetSearch(void);
void procInit2_sceneSetSearch(void);
void procScene_sceneSetSearch(void);
void input_sceneSetSearch(void);
void draw_sceneSetSearch(void);
void button_sceneSetSearch(void);
void isEnd_sceneSetSearch(void);
void procError_sceneSetSearch(void);
void endScene_sceneSetSearch(void);
void endScene1_sceneSetSearch(void);
void endScene2_sceneSetSearch(void);
void procError1_sceneSetSearch(void);

void DWCi_SceneSetSearch(void) {
    ((u16 *)&data_ov016_021de608)[1] = 0;
    data_ov016_021de608 = 0;
    initGraphics_sceneSetSearch();
    DWCi_COMMENTlDispEx(0x33, -1, 0);
    DWCi_HLlLoadSet();
    DWCi_CMNlStep(2);
    DWCi_CMNlMsgDraw(0x33);
    DWCi_ANIMElInitEx(0);
    DWCi_SEARCHlInit();
    DWCi_SEARCHlBegin();
    DWCi_SNDlPlay(10);
    DWCi_ChangeScene((u32)procInit_sceneSetSearch);
}

void initGraphics_sceneSetSearch(void) {
    DWCi_CMNlLoadResource("char/jbBgStep3.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/ybBgStep3.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/xb4Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetSearch(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetSearch);
}

void procInit1_sceneSetSearch(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_CMNlButton(1);
    DWCi_ChangeScene((u32)procInit2_sceneSetSearch);
}

void procInit2_sceneSetSearch(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    DWCi_BTNlEnable();
    DWCi_ChangeScene((u32)procScene_sceneSetSearch);
}

void procScene_sceneSetSearch(void) {
    input_sceneSetSearch();
    draw_sceneSetSearch();
    button_sceneSetSearch();
    isEnd_sceneSetSearch();
}

void isEnd_sceneSetSearch(void) {
    s32 result;
    s32 i;
    s32 iStack_c;

    ((u16 *)&data_ov016_021de608)[1] = ((u16 *)&data_ov016_021de608)[1] + 1;
    if (((u16 *)&data_ov016_021de608)[1] < 300) {
        return;
    }
    DWCi_SNDlStop();
    result = func_ov016_021cf114(&iStack_c);
    if (result == 0) {
        data_ov016_021de608 = 2;
        DWCi_WINlInitEx(0xf, 1, 1, -1, 0);
        DWCi_SNDlPlay(0x12);
        DWCi_BTNlDisable();
        DWCi_ChangeScene((u32)procError_sceneSetSearch);
        return;
    }
    i = 0;
    if (0 < result) {
        do {
            if (*(u8 *)(i * 0x2a + iStack_c + 0x28) != 2) break;
            i = i + 1;
        } while (i < result);
    }
    if (i == result) {
        data_ov016_021de608 = 3;
        DWCi_WINlInitEx(0xe, 1, 1, -1, 0);
        DWCi_SNDlPlay(0x12);
        DWCi_BTNlDisable();
        DWCi_ChangeScene((u32)procError_sceneSetSearch);
        return;
    }
    data_ov016_021de608 = 1;
    DWCi_SNDlPlay(0xf);
    DWCi_ChangeScene((u32)endScene_sceneSetSearch);
}

void input_sceneSetSearch(void) {
    if (!DWCi_IPTlCheckTrigger(2)) {
        return;
    }
    DWCi_SNDlStop();
    func_ov016_021bcb28(0);
}

void draw_sceneSetSearch(void) {
}

void button_sceneSetSearch(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    DWCi_SNDlStop();
    DWCi_SNDlPlay(7);
    DWCi_ChangeScene((u32)endScene_sceneSetSearch);
}

void endScene_sceneSetSearch(void) {
    DWCi_BTNlDisable();
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetSearch);
}

void endScene1_sceneSetSearch(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    func_ov016_021d55a4(3, 1, 1, 8);
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetSearch);
}

void endScene2_sceneSetSearch(void) {
    u8 result;
    BOOL result2;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    result2 = DWCi_BTNlIsEnd();
    if (!result2) {
        return;
    }
    do {
        result2 = DWCi_SEARCHlStop();
    } while (!result2);
    DWCi_ANIMElEnd();
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_COMMENTlDelete();
    DWCi_GXlInvisible(1, 1);
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de608 != 1) {
        DWCi_SEARCHlEnd();
        DWCi_SETTINGlSetSetting((u32)*(u8 *)(DWCi_SETTIGNlGet() + 0xf4));
        DWCi_SetParam_(2, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetSelectWay);
        return;
    }
    DWCi_SetParam_(0, 1);
    DWCi_SetParam2_(0, 0);
    DWCi_ChangeScene((u32)DWCi_SceneSetApList);
}

void procError_sceneSetSearch(void) {
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procError1_sceneSetSearch);
}

void procError1_sceneSetSearch(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetSearch);
}
