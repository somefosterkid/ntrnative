#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"

s32 ov96_0220C768(ManagedSprite *, s32, s32);

void ov96_0220BD38(s32 *arg0);

void ov96_0220BD38(s32 *arg0) {
    s32 sp8;
    s32 sp4;
    s32 *sp0;
    ManagedSprite *var_r4;
    s32 *var_r6;
    s32 *var_r7;
    s32 temp_r2;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r5;
    u32 temp_r0;
    u32 temp_r1;

    var_r6 = arg0;
    if (((u32)(*(s32 *)((u8 *)var_r6 + 64) << 0x16) >> 0x18) != 3) {
        sp0 = var_r6;
        var_r7 = var_r6;
        var_r5 = 0;
        sp0 = var_r6 + 13;
        var_r7 += 17;
        do {
            temp_r0 = (u32)(*(s32 *)((u8 *)var_r6 + 64) << 0x1E) >> 0x1E;
            if (var_r5 == temp_r0) {
                sp8 = 0x1A;
                sp4 = 0x18;
                var_r4 = *(ManagedSprite **)((u8 *)var_r6 + 0x18);
                *var_r7 &= 0xFFFFFDFF;
            } else {
                var_r0 = temp_r0 + 1;
                if (var_r0 >= 3) {
                    var_r0 = 0;
                }
                if (var_r0 == var_r5) {
                    var_r0_2 = 1;
                } else {
                    var_r0_2 = 0;
                }
                var_r4 = *(ManagedSprite **)((u8 *)(&var_r6[var_r0_2]) + 0x2C);
                sp4 = 0x11;
            }
            temp_r2 = *sp0;
            temp_r1 = (u32)(temp_r2 << 6) >> 0x18;
            if (temp_r1 == 0) {
                ManagedSprite_SetAnimNoRestart(var_r4, sp8);
                if (ManagedSprite_GetDrawFlag(var_r4) == 0) {
                    ManagedSprite_SetDrawFlag(var_r4, 1);
                }
                if (IsSEPlaying(0x890U) == 0) {
                    PlaySE(0x890U);
                }
            } else if (ov96_0220C768(var_r4, temp_r1 << 0xC, ((u32)(temp_r2 << 0xE) >> 0x17) << 0xC) != 0) {
                ManagedSprite_SetAnimNoRestart(var_r4, sp4);
                ManagedSprite_SetDrawFlag(var_r4, 1);
                if (var_r5 == ((u32)(*(s32 *)((u8 *)var_r6 + 64) << 0x1E) >> 0x1E)) {
                    *var_r7 |= 0x200;
                }
            } else {
                ManagedSprite_SetDrawFlag(var_r4, 0);
                ManagedSprite_SetAnimSpeed(var_r4, FX32_ONE);
            }
            var_r5 += 1;
            sp0 += 1;
        } while (var_r5 < 3);
    }
}
