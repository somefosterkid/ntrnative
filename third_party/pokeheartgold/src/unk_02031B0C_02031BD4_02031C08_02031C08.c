#include "global.h"

u8 sub_02031C08(void *arg0);

u8 sub_02031C08(void *arg0) {
    s32 var_r2;
    u8 var_r3;
    u32 var_r4;

    var_r3 = 0;
    var_r4 = *(u16 *)((u8 *)arg0 + 0xA);
    var_r2 = 0;
    do {
        if (var_r4 & 7) {
            var_r3 += 1;
        }
        var_r2 += 1;
        var_r4 = (u32)(var_r4 << 0xD) >> 0x10;
    } while (var_r2 < 5);
    return var_r3;
}
