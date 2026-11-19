#include "global.h"
#include "overlay_28.h"

u16 ov28_0225DE64(s32 arg0, s32 arg1) {
    return FX_Atan2Idx((arg1 - 0x64) << 0xC, (arg0 - 0x54) << 0xC);
}

u16 ov28_0225DE78(s32 arg0, s32 arg1) {
    return FX_Atan2Idx(arg1 << 0xC, arg0 << 0xC);
}

u32 ov28_0225DE88(u32 param_1) {
    return (param_1 & 0xffff) * 0x168 >> 0x10;
}
