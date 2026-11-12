// libdwcutil's headline.c (Black's ov016 0x021bd5e8-0x021bd744): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

static s8 *data_ov016_021dae04[20] = {
    "char/jb2HlWiFi.nsc.l",
    "char/jb2HlAp.nsc.l",
    "char/jb3HlList1.nsc.l",
    "char/jb3HlList2.nsc.l",
    "char/jb3HlList3.nsc.l",
    "char/jb4HlSet1.nsc.l",
    "char/jb4HlSet2.nsc.l",
    "char/jb4HlSet3.nsc.l",
    "char/jb4HlUsb.nsc.l",
    "char/jb4HlSsid.nsc.l",
    "char/jb4HlWep.nsc.l",
    "char/jb4HlIp.nsc.l",
    "char/jb4HlMask.nsc.l",
    "char/jb4HlGateway.nsc.l",
    "char/jb4HlDns0.nsc.l",
    "char/jb4HlDns1.nsc.l",
    "char/jb5HlOption.nsc.l",
    "char/jb5HlInfo.nsc.l",
    "char/jb5HlErase.nsc.l",
    "char/jb5HlMove.nsc.l",
};
static void *data_ov016_021de554;
void taskLoad_headline(u32 param_1);
void DWCi_HLlInit(void);
void DWCi_HLlLoadList(void);

void DWCi_HLlInit(void) {
    data_ov016_021de554 = DWCi_HEAPlAllocEx(0xc0, 4);
    DWCi_CMNlLoadResource("char/jbBgHl.ncg.l", (u8 *)GX_LoadBG1Char);
    switch (DWCi_Entry()) {
    case 0:
        DWCi_CMNlLoadResource(data_ov016_021dae04[0], (u8 *)GX_LoadBG1Scr);
        return;
    case 1:
        DWCi_CMNlLoadResource(data_ov016_021dae04[1], (u8 *)GX_LoadBG1Scr);
        return;
    default:
        return;
    }
}

void DWCi_HLlEnd(void) {
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de554);
}

void DWCi_HLlLoad(s32 idx) {
    u32 *src;

    src = DWCi_ARClReadEx(DWCi_Char(*(s8 **)((void *)data_ov016_021dae04 + idx * 4)), (u32 *)0x0, 4);
    MIi_CpuCopyFast(src, data_ov016_021de554, 0xc0);
    DWCi_ARClRelease((u32)src);
    DWCi_TSKlForm(1, (s32)taskLoad_headline, 0, 0x78);
}

void DWCi_HLlLoadSet(void) {
    DWCi_HLlLoad(*(u8 *)(DWCi_SETTIGNlGet() + 0xf4) + 5);
}

void DWCi_HLlLoadList(void) {
    DWCi_HLlLoad(*(u8 *)(DWCi_SETTIGNlGet() + 0xf4) + 2);
}

void taskLoad_headline(u32 task) {
    DC_FlushRange(data_ov016_021de554, 0xc0);
    GX_LoadBG1Scr(data_ov016_021de554, 0, 0xc0);
    DWCi_TSKlDelete(1, task);
}
