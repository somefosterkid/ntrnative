#include "global.h"

u32 ov112_021F0F48(void *arg0, s32 arg1);

u32 ov112_021F0F48(void *arg0, s32 arg1) {
    if (arg1 != 0) {
        return (u32)(*(u8 *)((u8 *)arg0 + 0x85) << 0x18) >> 0x1F;
    }
    return (u32)(*(u8 *)((u8 *)arg0 + 0x86) << 0x18) >> 0x1F;
}
