#include "global.h"

#include "bg_window.h"
#include "overlay_99_pokeathlon_event_record.h"

void ov99_021E86D4(void *arg0, s32 arg1, u8 arg2, s32 arg3, s32 arg4, s32 arg5);

void ov99_021E86D4(void *arg0, s32 arg1, u8 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 sp20;
    u8 sp1C;
    s32 sp18;
    s32 sp14;
    s16 sp24;
    s32 sp10;
    s32 spC;
    s32 temp_r0;
    s32 var_r4;
    s32 var_r5;
    u8 var_r0;

    spC = arg2;
    sp10 = arg3;
    if (arg5 != 0) {
        var_r0 = 6;
    } else {
        var_r0 = 2;
    }
    sp1C = (s32)var_r0;
    sp18 = 6 * arg1;
    var_r5 = 0;
    sp20 = 0;
    sp14 = 0x168 * arg4;
    do {
        var_r4 = 0;
loop_5:
        temp_r0 = var_r4 + 1 + sp18;
        temp_r0 += var_r5;
        sp24 = sp14 + temp_r0;
        LoadRectToBgTilemapRect(((PokeathlonEventRecord *)arg0)->bgConfig, sp1C, &sp24, (spC + var_r4), (s32)sp10, 1U, 1U);
        var_r4 += 1;
        if (var_r4 < 6) {
            goto loop_5;
        }
        var_r5 += 0x3C;
        sp10 += 1;
        temp_r0 = sp20 + 1;
        sp20 = temp_r0;
    } while (temp_r0 < 6);
    BgCommitTilemapBufferToVram(((PokeathlonEventRecord *)arg0)->bgConfig, sp1C);
}
