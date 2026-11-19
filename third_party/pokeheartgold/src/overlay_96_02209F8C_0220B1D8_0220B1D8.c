#include "global.h"

#include "math_util.h"
#include "sprite.h"
#include "sprite_system.h"

void ov96_0220B1D8(void *arg0, u8 arg1, s16 arg2, u8 arg3);

void ov96_0220B1D8(void *arg0, u8 arg1, s16 arg2, u8 arg3) {
    s32 sp8;
    s16 sp4;
    s32 sp0;
    s16 var_r7;
    s32 var_r6;
    void *var_r5;
    s32 var_r4;
    f32 var_r0;
    f32 var_r0_2;

    sp0 = arg1;
    var_r4 = 0;
    var_r5 = arg0;
    var_r7 = arg2;
    sp4 = arg3;
    var_r6 = 0;
loop_1:
    if (*(s32 *)((u8 *)var_r5 + 4) == 0) {
        *(s32 *)((u8 *)var_r5 + 4) = 1;
        *(s16 *)((u8 *)var_r5 + 0x10) = var_r7;
        *(s16 *)((u8 *)var_r5 + 0x12) = sp4;
        if (((MTRandom() & 7) + 4) != 0) {
            var_r0 = 0.5f + (f32)(((MTRandom() & 7) + 4) << 0xC);
        } else {
            var_r0 = (f32)(((MTRandom() & 7) + 4) << 0xC) - 0.5f;
        }
        if ((var_r6 % 2) == 0) {
            sp8 = 1;
        } else {
            sp8 = -1;
        }
        *(s32 *)((u8 *)var_r5 + 8) = (s32)(sp8 * (s32)var_r0);
        if (((MTRandom() & 7) + 8) != 0) {
            var_r0_2 = 0.5f + (f32)(((MTRandom() & 7) + 8) << 0xC);
        } else {
            var_r0_2 = (f32)(((MTRandom() & 7) + 8) << 0xC) - 0.5f;
        }
        *(s32 *)((u8 *)var_r5 + 0xC) = (s32)var_r0_2;
        ManagedSprite_SetPositionXY(*(ManagedSprite **)var_r5, var_r7, sp4);
        ManagedSprite_ResetSpriteAnimCtrlState(*(ManagedSprite **)var_r5);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)var_r5, 1);
        var_r4 += 1;
    }
    if (var_r4 < sp0) {
        var_r6 += 1;
        var_r5 += 0x14;
        if ((s32)var_r6 < 0x10) {
            goto loop_1;
        }
    }
}
