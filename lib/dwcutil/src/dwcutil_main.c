// libdwcutil's main.c (Black's ov016 0x021c0970-0x021c0f60): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order. Named
// dwcutil_main.c here: dsd takes each file name once, and src/system/main.c has main.c.
#include "dwcutil.h"

static u8 data_ov016_021de56c[32];
u32 func_ov016_021c0a2c(s32 param_1, u32 param_2);
u32 func_ov009_02177ed8(void);
void DWC_Auth_SetCustomNas(u32 param_1);
void initGame(void);
void func_ov016_021c0b7c(void);
void DWCi_SNDlInit(void);
void DWCi_SceneInit(void);
void procEnd(void);
void func_ov016_021d6ce4(void);
void DWCi_CELLlEnd(void);
void DWCi_TSKlEnd(void);
void func_ov016_021d6c78(void);
void DWCi_TSKlInit(void);
void DWCi_ARClInit(void);
u32 DWC_BM_Init(void *param_1);
void func_ov016_021d5824(void);
void DWCi_OVRlInit(void);
int DWC_StartUtility(void *work, int language, int param);

int DWC_StartUtility(void *work, int language, int param) {
    BOOL BVar1;
    u32 nasaddr;
    BVar1 = OS_IsRunOnTwl();
    if (BVar1 != 0) {
        return -2;
    }
    ((void **)data_ov016_021de56c)[1] = work;
    if (func_ov016_021c0a2c(language, param) == 0) {
        return -1;
    }
    nasaddr = func_ov009_02177ed8();
    DWC_Auth_SetCustomNas((u32)"https://nas.nintendowifi.net/ac");
    ((u8 *)data_ov016_021de56c)[1] = 0;
    initGame();
    func_ov016_021c0b7c();
    DWCi_SNDlInit();
    DWCi_ChangeScene((u32)DWCi_SceneInit);
    do {
        DWCi_IPTlRead();
        (*((void (**)())data_ov016_021de56c)[3])();
        DWCi_TSKlAct(0);
        func_ov016_021d6bc8();
        DWCi_SetLedWireless();
        OS_WaitVBlankIntr();
    } while (((u8 *)data_ov016_021de56c)[1] == 0);
    procEnd();
    DWC_Auth_SetCustomNas(nasaddr);
    return 0;
}

u32 func_ov016_021c0a2c(s32 lang, u32 param) {
    u32 val;
    int val2;

    *(u8 *)((void *)data_ov016_021de56c) = (u8) lang;
    *(u32 *)((u8 *)((void *)data_ov016_021de56c) + 8) = param;
    if ((lang < 0) || (lang > 6)) {
        return 0U;
    }
    val2 = param << 0x1C;
    if ((u32)((u32)val2 >> 0x1C) > 1U) {
        return 0U;
    }
    val = param >> 4;
    if (val & 2) {
        return 0U;
    }
    if ((lang != 0) && (val & 1)) {
        return 0U;
    }
    if ((lang == 0) && !(val & 1)) {
        return 0U;
    }
    return 1U;
}

void initGame(void) {
    void *work;

    OS_DisableIrq();
    GX_DispOff();
    reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xfffeffff;
    if (!OS_IsTickAvailable()) {
        OS_Terminate();
    }
    if (!OS_IsAlarmAvailable()) {
        OS_Terminate();
    }
    GX_VBlankIntr(0);
    FX_Init();
    FS_Init(0xffffffff);
    TP_Init();
    RTC_Init();
    GX_DispOff();
    reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xfffeffff;
    func_ov016_021d7274();
    DWCi_HEAPlInit(((void **)data_ov016_021de56c)[1]);
    func_ov016_021d6c78();
    DWCi_TSKlInit();
    DWCi_ARClInit();
    DWCi_IPTlInit();
    func_ov016_021d551c();
    work = DWCi_HEAPlAllocEx(0x1000, 0x20);
    DWC_BM_Init(work);
    DWCi_HEAPlFree_((u32 *)&work);
}

