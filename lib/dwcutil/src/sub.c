// libdwcutil's sub.c (Black's ov016 0x021d7274-0x021d7484): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

u32 func_0208c6a4(u32 arg);
static s32 data_ov016_021de6b0[13];
GXVRamBG GX_DisableBankForBG(void);
GXVRamOBJ GX_DisableBankForOBJ(void);
void func_ov016_021d73bc(void);
u32 GX_DisableBankForBGExtPltt(void);
u32 GX_DisableBankForOBJExtPltt(void);
u32 GX_DisableBankForTex(void);
u32 GX_DisableBankForTexPltt(void);
u32 GX_DisableBankForClearImage(void);
u32 GX_DisableBankForSubBGExtPltt(void);
u32 GX_DisableBankForSubOBJExtPltt(void);
u32 GX_DisableBankForARM7(void);
void GX_SetBankForARM7(u32 banks);
u32 func_0208c6c0(u32 *out);

void func_ov016_021d7274(void)
{

    ((s32 *)data_ov016_021de6b0)[0] = GX_DisableBankForBG();
    ((s32 *)data_ov016_021de6b0)[1] = GX_DisableBankForOBJ();
    ((s32 *)data_ov016_021de6b0)[2] = GX_DisableBankForBGExtPltt();
    ((s32 *)data_ov016_021de6b0)[3] = GX_DisableBankForOBJExtPltt();
    ((s32 *)data_ov016_021de6b0)[4] = GX_DisableBankForTex();
    ((s32 *)data_ov016_021de6b0)[5] = GX_DisableBankForTexPltt();
    ((s32 *)data_ov016_021de6b0)[6] = GX_DisableBankForClearImage();
    ((s32 *)data_ov016_021de6b0)[7] = GX_DisableBankForSubBG();
    ((s32 *)data_ov016_021de6b0)[8] = GX_DisableBankForSubOBJ();
    ((s32 *)data_ov016_021de6b0)[9] = GX_DisableBankForSubBGExtPltt();
    ((s32 *)data_ov016_021de6b0)[10] = GX_DisableBankForSubOBJExtPltt();
    ((s32 *)data_ov016_021de6b0)[11] = GX_DisableBankForARM7();
    ((s32 *)data_ov016_021de6b0)[12] = GX_DisableBankForLCDC();
    GX_SetBankForARM7(((s32 *)data_ov016_021de6b0)[11]);
    func_ov016_021d73bc();
}

void DWCi_RepairVram(void) {
    GX_DisableBankForBG();
    GX_DisableBankForOBJ();
    GX_DisableBankForSubBG();
    GX_DisableBankForSubOBJ();
    func_ov016_021d73bc();
    GX_SetBankForBG(*(s32 *)(((u8 *)data_ov016_021de6b0)));
    GX_SetBankForOBJ(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 4));
    GX_SetBankForBGExtPltt(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 8));
    GX_SetBankForOBJExtPltt(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 0xC));
    GX_SetBankForTex(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 0x10));
    GX_SetBankForTexPltt(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 0x14));
    GX_SetBankForClearImage(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 0x18));
    GX_SetBankForSubBG(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 0x1C));
    GX_SetBankForSubOBJ(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 0x20));
    GX_SetBankForSubBGExtPltt(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 0x24));
    GX_SetBankForSubOBJExtPltt(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 0x28));
    GX_SetBankForLCDC(*(s32 *)((u8 *)(((u8 *)data_ov016_021de6b0)) + 0x30));
    *(s16 *)((void *)0x04000050) = 0;
    *(s16 *)((u8 *)((void *)0x04000050) + 0x1000) = 0;
    *(s32 *)((u8 *)((void *)0x04000050) + -0x40) = 0;
    *(s32 *)((u8 *)((void *)0x04000050) + -0x3C) = 0;
    *(s32 *)((u8 *)((void *)0x04000050) + -0x38) = 0;
    *(s32 *)((u8 *)((void *)0x04000050) + -0x34) = 0;
    *(s32 *)((u8 *)((void *)0x04000050) + 0xFC0) = 0;
    *(s32 *)((void *)0x04001014) = 0;
    *(s32 *)((u8 *)((void *)0x04001014) + 4) = 0;
    *(s32 *)((u8 *)((void *)0x04001014) + 8) = 0;
}

void func_ov016_021d73bc(void) {
    GX_SetBankForLCDC(499);
    MIi_CpuClearFast(0, (void *)0x6800000, 0x40000);
    MIi_CpuClearFast(0, (void *)0x6880000, 0x24000);
    GX_DisableBankForLCDC();
    MIi_CpuClearFast(0x200, (void *)0x7000000, 0x400);
    MIi_CpuClearFast(0, (void *)0x5000000, 0x400);
    MIi_CpuClearFast(0x200, (void *)0x7000400, 0x400);
    MIi_CpuClearFast(0, (void *)0x5000400, 0x400);
}

void DWCi_SetLedWireless(void) {
    u32 sp0;

    if (func_0208c6c0(&sp0) != 0) {
        return;
    }
    if (sp0 == 0xF) {
        return;
    }
    func_0208c6a4(0xFU);
}

void DWCi_SetLedNormal(void) {
    func_0208c6a4(1);
}
