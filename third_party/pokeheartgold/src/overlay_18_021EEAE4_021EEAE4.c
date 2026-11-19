#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"

s32 ov18_021EEA84(PokedexAppData *, s32, s32, s32, s32, s32, s32, s32);

void ov18_021EEAE4(PokedexAppData *arg0, s32 arg1, s32 arg2, s32 arg3);

void ov18_021EEAE4(PokedexAppData *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp14;
    s32 sp10;
    Window *var_r4;
    PokedexAppData *var_r5;
    s32 var_r7;

    var_r5 = arg0;
    sp10 = arg1;
    var_r4 = var_r5->windows;
    var_r7 = arg2;
    sp14 = arg3;
    FillWindowPixelBuffer(&var_r4[arg3], 0U);
    ov18_021EEA84(var_r5, arg1, var_r5->unk_1030[var_r7].unk_2, arg3, 4, 0, 0x20100, 0);
    ScheduleWindowCopyToVram(&var_r4[arg3]);
}