void func_ov016_021c0b7c(void) {
    GX_VBlankIntr(0);
    GX_SetBankForBG(1);
    GX_SetBankForOBJ(2);
    GX_SetGraphicsMode(1, 0, 0);
    GX_SetVisiblePlane(0);
    GX_SetVisibleWnd(0);
    GX_SetMasterBrightness(0);
    GX_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_128K);
    G2_BG0Mosaic(FALSE);
    G2_BG1Mosaic(FALSE);
    G2_BG2Mosaic(FALSE);
    G2_BG3Mosaic(FALSE);
    G2_SetBG0Offset(0, 0);
    G2_SetBG1Offset(0, 0);
    G2_SetBG2Offset(0, 0);
    G2_SetBG3Offset(0, 0);
    G2_SetBlendBrightness(0x3f, 0x10);
    GX_SetBankForSubBG(0x80);
    GX_SetBankForSubOBJ(0x100);
    GXS_SetGraphicsMode(0);
    GXS_SetVisiblePlane(0);
    GXS_SetVisibleWnd(0);
    GXS_SetMasterBrightness(0);
    GXS_SetOBJVRamModeChar(GX_OBJVRAMMODE_CHAR_1D_32K);
    G2S_BG0Mosaic(FALSE);
    G2S_BG1Mosaic(FALSE);
    G2S_BG2Mosaic(FALSE);
    G2S_BG3Mosaic(FALSE);
    G2S_SetBG0Offset(0, 0);
    G2S_SetBG1Offset(0, 0);
    G2S_SetBG2Offset(0, 0);
    G2S_SetBG3Offset(0, 0);
    G2S_SetBlendBrightness(0x3f, 0x10);
    G2_SetBG0Control(0, 0, 12, 0, 0);
    G2_SetBG1Control(0, 0, 13, 2, 0);
    G2_SetBG2ControlText(0, 0, 14, 4);
    G2_SetBG3ControlText(0, 0, 15, 4);
    G2S_SetBG0Control(0, 0, 12, 0, 0);
    G2S_SetBG1Control(0, 0, 13, 0, 0);
    G2S_SetBG2ControlText(0, 0, 14, 0);
    G2S_SetBG3ControlText(0, 0, 15, 0);
    GX_SetBGScrOffset(0);
    GX_SetBGCharOffset(0);
    GX_SetDispSelect(GX_DISP_SELECT_SUB_MAIN);
    func_ov016_021d5824();
    DWCi_OBJlInit();
    DWCi_OVRlInit();
    DWCi_CELLlInit();
    GX_DispOn();
    GXS_DispOn();
    GX_VBlankIntr(1);
}

void procEnd(void) {
    GX_DispOff();
    reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xfffeffff;
    DWCi_SetLedNormal();
    PM_SetLCDPower(1);
    DWCi_IPTlEnd();
    func_ov016_021d6ce4();
    DWCi_SNDlEnd();
    DWCi_CELLlEnd();
    DWCi_OBJlEnd();
    DWCi_FNTlEnd();
    DWCi_EFFlEnd();
    func_ov016_021d4a20();
    DWCi_TSKlEnd();
    DWCi_HEAPlEnd();
    DWCi_RepairVram();
}

void DWCi_ChangeScene(u32 scene) {
    ((u32 *)data_ov016_021de56c)[3] = scene;
}

void DWCi_SetParam_(u32 param0, u32 param1) {
    ((u32 *)data_ov016_021de56c)[4] = param0;
    ((u32 *)data_ov016_021de56c)[5] = param1;
}

void DWCi_GetParam_(u32 *param0, u32 *param1) {
    if (param0 != NULL) {
        *param0 = ((u32 *)data_ov016_021de56c)[4];
    }
    if (param1 != NULL) {
        *param1 = ((u32 *)data_ov016_021de56c)[5];
    }
}

void DWCi_SetParam2_(u32 param2, u32 param3) {
    ((u32 *)data_ov016_021de56c)[6] = param2;
    ((u32 *)data_ov016_021de56c)[7] = param3;
}

void DWCi_GetParam2_(u32 *param2, u32 *param3) {
    if (param2 != NULL) {
        *param2 = ((u32 *)data_ov016_021de56c)[6];
    }
    if (param3 != NULL) {
        *param3 = ((u32 *)data_ov016_021de56c)[7];
    }
}

u8 DWCi_Language(void) {
    return data_ov016_021de56c[0];
}

u32 DWCi_Entry(void) {
    return (u32) (*(s32 *)((u8 *)((void *)data_ov016_021de56c) + 8) << 0x1C) >> 0x1C;
}

BOOL DWCi_Flag(u32 flag) {
    return (u32)((flag & ((u32 *)data_ov016_021de56c)[2] >> 4) != 0);
}

void DWCi_EndUtility(void) {
    ((u8 *)data_ov016_021de56c)[1] = 1;
}

u8 kDWCi_Language(void) {
    if (data_ov016_021de56c[0] == 6) {
        return 0x38;
    }
    if ((data_ov016_021de56c[0] == 1) && ((((u32 *)data_ov016_021de56c)[2] >> 4 & 2) == 0)) {
        return 0x37;
    }
    return data_ov016_021de56c[0] + 0x31;
}
