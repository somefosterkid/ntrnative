// libdwcutil's sceneInit.c (Black's ov016 0x021c16f4-0x021c1984): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void procWait_sceneInit(void);
void procFade(void);

s32 *data_ov016_021de598;

void DWCi_SceneInit(void) {
static const char * msgFiles[] = {
        "msg/jap.bmg.l", "msg/eng.bmg.l", "msg/fre.bmg.l", "msg/ger.bmg.l",
        "msg/ita.bmg.l", "msg/spa.bmg.l", "msg/kor.bmg.l"
    };
    u8 *n;

    DWCi_SETTIGNlInit();
    DWCi_MSGlInit();
    DWCi_FOCUSlInit();
    DWCi_HLlInit();
    DWCi_COMMENTlInit();
    if (DWCi_Language() == 1 && DWCi_Flag(2)) {
        data_ov016_021de598 = DWCi_MSGlRead((u8 *)"msg/usa.bmg.l");
    } else {
        data_ov016_021de598 = DWCi_MSGlRead((u8 *)msgFiles[DWCi_Language()]);
    }
    n = DWCi_Char("char/jtMain.nce.l");
    DWCi_CEINlRead(1, (s8 *)n);
    n = DWCi_Char("char/jbMain.nce.l");
    DWCi_CEINlRead(0, (s8 *)n);
    DWCi_CMNlLoadResource("char/jtBgMain.ncg.l", (u8 *)GXS_LoadBG1Char);
    DWCi_CMNlLoadResource("char/jtBgMain.ncl.l", (u8 *)GXS_LoadBGPltt);
    DWCi_CMNlLoadResource("char/jtObjMain.ncg.l", (u8 *)GXS_LoadOBJ);
    DWCi_CMNlLoadResource("char/xtObjMain.ncl.l", (u8 *)GXS_LoadOBJPltt);
    DWCi_CMNlLoadResource("char/jbBgStep1.ncg.l", (u8 *)GX_LoadBG2Char);
    DWCi_CMNlLoadResource("char/jbBgStep1.ncl.l", (u8 *)GX_LoadBGPltt);
    DWCi_CMNlLoadResource("char/jbObjMain.ncg.l", (u8 *)GX_LoadOBJ);
    DWCi_CMNlLoadResource("char/ybObjMain.ncl.l", (u8 *)GX_LoadOBJPltt);
    switch (DWCi_Entry()) {
    case 0:
        DWCi_CMNlLoadResource("char/jtTop.nsc.l", (u8 *)GXS_LoadBG1Scr);
        break;
    case 1:
        DWCi_CMNlLoadResource("char/jtStep1.nsc.l", (u8 *)GXS_LoadBG1Scr);
        break;
    }
    reg_G2S_DB_BG1CNT = (reg_G2S_DB_BG1CNT & ~3) | 3;
    reg_G2_BG1CNT = (reg_G2_BG1CNT & ~3) | 3;
    reg_G2_BG1CNT = (reg_G2_BG1CNT & ~3) | 3;
    DWCi_GXlVisible(1, 2);
    DWCi_GXlVisible(0, 2);
    DWCi_ChangeScene((u32)procFade);
}

void procFade(void) {
    func_ov016_021d55a4(2, 1, 2, 0x14);
    func_ov016_021d55a4(2, 0, 2, 0x14);
    DWCi_ChangeScene((u32)procWait_sceneInit);
}

void procWait_sceneInit(void) {
    u8 result;
    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    switch (DWCi_Entry()) {
    case 0:
        DWCi_SetParam_(0, 1);
        DWCi_ChangeScene((u32)DWCi_SceneMenu);
        return;
    case 1:
        DWCi_SetParam_(1, 1);
        DWCi_ChangeScene((u32)DWCi_SceneSetSelectAp);
        return;
    default:
        return;
    }
}
