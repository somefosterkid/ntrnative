#include "global.h"

u8 ov112_021F0F60(void *arg0);

s32 ov112_021F0F70(u32 **arg0, u32 **arg1);

u8 ov112_021F0F60(void *arg0) {
    u16 v0;

    v0 = *(u8 *)((u8 *)arg0 + 0x76);
    if ((u32)v0 >= 8U) {
        v0 = 0;
    }
    return v0;
}

s32 ov112_021F0F70(u32 **arg0, u32 **arg1) {
    u32 v0;
    u32 v1;

    v1 = **arg1;
    v0 = **arg0;
    if (v0 == v1) {
        return 0;
    }
    if (v0 > v1) {
        return 1;
    }
    return -1;
}
