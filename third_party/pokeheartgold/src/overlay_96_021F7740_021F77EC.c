#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "math_util.h"
#include "overlay_96_unk_struct_ov96_021_f7684.h"

void ov96_021E8228(s32, s32, u32, s32, s32);

void ov96_021F77EC(s32 arg0, UnkStruct_ov96_021F7684 *arg1, s32 arg2);

void ov96_021F77EC(s32 arg0, UnkStruct_ov96_021F7684 *arg1, s32 arg2) {
    s32 sp4;
    s32 var_r0;
    s32 *temp_r7;
    long long temp_r0;
    u8 var_r4;
    u8 *temp_r4;
    sp4 = arg0;
    temp_r4 = arg1->unk_8;
    temp_r7 = (s32 *)ov96_021E8A20(arg1->dataCopyArea + 0x50 + (0x28 * arg2));
    if (temp_r4[arg2] == 0) {
        var_r0 = 0;
    } else {
        var_r0 = (arg1->unk_C[arg2] + (temp_r4[arg2] * 2)) + (((s32)LCRandom()) % 101);
    }
    if (var_r0 >= 0x64) {
        *temp_r7 = 1;
        temp_r0 = (u8)(((s32)LCRandom()) % ((s32)temp_r4[arg2]));
        var_r4 = 0;
loop_5:
        if (((u32)temp_r0) < ((u32)arg1->unk_10[arg2][var_r4])) {
            ov96_021E8228(sp4, arg2, var_r4, 3, 1);
        } else {
            var_r4 += 1;
            if (((u32)var_r4) < 3U) {
                goto loop_5;
            }
        }

        if (((u32)var_r4) >= 3U) {
            GF_AssertFail();
        }
    }
}
