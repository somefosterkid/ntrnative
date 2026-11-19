#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "pokedex_util.h"
#include "string_util.h"
#include "application/pokedex/pokedex_internal.h"

void ov18_021EE3AC(PokedexAppData *, MsgData *, s32, s32, s32, s32, s32, s32, s32);

void ov18_021EEC34(PokedexAppData *arg0, u32 arg1, s32 arg2, s32 arg3);

void ov18_021EEC34(PokedexAppData *arg0, u32 arg1, s32 arg2, s32 arg3) {
    s32 sp18;
    u32 sp14;
    s32 temp_r0;
    int new_var;
    s32 var_r4;
    s32 var_r6;
    PokedexAppData *var_r5;
    void *var_r7;
    var_r5 = arg0;
    var_r6 = arg2;
    var_r7 = var_r5;
    temp_r0 = var_r6 * sizeof(Window);
    sp14 = arg1;
    var_r7 += offsetof(PokedexAppData, windows);
    sp18 = temp_r0;
    var_r4 = arg3;
    FillWindowPixelBuffer(var_r7 + temp_r0, 0U);
    if (var_r4 == 1) {
        new_var = 9;
        var_r4 = 0;
    } else {
        var_r4 = 1;
    }
    BufferIntegerAsString(var_r5->msgFormat, 0U, (s32)Pokedex_ConvertToCurrentDexNo((s32)var_r5->unk_1858, sp14), 3U, PRINTING_MODE_LEADING_ZEROS, var_r4);
    ov18_021EE3AC(var_r5, var_r5->msgData, var_r6, new_var, 0, 0, 0, 0x20100, 0);
    ScheduleWindowCopyToVram(var_r7 + sp18);
}
