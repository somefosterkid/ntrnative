#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "unk_0201956C.h"

extern s32 ov18_021F9E4C;

void ov18_021F018C(PokedexAppData *pokedexApp) {
    PokedexAppData *var_r7;
    s32 var_r5;
    u16 *temp_r2;
    u16 var_r3;
    u16 var_r4;
    u16 var_r6;

    var_r5 = 1;
    var_r7 = pokedexApp;
    var_r4 = 0;
    var_r5 = 0x1000;
    do {
        temp_r2 = sub_02019B08(var_r7->unk_0008, var_r4 + 0x11);
        var_r6 = *(u16 *)((u8 *)(&ov18_021F9E4C + var_r4 * 2) + 0x46);
        var_r3 = 0;
loop_2:
        temp_r2[var_r3] = (var_r6 + var_r3) | 0x1000;
        var_r3 += 1;
        if ((u32)var_r3 < 0x24U) {
            goto loop_2;
        }
        var_r4 += 1;
    } while ((u32)var_r4 < 6U);
}

extern s32 ov18_021F9EBC;

void ov18_021F01D4(PokedexAppData *pokedexApp) {
    PokedexAppData *var_r7;
    s32 var_r5;
    u16 *temp_r2;
    u16 var_r3;
    u16 var_r4;
    u16 var_r6;

    var_r5 = 1;
    var_r7 = pokedexApp;
    var_r4 = 0;
    var_r5 = 0x1000;
    do {
        temp_r2 = sub_02019B08(var_r7->unk_0008, var_r4 + 0x11);
        var_r6 = *(u16 *)((u8 *)(&ov18_021F9EBC + var_r4 * 2) + 0x56);
        var_r3 = 0;
loop_2:
        temp_r2[var_r3] = (var_r6 + var_r3) | 0x1000;
        var_r3 += 1;
        if ((u32)var_r3 < 0x24U) {
            goto loop_2;
        }
        var_r4 += 1;
    } while ((u32)var_r4 < 6U);
}
