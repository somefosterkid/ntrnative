// libdwcutil's obj.c (Black's ov016 0x021d6e14-0x021d6f9c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

static void *data_ov016_021de6a8;
void taskTransfer(void);

void DWCi_OBJlInit(void) {
    s32 i;

    data_ov016_021de6a8 = DWCi_HEAPlAllocEx(0x80c, 4);
    MIi_CpuClearFast(0x200, data_ov016_021de6a8, 0x800);
    for (i = 0; i < 2; i++) {
        ((u32 *)((u8 *)data_ov016_021de6a8 + 0x800))[i] = DWCi_QUElFormEx(0x40, (s32)((u8 *)data_ov016_021de6a8 + i * 0x400), 8);
    }
    *(s32 **)((u8 *)data_ov016_021de6a8 + 0x808) = DWCi_TSKlForm(1, (s32)taskTransfer, 0, 200);
}

void taskTransfer(void) {
    DC_FlushRange(data_ov016_021de6a8, 0x800);
    GX_LoadOAM(data_ov016_021de6a8, 0, 0x400);
    GXS_LoadOAM((void *)((s32)(u8 *)data_ov016_021de6a8 + 0x400), 0, 0x400);
}

void DWCi_OBJlEnd(void) {
    DWCi_TSKlDeleteEx(1, *(s32 **)((u8 *)data_ov016_021de6a8 + 0x808));
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de6a8);
}

void *DWCi_OBJlFormGx(s32 lcd) {
    return (void *)DWCi_QUElPopBack(*(u16 **)(data_ov016_021de6a8 + lcd * 4 + 0x800));
}

void DWCi_OBJlDelete(u32 *obj) {
    s32 flag;

    *obj = (*obj & 0xC1FFFCFF) | 0x200;
    flag = 0;
    if ((u32) obj >= (u32) ((u8 *)data_ov016_021de6a8 + 0x400)) {
        flag = 1;
    }
    DWCi_QUElPushBack(*(u16 **)((u8 *)(((u8 *)data_ov016_021de6a8 + (flag * 4))) + 0x800), (u32) obj);
}

s32 DWCi_OBJlGetOam(s32 lcd, s32 idx) {
    return (s32)(data_ov016_021de6a8 + lcd * 0x400 + idx * 8);
}
