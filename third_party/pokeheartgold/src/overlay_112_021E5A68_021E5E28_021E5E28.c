#include "global.h"

#include "overlay_112_ir_state.h"


void ov112_021E5E28(void);

void ov112_021E5E28(void) {
    s32 temp_r1;
    u64 temp_ret_64;
    void *temp_r4;
    void *temp_r5;

    temp_r4 = ((void *)ov112_021FFAA4.unk_20);
    temp_r5 = temp_r4 + 0x10;
    temp_ret_64 = OS_GetOwnerRtcOffset();
    temp_r1 = (s32)(temp_ret_64 >> 32);
    *(s32 *)((u8 *)temp_r4 + 0x10) = (s32)(u32)temp_ret_64;
    *(s32 *)((u8 *)temp_r5 + 4) = temp_r1;
    OS_GetLowEntropyData(temp_r5 + 8);
}
