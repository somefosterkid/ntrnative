#include "global.h"

#include "sprite_system.h"
#include "overlay_14_box_funcs.h"

void ov14_021F3B5C(void *arg0);

extern s8 ov14_021F8070;

extern s8 ov14_021F8078;

void ov14_021F3B5C(void *arg0) {
    s16 sp0[2];
    s8 *var_r6;
    s8 *var_r4;
    u32 var_r7;
    void *var_r5;

    var_r5 = arg0;
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)var_r5 + 0x328), &sp0[1], &sp0[0]);
    if (*(s32 *)((u8 *)var_r5 + 0x3F0) != 0) {
        var_r6 = &ov14_021F8078;
        var_r4 = &ov14_021F8070;
        var_r7 = 0;
        do {
            ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)var_r5 + 0x3F0), (s16)(sp0[1] + *var_r4), (s16)(sp0[0] + *var_r6));
            var_r6 += 1;
            var_r4 += 1;
            var_r5 += 4;
            var_r7 += 1;
        } while (var_r7 < 8U);
    }
}
