#include "global.h"

#include "unk_020379A0.h"
#include "comm_tool.h"


void sub_02037BC8(void) {
    s32 iVar1;
    s32 iVar2;

    iVar1 = 0;
    iVar2 = 0;
    do {
        MI_CpuFill8(&_021D414C->unk_00[iVar1], 0, sizeof(UnkStruct_02036574));
        iVar1 = iVar1 + 1;
        iVar2 = iVar2 + 2;
    } while (iVar1 < 8);
    return;
}
