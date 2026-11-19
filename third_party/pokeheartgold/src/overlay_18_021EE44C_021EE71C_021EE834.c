#include "global.h"

#include "constants/heap.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "pm_string.h"

s32 ov18_021F95FC(void *, String *, s32, s32, s32, s32, s32);

void ov18_021EE834(PokedexAppData *arg0, u16 arg1, s32 arg2, s32 arg3);

void ov18_021EE834(PokedexAppData *arg0, u16 arg1, s32 arg2, s32 arg3) {
    s32 spC;
    String *var_r0;
    int new_var;
    spC = arg2;
    FillWindowPixelBuffer(&arg0->windows[arg3], 0U);
    if (arg1 != 0) {
        if (arg0->unk_1030[spC].unk_2 == 2) {
            var_r0 = ov18_021E595C(arg1, 2, HEAP_ID_POKEDEX_APP);
        } else {
            var_r0 = ov18_021E595C(0U, 2, HEAP_ID_POKEDEX_APP);
        }
        new_var = GetWindowWidth(&arg0->windows[arg3]);
        new_var = new_var * 8 - 4;
        ov18_021F95FC(&arg0->windows[arg3], var_r0, new_var, 0, 4, 0x20100, 1);
        String_Delete(var_r0);
        ScheduleWindowCopyToVram(&arg0->windows[arg3]);
        return;
    }
    ClearWindowTilemapAndScheduleTransfer(&arg0->windows[arg3]);
}
