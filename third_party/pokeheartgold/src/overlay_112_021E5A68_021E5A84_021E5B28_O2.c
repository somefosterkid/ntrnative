#include "global.h"

#include "overlay_112_ir_state.h"

void ov112_021E5938(void);

s32 ov112_021E5900(s32 *, u32, s32, s32);


extern void *ov112_021FFA18;

s32 ov112_021E5B28(u32 arg0, s32 arg1, s32 arg2, s32 arg3);

s32 ov112_021E5B28(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_r0;
    u16 temp_r4;
    u32 var_r0;
    u8 *var_r6;

    var_r6 = (u8 *)&ov112_021FFA18;
    if (arg0 == 1) {
        if (*var_r6 == 0xFC) {
            return 1;
        }
        return 0;
    }
    if (arg0 < 4U) {
        return 0;
    }
    temp_r4 = var_r6[2] + (var_r6[3] << 8);
    var_r6[2] = 0U;
    var_r6[3] = 0U;
    temp_r0 = ov112_021E5900((s32 *)var_r6, arg0, arg2, arg3);
    var_r6[2] = (u8)temp_r4;
    var_r6[3] = (u8)((s32)temp_r4 >> 8);
    if (temp_r4 == temp_r0) {
        _021FF9E0.unk_00 = 0;
        return 1;
    }
    if (_021FF9E0.unk_2C != 0) {
        for (var_r0 = 0; var_r0 < arg0; var_r0++) {
        }
        ov112_021E5938();
    }
    return 0;
}
