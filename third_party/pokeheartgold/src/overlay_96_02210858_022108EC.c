#include "global.h"

s32 ov96_02210AE0(void *, s32, s32);

void ov96_022108EC(u8 *arg0, s32 arg1, s32 arg2);

void ov96_022108EC(u8 *arg0, s32 arg1, s32 arg2) {
    s32 v0;
    u8 *v1;

    v1 = arg0;
    v0 = 0;
    do {
        ov96_02210AE0(v1, arg1, arg2);
        v0 += 1;
        v1 += 0x14;
    } while (v0 < 3);
}
