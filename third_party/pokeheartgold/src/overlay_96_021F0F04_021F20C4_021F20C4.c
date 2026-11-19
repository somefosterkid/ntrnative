#include "global.h"

#include "error_handling.h"

#include "overlay_96_ov96_arena_work.h"

s32 ov96_021E6104(void *);

s32 ov96_021E872C(s32, s32, s32, s32, s32, s32 *);

u8 ov96_021F20C4(Ov96ArenaWork *arg0, u8 arg1, u8 arg2, u8 arg3);

u8 ov96_021F20C4(Ov96ArenaWork *arg0, u8 arg1, u8 arg2, u8 arg3) {
    s32 sp20[3];
    u32 sp18[2];
    s32 sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r1_2;
    s32 temp_r4;
    s32 var_r6;
    u8 var_r2;
    u8 var_r3;
    u8 var_r4;
    u8 var_r5;
    Ov96ArenaMon *temp_r1;

    sp8 = arg1;
    spC = arg2;
    sp10 = arg3;
    temp_r0 = ov96_021E6104(arg0);
    var_r5 = 0;
    sp14 = temp_r0;
    var_r6 = temp_r0 << 0xC;
    var_r4 = 0;
    do {
        temp_r1 = &arg0->mon[sp8][var_r4];
        temp_r0_2 = temp_r1->pos.x;
        temp_r1_2 = temp_r1->pos.y;
        if (ov96_021E872C((temp_r0_2 / 0x1000), (temp_r1_2 / 0x1000), spC, sp10, sp14, ((s32 *)sp18)) != 0) {
            (&sp20[0])[var_r4] = (*(s32 *)sp18);
            *(u8 *)(((u8 *)((u8 *)sp18 + 4)) + var_r4) = 1;
            var_r5 += 1;
        } else {
            (&sp20[0])[var_r4] = 0;
            *(u8 *)(((u8 *)((u8 *)sp18 + 4)) + var_r4) = 0;
        }
        var_r4 += 1;
    } while ((u32)var_r4 < 3U);
    if (var_r5 == 0) {
        return 0xCU;
    }
    var_r2 = 3;
    var_r3 = 0;
    do {
        if (*(u8 *)(((u8 *)((u8 *)sp18 + 4)) + var_r3) != 0) {
            temp_r4 = (&sp20[0])[var_r3];
            if (temp_r4 < var_r6) {
                var_r2 = var_r3;
                var_r6 = temp_r4;
            }
        }
        var_r3 += 1;
    } while ((u32)var_r3 < 3U);
    if (var_r2 == 3) {
        GF_AssertFail();
        return 0xCU;
    }
    return (u8)(var_r2 + (sp8 * 3));
}
