#include "global.h"

#include "overlay_96_arena_cpu.h"

s32 ov96_021F3930(Ov96ArenaCpu *arg0, Ov96ArenaCpuItem *arg1, s32 arg2, Ov96ArenaCpuMon *arg3, s32 arg4, s32 arg5, BOOL *arg6) {
    Ov96ArenaCpuItem *sp4;
    Ov96ArenaCpu *sp0;
    s32 temp_r0;
    s32 var_r4;
    VecFx32 *temp_r2;

    sp0 = arg0;
    sp4 = arg1;
    var_r4 = 0;
    if (arg2 < 0) {
        return 0;
    }
    *arg6 = FALSE;
    if (*arg3->unk_08 != 0) {
        if (*arg3->state == 1) {
            if (ov96_021F3A5C(arg1, arg4 << 0xC, arg5 << 0xC) != 0) {
                var_r4 += 5;
            }
            if (ov96_021F3AB0(sp0, (u8)arg2) != 0) {
                var_r4 += 4;
                *arg6 = TRUE;
            }
            var_r4 += ov96_021F39F0(sp0, arg3, arg2);
        } else {
            goto block_9;
        }
    } else {
block_9:
        if (ov96_021F3A5C(sp4, arg4 << 0xC, arg5 << 0xC) != 0) {
            var_r4 += 5;
        }
        if (ov96_021F3AB0(sp0, (u8)arg2) != 0) {
            var_r4 += 4;
            *arg6 = TRUE;
        }
        temp_r0 = ov96_021F39F0(sp0, arg3, arg2);
        if (temp_r0 == 0) {
            var_r4 += 2;
        }
        if (arg3->cell == arg2) {
            var_r4 += 2;
        }
        if (temp_r0 != 0) {
            var_r4 = 0;
        }
        temp_r2 = arg3->pos;
        if (ov96_021F3A5C(sp4, temp_r2->x, temp_r2->y) != 0) {
            var_r4 += 4;
        }
    }
    return var_r4;
}
