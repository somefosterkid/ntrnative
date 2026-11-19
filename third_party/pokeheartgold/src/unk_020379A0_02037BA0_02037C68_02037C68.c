#include "global.h"
#include "comm_tool.h"


void sub_02037C68(s32 param_1, u32 param_2, void *param_3);

void sub_02037C68(s32 param_1, u32 param_2, void *param_3) {
    _021D414C->hasRecievedTempData[param_1] = 1;
    MI_CpuCopy8(param_3, _021D414C->unk_18[param_1], 0x46);
    return;
}
