#include "global.h"

#include "error_handling.h"

void ov96_021E8914(s32 *arg0, s32 arg1, void *arg2, s32 arg3);

void ov96_021E8914(s32 *arg0, s32 arg1, void *arg2, s32 arg3) {
    void *sp0;
    s32 (*temp_r0)();
    s32 *var_r2;
    s32 var_r0;
    void *var_r4;

    var_r4 = NULL;
    sp0 = arg2;
    var_r0 = 0;
    var_r2 = arg0;
loop_1:
    if (*var_r2 == 0) {
        var_r4 = arg0 + var_r0 * 14;
    } else {
        var_r0 += 1;
        var_r2 += 14;
        if (var_r0 < 8) {
            goto loop_1;
        }
    }
    if (var_r4 == NULL) {
        GF_AssertFail();
    }
    temp_r0 = *(s32(**)())((u8 *)(*(u8 **)((u8 *)arg0 + 0x1C0) + (0xC * (arg1 - 0x16))) + 4);
    if (temp_r0 != NULL) {
        s32 temp_r1 = temp_r0();
        if (temp_r1 != arg3) {
            GF_AssertFail();
        }
    }
    if (arg3 > 0x26) {
        GF_AssertFail();
    }
    *(s32 *)((u8 *)var_r4 + 4) = (s32)(arg1 - 0x16);
    if (arg3 > 0) {
        memcpy(var_r4 + 8, sp0, (u32)arg3);
    }
    *(s32 *)((u8 *)var_r4 + 0x30) = arg3;
    *(s32 *)var_r4 = 1;
}
