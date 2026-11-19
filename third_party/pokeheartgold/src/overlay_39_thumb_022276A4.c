#include "global.h"

#include "overlay_39_thumb.h"

s32 ov39_02227DB8(void *arg0);

s32 ov39_022276A4(void *arg0) {
    if (ov39_02227DB8(arg0) == 0) {
        return 0;
    }
    MI_CpuFill8(arg0 + 0x190, 0U, 4U);
    *(s32 *)((u8 *)arg0 + 0x190) = 0x140;
    *(s32 *)((u8 *)arg0 + 0x3F0) = 1;
    *(s32 *)((u8 *)arg0 + 0x3E8) = 0x59D9;
    return 1;
}

s32 ov39_022276E0(void *arg0) {
    if (ov39_02227DB8(arg0) == 0) {
        return 0;
    }
    MI_CpuFill8(arg0 + 0x190, 0U, 4U);
    *(s32 *)((u8 *)arg0 + 0x190) = 0x140;
    *(s32 *)((u8 *)arg0 + 0x3F0) = 2;
    *(s32 *)((u8 *)arg0 + 0x3E8) = 0x59D9;
    return 1;
}

s32 ov39_02227720(void *arg0, s32 arg1, s32 arg2) {
    if (ov39_02227DB8(arg0) == 0) {
        return 0;
    }
    *(s32 *)((u8 *)arg0 + 0x3AC) = arg1;
    *(s32 *)((u8 *)arg0 + 0x3B0) = arg2;
    *(s32 *)((u8 *)arg0 + 0x3E8) = 0x59DA;
    return 1;
}

s32 ov39_0222774C(void *arg0, s32 arg1, s32 arg2) {
    if (ov39_02227DB8(arg0) == 0) {
        return 0;
    }
    *(s32 *)((u8 *)arg0 + 0x3AC) = arg1;
    *(s32 *)((u8 *)arg0 + 0x3B0) = arg2;
    *(s32 *)((u8 *)arg0 + 0x3E8) = 0x59DB;
    return 1;
}
