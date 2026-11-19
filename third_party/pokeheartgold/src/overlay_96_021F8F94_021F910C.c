#include "global.h"

#include "error_handling.h"

s32 ov96_021F91E8(void *, s32);

void ov96_021F910C(u8 *arg0, s32 arg1, u32 arg2);

void ov96_021F910C(u8 *arg0, s32 arg1, u32 arg2) {
    s32 v0;

    if (arg2 >= 4U) {
        GF_AssertFail();
    }
    v0 = arg2 * 0x28;
    *(s32 *)((u8 *)(arg0 + v0) + 0x44) = arg1;
    ov96_021F91E8(arg0 + 0x24 + v0, arg1);
}
