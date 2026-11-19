#include "global.h"

#include "overlay_96_unk_struct_ov96_021_f94a8.h"

typedef struct {
    s32 a[3];
} ov96_0221C41C_Array;

extern const ov96_0221C41C_Array ov96_0221C41C;

u8 ov96_021FB784(UnkStruct_ov96_021F94A8 *arg0, s32 arg1, s32 arg2);

u8 ov96_021FB784(UnkStruct_ov96_021F94A8 *arg0, s32 arg1, s32 arg2) {
    s32 sp0[3];
    s32 temp_r1;
    u8 var_r0;

    *((ov96_0221C41C_Array *)sp0) = ov96_0221C41C;
    var_r0 = 0;
    do {
        temp_r1 = sp0[var_r0] - 0x18;
        if (temp_r1 < arg1 && arg1 < temp_r1 + 0x30 && arg2 > 0x58 && arg2 < 0xA8) {
            break;
        }
        var_r0 += 1;
        if (((u32)var_r0) >= 3U) {
            var_r0 = 3;
            break;
        }
    } while (1);
    return var_r0;
}
