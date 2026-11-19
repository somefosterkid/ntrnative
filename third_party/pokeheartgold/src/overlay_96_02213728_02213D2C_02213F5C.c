#include "global.h"

#include "error_handling.h"

extern void *ov96_0221DC80;

s32 ov96_02213F5C(s32 arg0);

s32 ov96_02213F5C(s32 arg0) {
    s32 var_r0;
    s32 var_r4;
    u8 temp_r1;

    var_r4 = arg0;
    if (var_r4 < 0xC0) {
        return var_r4 << 0xC;
    }
    if (var_r4 < 0xD8) {
        var_r4 -= 0xC0;
        temp_r1 = (var_r4 % 8);
        if (var_r4 < 8) {
            var_r0 = *(u8 *)(((u8 *)&ov96_0221DC80) + temp_r1) + 0x58;
        } else if (var_r4 < 0x10) {
            var_r0 = *(u8 *)(((u8 *)&ov96_0221DC80) + temp_r1) + 0x68;
        } else {
            var_r0 = *(u8 *)(((u8 *)&ov96_0221DC80) + temp_r1) + 0x78;
        }
        return var_r0 << 0xC;
    }
    if (var_r4 < 0xE8) {
        return var_r4 << 0xC;
    }
    GF_AssertFail();
    return var_r4 << 0xC;
}
