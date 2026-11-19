#include "global.h"

#include "save.h"

void sub_0202E474(u8 *);

void sub_0202E43C(u8 *param_1);

s32 sub_0202E48C(u8 *, s8);
s32 sub_0202E498(u8 *);
void sub_0202E474(u8 *arg0);

void sub_0202E43C(u8 *param_1) {
    MIi_CpuClearFast(0, (void *)(param_1 + 0xc), 0xc0);
    MIi_CpuClearFast(0, (void *)(param_1 + 0xcc), 0xc0);
    MIi_CpuClearFast(0, (void *)(param_1 + 0x18c), 0xc0);
    sub_0202E474(param_1);
    SaveSubstruct_UpdateCRC(0x17);
    return;
}

void sub_0202E474(u8 *arg0) {
    sub_0202E48C(arg0, 0);
    sub_0202E498(arg0);
    SaveSubstruct_UpdateCRC(0x17);
}
