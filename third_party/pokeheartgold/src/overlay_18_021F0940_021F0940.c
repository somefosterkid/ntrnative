#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "msgdata.h"
#include "unk_0201956C.h"

s32 ov18_021EE3AC(PokedexAppData *, MsgData *, s32, s32, s32, s32, s32, s32, s32);

s32 ov18_021F09D8(PokedexAppData *, s32 arg1);

void ov18_021F0940(PokedexAppData *pokedexApp) {
    s32 sp14;
    Window *temp_r6;
    u32 temp_r0;
    s32 temp_r1;
    u32 var_r4;

    var_r4 = 0;
    temp_r6 = pokedexApp->windows;
    do {
        temp_r0 = var_r4 + 0xA;
        FillWindowPixelBuffer(&temp_r6[temp_r0], 0U);
        temp_r1 = (pokedexApp->unk_18C5 + var_r4) - 2;
        if ((temp_r1 >= 0) && (temp_r1 < (s32)pokedexApp->numSeenForms)) {
            ov18_021EE3AC(pokedexApp, pokedexApp->msgData, var_r4 + 0xA, ov18_021F09D8(pokedexApp, temp_r1), 0, 0, 4, 0xF0C00, 0);
        }
        CopyWindowPixelsToVram_TextMode(&temp_r6[temp_r0]);
        sp14 = (s32)GetWindowX(&temp_r6[temp_r0]);
        sub_020196E8(pokedexApp->unk_0008, var_r4 + 0x11, (s32)(s8)sp14, (s32)(s8)GetWindowY(&temp_r6[temp_r0]));
        var_r4 += 1;
    } while (var_r4 < 6U);
}
