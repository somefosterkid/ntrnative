// libdwcutil's common.c (Black's ov016 0x021bd170-0x021bd3e4): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

void DWCi_BTNlInit(s32 param_1);
static const u8 sLangChar[7] = { 'j', 'e', 'f', 'g', 'i', 's', 'k' };
static const u32 sFontFlags[7] = { 0x480, 0x280, 0x280, 0x280, 0x280, 0x280, 0x280 };
static const u16 sNamePos[7][2] = { { 0x6b, 0x22 }, { 0x6c, 0x22 }, { 0x7c, 0x22 }, { 0x5d, 0x22 }, { 0x5f, 0x22 }, { 0x7d, 0x22 }, { 0x50, 0x22 } };
static u8 sPathBuf[0x40];

u8 *DWCi_Char(s8 *name) {
    u8 result;

    strncpy(sPathBuf, name, 0x3FU);
    if (*(u8 *)((u8 *)name + 5) == 0x78) {
        return (u8 *) sPathBuf;
    }
    result = DWCi_Language();
    if ((*(u8 *)((u8 *)name + 5) == 0x79) && (result != 0)) {
        return (u8 *) sPathBuf;
    }
    *(u8 *)((u8 *)sPathBuf + 5) = sLangChar[result];
    return (u8 *) sPathBuf;
}

void DWCi_CMNlLoadResource(s8 *file, u8 *func) {
    u32 *addr;
    u32 uStack_10;

    addr = DWCi_ARClReadEx(DWCi_Char(file), &uStack_10, 4);
    DC_FlushRange(addr, uStack_10);
    (*(void (*)())func)(addr, 0, uStack_10);
    DWCi_ARClRelease((u32)addr);
}

void DWCi_CMNlStep(s32 step) {
    u32 auStack_10 [1];

    DWCi_GetParam_(auStack_10, (u32 *)0x0);
    if (auStack_10[0] == 1) {
        DWCi_STEPlBegin(step);
        return;
    }
    if (auStack_10[0] == 2) {
        DWCi_STEPlChange(step);
        return;
    }
}

void DWCi_CMNlButton(s32 kind) {
    u32 auStack_10 [1];

    DWCi_GetParam_((u32 *)0x0, auStack_10);
    if (auStack_10[0] == 1) {
        DWCi_BTNlInit(kind);
        return;
    }
}

void DWCi_CMNlMsgDraw(u32 msgId) {
    NNSG2dCharCanvas *pNVar1;
    void *ptr;

    pNVar1 = DWCi_FNTlFormBgEx(0, 0);
    ptr = (void *)DWCi_MSGlGet(data_ov016_021de598, msgId);
    DWCi_FNTlDrawRect(pNVar1, 0xd, 0x28, 0xe6, 0x70, 2, DWCi_CMNlFontFlag(), ptr);
    DWCi_FNTlRenewBg((s32)pNVar1);
}

void DWCi_CMNlMsgNameDraw(void *name, u32 msgId) {
    NNSG2dCharCanvas *pNVar3;
    void *ptr;

    pNVar3 = DWCi_FNTlFormBgEx(0, 0);
    ptr = (void *)DWCi_MSGlGet(data_ov016_021de598, msgId);
    DWCi_FNTlDrawRect(pNVar3, 0xd, 0x3c, 0xe6, 0x5e, 2, DWCi_CMNlFontFlag(), ptr);
    DWCi_FNTlDraw((s32)pNVar3, sNamePos[DWCi_Language()][0],
                                            sNamePos[DWCi_Language()][1], 2, 0x209, name);
    DWCi_FNTlRenewBg((s32)pNVar3);
}

u32 DWCi_CMNlFontFlag(void) {
    return sFontFlags[DWCi_Language()];
}
