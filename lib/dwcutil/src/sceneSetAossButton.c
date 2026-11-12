// libdwcutil's sceneSetAossButton.c (Black's ov016 0x021c48e4-0x021c4ec8): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

typedef struct Ov016Unk21de5cc {
    u8 unk_0;
    u16 unk_2;
    s32 *unk_4;
} Ov016Unk21de5cc;

Ov016Unk21de5cc data_ov016_021de5cc;
void initGraphics_sceneSetAossButton(void);
void procInit_sceneSetAossButton(void);
void procInit1_sceneSetAossButton(void);
void procInit2_sceneSetAossButton(void);
void procCancel(void);
void endScene1_sceneSetAossButton(void);
void endScene2_sceneSetAossButton(void);
void procError1_sceneSetAossButton(void);
void taskVBlank_sceneSetAossButton(void);
void procScene_sceneSetAossButton(void);
BOOL DWCi_IsFold_sceneSetAossButton(void);
void DWCi_SceneSetAossButton(void);
void input_sceneSetAossButton(void);
void draw_sceneSetAossButton(void);
void button_sceneSetAossButton(void);
void endScene_sceneSetAossButton(void);
void procError_sceneSetAossButton(void);
void procWait_sceneSetAossButton(void);
s32 DWCi_AOSSlStart(void);

BOOL DWCi_IsFold_sceneSetAossButton(void) {
    return (u32)((s32)(*(u16 *)0x02ffffa8 & 0x8000) >> 0xf != 0);
}

void DWCi_SceneSetAossButton(void) {
    data_ov016_021de5cc.unk_2 = 0;
    initGraphics_sceneSetAossButton();
    DWCi_HLlLoadSet();
    DWCi_COMMENTlDispEx(0x36, -1, 0);
    DWCi_CMNlStep(2);
    DWCi_CMNlMsgDraw(0x22);
    DWCi_ANIMElInitEx(0);
    DWCi_AOSSlInit();
    DWCi_SNDlPlay(0xb);
    DWCi_ChangeScene((u32)procInit_sceneSetAossButton);
}

void initGraphics_sceneSetAossButton(void) {
    DWCi_CMNlLoadResource("char/jbBgStep3.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/ybBgStep3.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/xb4Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void procInit_sceneSetAossButton(void) {
    func_ov016_021d55a4(2, 1, 1, 8);
    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(1, 1);
    DWCi_GXlVisible(0, 0x15);
    DWCi_ChangeScene((u32)procInit1_sceneSetAossButton);
}

void procInit1_sceneSetAossButton(void) {
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
    DWCi_ChangeScene((u32)procInit2_sceneSetAossButton);
}

void procInit2_sceneSetAossButton(void) {
    if (DWCi_BTNlGet() == -2) {
        return;
    }
    data_ov016_021de5cc.unk_4 = DWCi_TSKlForm(1, (s32)taskVBlank_sceneSetAossButton, 0, 0x78);
    DWCi_ChangeScene((u32)procScene_sceneSetAossButton);
}

void procScene_sceneSetAossButton(void) {
    s32 state;
    s32 *task;

    input_sceneSetAossButton();
    draw_sceneSetAossButton();
    button_sceneSetAossButton();
    state = DWCi_AOSSlStart();
    switch (state) {
    case 0:
        return;
    case 1:
        task = data_ov016_021de5cc.unk_4;
        if (task != NULL) {
            data_ov016_021de5cc.unk_0 = 1;
            DWCi_TSKlDeleteEx(1, task);
            data_ov016_021de5cc.unk_4 = NULL;
            DWCi_ChangeScene((u32)procWait_sceneSetAossButton);
        }
        return;
    case 2:
        if (data_ov016_021de5cc.unk_4 != NULL) {
            DWCi_SNDlStop();
            DWCi_WINlInitEx(12, 1, 1, -1, 0);
            DWCi_SNDlPlay(9);
            DWCi_BTNlDisable();
            DWCi_TSKlDeleteEx(1, data_ov016_021de5cc.unk_4);
            data_ov016_021de5cc.unk_4 = NULL;
            DWCi_ChangeScene((u32)procError_sceneSetAossButton);
        }
        return;
    default:
        return;
    }
}


void input_sceneSetAossButton(void) {
    BOOL BVar2;

    if (DWCi_IPTlCheckTrigger(2)) {
        func_ov016_021bcb28(0);
        return;
    }
    BVar2 = DWCi_IsFold_sceneSetAossButton();
    if (BVar2 == 0) {
        return;
    }
    func_ov016_021bcb28(0);
}

void draw_sceneSetAossButton(void) {
}

void button_sceneSetAossButton(void) {
    if (DWCi_BTNlGet() != 0) {
        return;
    }
    if (data_ov016_021de5cc.unk_4 == NULL) {
        return;
    }
    DWCi_TSKlDelete(1, (u32)data_ov016_021de5cc.unk_4);
    data_ov016_021de5cc.unk_4 = NULL;
    DWCi_BTNlDisable();
    DWCi_ChangeScene((u32)procCancel);
}

void endScene_sceneSetAossButton(void) {
    DWCi_BTNlDisable();
    if (data_ov016_021de5cc.unk_4 != NULL) {
        DWCi_TSKlDeleteEx(1, data_ov016_021de5cc.unk_4);
    }
    func_ov016_021d5790(8);
    DWCi_ChangeScene((u32)endScene1_sceneSetAossButton);
}

void endScene1_sceneSetAossButton(void) {
    if (DWCi_EFFlCheck(1) != 0) {
        return;
    }
    DWCi_BTNlEnd();
    if (data_ov016_021de5cc.unk_0 == 0) {
        func_ov016_021d55a4(3, 1, 1, 8);
    }
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene2_sceneSetAossButton);
}

