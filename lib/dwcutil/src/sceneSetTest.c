// libdwcutil's sceneSetTest.c (Black's ov016 0x021cd160-0x021cd580): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void acAlloc(u32 param_1, u32 param_2);
void acFree(u32 param_1, void *param_2);
static const DWCAcParam data_ov016_021da898 = { (void *)acAlloc, (void *)acFree, 3, 1, 0, 0, 0, 0, 0, { 0, 0, 0 } };
void initGraphics_sceneSetTest(void);
void initAc(void);
void procInit_sceneSetTest(void);
static u8 data_ov016_021de61c;
void procInit1_sceneSetTest(void);
void procScene_sceneSetTest(void);
void input_sceneSetTest(void);
void draw_sceneSetTest(void);
void button_sceneSetTest(void);
void endScene1_sceneSetTest(void);
void endScene2_sceneSetTest(void);
BOOL DWC_AC_Destroy(void);
u8 *DWCi_BACKUPlGetWifi(void);
void taskAc(u32);
void endScene_sceneSetTest(void);

void DWCi_SceneSetTest(void) {
    u32 auStack_10 [1];

    data_ov016_021de61c = 0;
    initGraphics_sceneSetTest();
    DWCi_GetParam2_((u32 *)0x0, auStack_10);
    if (auStack_10[0] == 0) {
        DWCi_COMMENTlDispEx(0x31, -1, 0);
    }
    DWCi_CMNlStep(2);
    if (auStack_10[0] == 0) {
        DWCi_HLlLoadSet();
    }
    DWCi_CMNlMsgDraw(0x2c);
    DWCi_ANIMElInitEx(0);
    initAc();
    DWCi_ICONlInit(0);
    DWCi_SNDlPlay(0xc);
    DWCi_ChangeScene((u32)procInit_sceneSetTest);
}

void initGraphics_sceneSetTest(void) {
    DWCi_CMNlLoadResource("char/jbBgStep3.ncg.l", (u8 *) GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/ybBgStep3.ncl.l", (u8 *) GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/xb4Multi.nsc.l", (u8 *) GX_LoadBG2Scr);
    *(u16 *)((void *)0x04001008) = (u16) ((*(u16 *)((void *)0x04001008) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x04001008) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x04001008) + 2) & ~3) | 3);
    *(u16 *)0x04000008 = (*(u16 *)0x04000008 & ~3) | 3;
    *(u16 *)((void *)0x0400000A) = (u16) ((*(u16 *)((void *)0x0400000A) & ~3) | 3);
    *(u16 *)((u8 *)((void *)0x0400000A) + 2) = (u16) ((*(u16 *)((u8 *)((void *)0x0400000A) + 2) & ~3) | 3);
}

void initAc(void)
{
    DWCAcParam param;
    u32 state;
    void *setting;

    setting = (void *)DWCi_SETTIGNlGet();
    MIi_CpuCopy32(&data_ov016_021da898, &param, 0x14);
    DWCi_GetParam2_(NULL, &state);
    if (state == 2) {
        param.mode = 4;
    } else {
        param.mode = *(u8 *)((u8 *)setting + 0xf4) + 1;
    }
    if (func_0205b98c(&param) == 0) {
        OS_Terminate();
    }
    if (state == 0) {
        DWCi_AC_InsertApInfo(*(u8 *)((u8 *)setting + 0xf4), setting);
    }
    DWCi_TSKlForm(0, (s32)taskAc, 0, 0x78);
}

void procInit_sceneSetTest(void) {
    u32 uStack_18;

    func_ov016_021d55a4(2, 0, 0x15, 8);
    DWCi_GXlVisible(0, 0x15);
    DWCi_GetParam2_((u32 *)0x0, &uStack_18);
    if (uStack_18 == 0) {
        func_ov016_021d55a4(2, 1, 1, 8);
        DWCi_GXlVisible(1, 1);
    }
    DWCi_ChangeScene((u32)procInit1_sceneSetTest);
}

void procInit1_sceneSetTest(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_ChangeScene((u32)procScene_sceneSetTest);
}

void procScene_sceneSetTest(void) {
    input_sceneSetTest();
    draw_sceneSetTest();
    button_sceneSetTest();
}

void input_sceneSetTest(void) {
}

void draw_sceneSetTest(void) {
}

void button_sceneSetTest(void) {
}

void endScene_sceneSetTest(void) {
    func_ov016_021d55a4(3, 0, 0x15, 8);
    DWCi_ChangeScene((u32)endScene1_sceneSetTest);
}

void endScene1_sceneSetTest(void) {
    if (DWCi_EFFlCheck(0) != 0) {
        return;
    }
    DWCi_ChangeScene((u32)endScene2_sceneSetTest);
}

void endScene2_sceneSetTest(void) {
    u32 result;
    u8 *src;

    result = DWCi_SETTINGlGetMemMap();
    if (DWC_AC_Destroy() == 0) {
        return;
    }
    src = DWCi_BACKUPlGetWifi();
    MI_CpuCopy8(src, (void *)(result + 0xf0), 0xe);
    src = DWCi_BACKUPlGetWifi();
    MI_CpuCopy8(src, (void *)(result + 0x1f0), 0xe);
    DWCi_ANIMElEnd();
    DWCi_FNTlDeleteBgLcd(0);
    DWCi_GXlInvisible(0, 0x15);
    if (data_ov016_021de61c == 0) {
        DWCi_SetParam_(0, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetError);
        return;
    }
    DWCi_SetParam_(0, 1);
    DWCi_ChangeScene((u32)DWCi_SceneSetTestCompletet);
}

void taskAc(u32 task) {
    s32 rs;

    rs = DWC_AC_Process();
    if (rs == 0) {
        return;
    }
    DWCi_ICONlEnd();
    DWCi_SNDlStop();
    if (rs > 0) {
        data_ov016_021de61c = 1;
        DWCi_SNDlPlay(0x11);
    } else {
        DWCi_SetErrorCode(DWC_AC_GetStatus());
        DWCi_SNDlPlay(0x12);
    }
    DWCi_ChangeScene((u32)endScene_sceneSetTest);
    DWCi_TSKlDelete(0, task);
}

void acAlloc(u32 param_1, u32 param_2) {
    DWCi_HEAPlAllocEx(param_2, 0x20);
}

void acFree(u32 param_1, void *param_2) {
    DWCi_HEAPlFree2(param_2);
}
