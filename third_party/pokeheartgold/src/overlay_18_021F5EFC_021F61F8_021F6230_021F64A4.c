#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F6230(PokedexAppData *, s32, s32 *, s32, s32);

s32 ov18_021F6244(PokedexAppData *, s32, s32 *, s32, s32);

extern s32 ov18_021FA310;

int ov18_021F64A4(PokedexAppData *pokedexApp, int a1) {
    int temp_r4;
    s32 var_r0;
    u32 temp_r1;
    var_r0 = 0x40;
    temp_r4 = ov18_021F6230(pokedexApp, 5, &ov18_021FA310, var_r0, 0x56);
    var_r0 = ov18_021F6244(pokedexApp, 5, &ov18_021FA310, var_r0, 0x56);
    temp_r1 = pokedexApp->numSeenForms - 1;
    if (a1 != temp_r1) {
        u32 temp_r2 = ((u32)((var_r0 - temp_r4) << 8)) / temp_r1;
        var_r0 = temp_r4 + (((u32)(temp_r2 * a1)) >> 8);
    }
    return var_r0;
}
