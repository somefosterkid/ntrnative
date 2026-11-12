// libdwcutil's sceneEnd.c (Black's ov016 0x021c1630-0x021c16f4): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void endScene_sceneEnd(void);
void DWCi_MSGlRelease(u32 param_1);

void DWCi_SceneEnd(void) {
    func_ov016_021d55a4(3, 1, 0x3f, 0x14);
    func_ov016_021d55a4(3, 0, 0x3f, 0x14);
    DWCi_ChangeScene((u32)endScene_sceneEnd);
}

void endScene_sceneEnd(void) {
    u8 result;

    result = DWCi_EFFlCheck(1);
    if (result != 0) {
        return;
    }
    result = DWCi_EFFlCheck(0);
    if (result != 0) {
        return;
    }
    DWCi_TSKlOperate(0, 0);
    DWCi_TSKlOperate(1, 0);
    func_ov016_021d52b0(1);
    func_ov016_021d52b0(0);
    DWCi_HLlEnd();
    DWCi_FOCUSlEnd();
    DWCi_MSGlRelease((u32)data_ov016_021de598);
    DWCi_MSGlEnd();
    DWCi_SETTIGNlEnd();
    DWCi_EndUtility();
}
