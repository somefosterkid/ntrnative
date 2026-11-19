#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"

s32 ov18_021EF1E4(PokedexAppData *, s32);

s32 ov18_021EF220(PokedexAppData *, s32);

s32 ov18_021EF25C(PokedexAppData *, s32);

s32 ov18_021EF298(PokedexAppData *, s32);

s32 ov18_021EF2D4(PokedexAppData *, s32);

s32 ov18_021EF310(PokedexAppData *, s32);

s32 ov18_021EF34C(PokedexAppData *, s32);

void ov18_021EFEC4(PokedexAppData *pokedexApp) {
    PokedexAppData *var_r4;
    Window *window;
    u32 var_r5;

    var_r4 = pokedexApp;
    FillWindowPixelBuffer(&pokedexApp->windows[1], 0U);
    FillWindowPixelBuffer(&var_r4->windows[2], 0U);
    FillWindowPixelBuffer(&var_r4->windows[3], 0U);
    FillWindowPixelBuffer(&var_r4->windows[4], 0U);
    FillWindowPixelBuffer(&var_r4->windows[5], 0U);
    FillWindowPixelBuffer(&var_r4->windows[6], 0U);
    FillWindowPixelBuffer(&var_r4->windows[7], 0U);
    ov18_021EF1E4(var_r4, 1);
    ov18_021EF220(var_r4, 2);
    ov18_021EF25C(var_r4, 3);
    ov18_021EF298(var_r4, 4);
    ov18_021EF2D4(var_r4, 5);
    ov18_021EF310(var_r4, 6);
    ov18_021EF34C(var_r4, 7);
    ov18_021EFBE8(var_r4, 8);
    ov18_021EFC3C(var_r4, 9);
    ov18_021EFC9C(var_r4, var_r4->dexSearchCriteria[2], 0xA, 0x1D);
    ov18_021EFC9C(var_r4, var_r4->dexSearchCriteria[3], 0xB, 0x1D);
    ov18_021EFD00(var_r4, var_r4->unk_1850[var_r4->dexSearchCriteria[4]].unk_0, 0xC);
    ov18_021EFD00(var_r4, var_r4->unk_1850[var_r4->dexSearchCriteria[5]].unk_0, 0xD);
    ov18_021EFDB4(var_r4, var_r4->unk_1850[var_r4->dexSearchCriteria[6]].unk_2, 0xE);
    ov18_021EFDB4(var_r4, var_r4->unk_1850[var_r4->dexSearchCriteria[7]].unk_2, 0xF);
    ov18_021EFE70(var_r4, 0x10);
    var_r5 = 1;
    window = &var_r4->windows[1];
    do {
        CopyWindowPixelsToVram_TextMode(window);
        var_r5 += 1;
        window++;
    } while (var_r5 <= 0x10U);
}
