// libdwcutil's cellInfo.c (Black's ov016 0x021d5280-0x021d53dc): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

static u32 data_ov016_021de674[2];
NNSG2dCharCanvas *DWCi_CELLlForm(s32 param_1, s32 param_2, s32 param_3);
void DWCi_CEINlRead(s32 param_1, s8 *param_2);

void DWCi_CEINlRead(s32 lcd, s8 *file) {
    u32 auStack_10 [1];

    *(u32 **)((void *)data_ov016_021de674 + lcd * 4) = DWCi_ARClReadEx(file, auStack_10, 4);
}

void func_ov016_021d52b0(s32 param_1) {
    DWCi_ARClRelease(*(u32 *)((void *)data_ov016_021de674 + param_1 * 4));
    *(u32 *)((void *)data_ov016_021de674 + param_1 * 4) = 0;
}

typedef struct { u16 num; u16 pad; u32 offset; } CellInfoEntry;
void func_ov016_021d52d4(s32 param_1, s32 param_2, void *param_3) {
    u8 *base;
    s32 count;
    s32 i;
    u8 *src;
    u8 *dest;
    u8 buf[8];

    base = ((u8 **)data_ov016_021de674)[param_1];
    count = ((CellInfoEntry *)base)[param_2].num;
    src = base + ((CellInfoEntry *)base)[param_2].offset;
    dest = param_3;
    MIi_CpuClear32(0, buf, 8);
    for (i = 0; i < count; i++) {
        MIi_CpuCopy16(src, buf, 6);
        MIi_CpuCopy32(buf, dest, 8);
        src += 6;
        dest += 8;
    }
}

void *DWCi_CEINlSetExObj(void *lcd, s32 idx) {
    void *ptr;


    ptr = DWCi_OBJlFormGx((s32)lcd);
    func_ov016_021d52d4((s32)lcd, idx, ptr);
    return ptr;
}

s32 *DWCi_CEINlSetExCell(s32 lcd, s32 idx, s32 head) {
    NNSG2dCharCanvas *pNVar1;

    pNVar1 = DWCi_CELLlForm(lcd, (u32)*(u16 *)
                                                                                            (*(s32 *)((void *)data_ov016_021de674 + lcd * 4) +
                                                                                            idx * 8), head);
    func_ov016_021d52d4(lcd, idx, (void *)DWCi_CELLlGetObj(pNVar1, 0));
    return (s32 *)pNVar1;
}
