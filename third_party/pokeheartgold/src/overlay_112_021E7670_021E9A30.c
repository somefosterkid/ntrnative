#include "global.h"

u8 ov112_021E98F8(void *, s32, s32);

s32 ov112_021E9998(u8 *, u8 *, s32, s32, s32);

void ov112_021E9A30(u8 *arg0, u8 *arg1, s32 arg2);

void ov112_021E9A30(u8 *arg0, u8 *arg1, s32 arg2) {
    s32 v0;
    s32 v1;

    ov112_021E9998(arg0, arg1, arg2, -8, -0xE);
    ov112_021E9998(arg0 + 0x300, arg1 + 0x300, arg2, -0x16, -0xE);
    v0 = 0;
    do {
        v1 = 0;
loop_2:
        ov112_021E98F8(arg0, v1, v0);
        v1 += 1;
        if (v1 < 0x40) {
            goto loop_2;
        }
        v0 += 1;
    } while (v0 < 0x30);
}