void endScene2_sceneSetAossButton(void) {
    s32 flag;

    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    if ((data_ov016_021de5cc.unk_0 == 0) && (DWCi_EFFlCheck(1) != 0)) {
        return;
    }
    if (DWCi_BTNlIsEnd() == 0) {
        return;
    }
    if (data_ov016_021de5cc.unk_0 != 0) {
        flag = 1;
    } else {
        flag = 0;
    }
    DWCi_AOSSlEnd(flag);
    DWCi_ANIMElEnd();
    DWCi_FNTlDeleteBgLcd(0);
    if (data_ov016_021de5cc.unk_0 == 0) {
        DWCi_COMMENTlDelete();
        DWCi_GXlInvisible(1, 1U);
    }
    DWCi_GXlInvisible(0, 0x15U);
    if (data_ov016_021de5cc.unk_0 == 0) {
        DWCi_SetParam_(2U, 1U);
        DWCi_ChangeScene((u32) DWCi_SceneSetSelectWay);
        return;
    }
    DWCi_SetParam_(0U, 0U);
    DWCi_ChangeScene((u32) DWCi_SceneSetAossComplete);
}

void taskVBlank_sceneSetAossButton(void) {
    DWCi_IPTlRead();
    DWCi_TSKlAct(0);
    input_sceneSetAossButton();
    button_sceneSetAossButton();
}

void procError_sceneSetAossButton(void) {
    if (DWCi_WINlGet() != 0) {
        return;
    }
    DWCi_SNDlPlay(6);
    DWCi_WINlEnd();
    DWCi_ChangeScene((u32)procError1_sceneSetAossButton);
}

void procError1_sceneSetAossButton(void) {
    BOOL BVar1;

    BVar1 = DWCi_WINlIsInit();
    if (BVar1 != 0) {
        return;
    }
    DWCi_ChangeScene((u32)endScene_sceneSetAossButton);
}

void procWait_sceneSetAossButton(void) {
    input_sceneSetAossButton();
    draw_sceneSetAossButton();
    button_sceneSetAossButton();
    data_ov016_021de5cc.unk_2 = data_ov016_021de5cc.unk_2 + 1;
    if (data_ov016_021de5cc.unk_2 < 0x438) {
        return;
    }
    DWCi_SNDlStop();
    DWCi_ChangeScene((u32)endScene_sceneSetAossButton);
}

void procCancel(void) {
    DWCi_SNDlStop();
    DWCi_SNDlPlay(7);
    data_ov016_021de5cc.unk_0 = 0;
    DWCi_ChangeScene((u32)endScene_sceneSetAossButton);
}
