#include "global.h"

#include "bg_window.h"

void ov99_021E738C(BgConfig **arg0, u8 arg1, u8 arg2);

void ov99_021E738C(BgConfig **arg0, u8 arg1, u8 arg2) {
    s16 sp18;
    s32 sp14;
    s32 sp10;
    s32 spC;
    s32 temp_r0;
    s32 var_r4;
    s32 var_r5;

    spC = arg1;
    var_r5 = 0;
    sp10 = arg2;
    sp14 = 0;
    do {
        var_r4 = 0;
loop_2:
        sp18 = var_r4 + var_r5 + 1;
        LoadRectToBgTilemapRect(*arg0, 3U, &sp18, (u8)(spC + var_r4), (u8)(s32)(s32)sp10, 1U, 1U);
        var_r4 += 1;
        if (var_r4 < 2) {
            goto loop_2;
        }
        var_r5 += 2;
        sp10 += 1;
        temp_r0 = sp14 + 1;
        sp14 = temp_r0;
    } while (temp_r0 < 2);
}
