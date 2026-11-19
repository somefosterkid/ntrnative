#include "global.h"

#include "overlay_96_ov96_arena_work.h"

s32 ov96_021F32FC(s32, s32, s32, s32, u16);

extern void *ov96_0221DBB8;

extern s32 ov96_0221DBC8;

s32 ov96_021F3180(u8 arg0, u16 arg1, u16 arg2, u8 *arg3) {
    s32 spC;
    u16 *sp8;
    u8 *sp4;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r1;
    u16 *temp_r0_3;
    u8 var_r5;
    void *temp_r3;
    sp4 = arg3;
    if (arg0 == 0xF) {
        return 0;
    }
    var_r5 = 0;
    spC = 0;
loop_3:
    temp_r1 = *((s32 *)((&ov96_0221DBC8) + arg0));

    temp_r0 = var_r5 * 8;
    temp_r3 = (void *)(temp_r1 + temp_r0);
    temp_r0_2 = *((s32 *)(temp_r1 + temp_r0));
    if (temp_r0_2 != 0) {
        temp_r0_3 = ((u16 **)&ov96_0221DBB8)[temp_r0_2 - 1];
        sp8 = temp_r0_3;
        if (ov96_021F32FC(arg1, arg2, *((u16 *)(((u8 *)temp_r3) + 4)), *((u16 *)(((u8 *)temp_r3) + 6)), (s32)(*temp_r0_3)) != 0) {
            spC = (s32)((u8)(*((u16 *)(((u8 *)sp8) + 2))));
            *sp4 = var_r5;
        } else {
            var_r5 += 1;
            if (((u32)var_r5) < 4U) {
                goto loop_3;
            }
        }
    }
    return spC;
}
