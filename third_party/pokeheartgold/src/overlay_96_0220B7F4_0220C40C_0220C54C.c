#include "global.h"

s32 ov96_021E6168(void *, s32, s32, s32 *);

s32 ov96_021EEBE4(s32, s32 *, s32, void *, s32);

void ov96_0220C54C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void ov96_0220C54C(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp4[4];
    s32 temp_r4;
    s32 temp_r5;

    temp_r4 = *(s32 *)arg0;
    temp_r5 = *(s32 *)((u8 *)(arg0 + (arg1 * 4)) + 0x10);
    ov96_021E6168(*(void **)((u8 *)arg0 + 0xC), arg2, arg3, sp4);
    ov96_021EEBE4(temp_r5, sp4, arg4, 0, temp_r4);
}
