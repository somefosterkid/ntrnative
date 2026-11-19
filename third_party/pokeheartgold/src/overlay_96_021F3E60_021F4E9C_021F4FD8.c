#include "global.h"

void ov96_021F4FD8(s32 arg0, void *arg1);

void ov96_021F4FD8(s32 arg0, void *arg1) {
    s32 v3;
    u16 *v4;
    u16 v0;
    u16 v1;
    u32 v2;

    v2 = (u32)(*(s32 *)((u8 *)arg1 + 8) << 0xF) >> 0x10;
    v4 = arg1 + 0xC;
    v3 = 0;
    if ((s32)v2 > 0) {
        do {
            v0 = *v4;
            v3 += 1;
            v1 = v0 & 0xFFF;
            *v4 = (u8)(v0 - v1) + (u16)(v1 + arg0);
            v4 += 1;
        } while (v3 < (s32)v2);
    }
}
