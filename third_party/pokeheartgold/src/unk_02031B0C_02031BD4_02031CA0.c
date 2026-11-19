#include "global.h"

u8 sub_02031C08(void *arg0);

s32 sub_02031CA0(void *arg0);

s32 sub_02031CA0(void *arg0) {
    u32 v0;

    if (sub_02031C08(arg0) == 0) {
        return 0;
    }
    v0 = (u32)(*(s32 *)((u8 *)arg0 + 0x1C) * 4) >> 2;
    if (v0 < 0x32U) {
        return 1;
    }
    if (v0 < 0x50U) {
        return 2;
    }
    return 3;
}
