#include "error_handling.h"
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "math_util.h"
#include "pokeathlon/pokeathlon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
s32 ov96_021E8228(PokeathlonCourseData *, u8, s32, s32, s32);
s32 ov96_0220B1D8(void *, s32, s16, s16);
s32 sub_0200592C(s32, s32, s32);

typedef struct CourseBits { /* invented */
    u8 pad[0x17C];
    u32 lo : 16;
    u32 hi : 16;
    u32 unk0 : 4;
    u32 cnt : 4;
    u32 rest : 24;
} CourseBits;

u16 ov96_0220AAEC(void *arg0, s32 arg1, s16 arg2, s32 arg3, s32 arg4, s32 *arg5) {
    u32 sp20;
    u32 sp1C;
    s16 sp18;
    void *sp10;
    s32 sp4;
    s16 temp_r0_2;
    s16 temp_r0_3;
    s16 var_r6;
    s32 var_r1;
    s32 var_r7;
    s32 var_r7_2;
    u16 temp_r0_4;
    u32 temp_r0;
    u32 temp_r1_2;
    void *temp_r1_3;
    void *temp_r4;
    u32 spC;
    s32 sp8;

    sp4 = arg1;
    var_r6 = arg2;
    sp8 = 0;
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (((CourseBits *)arg0)->cnt == 0) {
        return 0U;
    }
    System_GetTouchNewCoords(&sp20, &sp1C);
    var_r7 = 7;
    ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)arg0 + 0xC), (s16) sp20, (s16) sp1C);
    if ((arg3 != 0) || (arg4 != 0)) {
        var_r7 = 0x12;
    }
    ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)arg0 + 0xC), var_r7);
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)arg0 + 0xC), 1);
    if (arg4 != 0) {
        var_r6 += 0x96;
        PlaySE(0x8BBU);
    } else if (arg3 != 0) {
        var_r6 += 0xA0;
        PlaySE(0x8BBU);
    }
    var_r7_2 = 0;
    temp_r0 = ((CourseBits *)arg0)->cnt;
    spC = temp_r0;
    if ((s32) temp_r0 > 0) {
        sp10 = arg0;
        sp10 = arg0 + 0x14;
loop_13:
        temp_r4 = *(void **)((u8 *)((arg0 + ((spC - (var_r7_2 + 1)) * 4))) + 0x154);
        if ((s32) var_r6 > 0) {
            *(s16 *)((u8 *)temp_r4 + 8) = (s16) (*(s16 *)((u8 *)temp_r4 + 8) - var_r6);
            temp_r0_2 = *(s16 *)((u8 *)temp_r4 + 8);
            if ((s32) temp_r0_2 <= 0) {
                if ((s32) temp_r0_2 <= 0) {
                    ((CourseBits *)arg0)->cnt -= 1;
                    ((CourseBits *)arg0)->hi += 1;
                    if (((CourseBits *)arg0)->hi > 0xC8) {
                        ((CourseBits *)arg0)->hi = 0xC8;
                    }
                    ManagedSprite_SetAnim(*(ManagedSprite **)temp_r4, 0);
                    ManagedSprite_GetPositionXY(*(ManagedSprite **)temp_r4, &sp18 + 1, &sp18);
                    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)temp_r4 + 4), 0);
                    var_r6 = 0 - *(s16 *)((u8 *)temp_r4 + 8);
                    sp8 += 1;
                    ov96_0220B1D8(sp10, 4, *(s16 *)((u8 *)(&sp18) + 2), *(s16 *)(&sp18));
                    ov96_021E8228(*(PokeathlonCourseData **)arg0, (u8) ov96_021E5F24(*(PokeathlonCourseData **)arg0), sp4, 3, 1);
                }
                var_r7_2 += 1;
                if (var_r7_2 < (s32) spC) {
                    goto loop_13;
                }
            }
        }
    }
    if (sp8 != 0) {
        PlaySE(0x8B9U);
    } else {
        PlaySE(0x8B8U);
        ManagedSprite_SetPositionXY(*(ManagedSprite **)((u8 *)arg0 + 0x10), (s16) sp20, (s16) sp1C);
        ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)arg0 + 0x10), 6);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)arg0 + 0x10), 1);
    }
    if (sp8 == 0) {
        temp_r1_2 = ((CourseBits *)arg0)->cnt;
        if (temp_r1_2 != 0) {
            temp_r1_3 = *(void **)((u8 *)((arg0 + (temp_r1_2 * 4))) + 0x150);
            temp_r0_3 = *(s16 *)((u8 *)temp_r1_3 + 8);
            if ((s32) temp_r0_3 <= 0x14) {
                ManagedSprite_SetAnim(*(ManagedSprite **)temp_r1_3, 3);
            } else if ((s32) temp_r0_3 <= 0x3C) {
                ManagedSprite_SetAnim(*(ManagedSprite **)temp_r1_3, 2);
                PlaySE(0x8B8U);
                temp_r0_4 = LCRandom();
                sub_0200592C(0x8B8, 0xFFFF, 0x40 - ((temp_r0_4 % 128)));
            }
        }
    }
    if ((sp8 > 0) && (((CourseBits *)arg0)->cnt == 0)) {
        var_r1 = 1;
    } else {
        var_r1 = 0;
    }
    *arg5 = var_r1;
    return (u16) sp8;
}
