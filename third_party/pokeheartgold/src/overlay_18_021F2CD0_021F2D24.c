#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

u32 ov18_021F2CB4(PokedexAppData *, s32);

u32 ov18_021F2CC0(PokedexAppData *, s32);

int ov18_021F2D24(PokedexAppData *pokedexApp, int a1, u32 x, u32 y, int a4) {
    u32 temp_r2;
    u32 var_r3;
    u32 var_r6_2;
    s16 sp8[2];
    s32 sp4;
    s32 sp0;
    s32 temp_r0;
    s32 temp_r0_4;
    s32 var_r7;
    u32 temp_r0_2;
    u32 temp_r0_3;
    u32 temp_r0_5;
    u32 temp_r4;
    u32 temp_r7_2;
    u32 var_r6;
    ManagedSprite **temp_r7;

    temp_r7 = pokedexApp->unk_0670;
    temp_r0 = a1;
    sp4 = temp_r0;
    var_r6 = y;
    ManagedSprite_GetPositionXY(temp_r7[temp_r0], &sp8[1], &sp8[0]);
    temp_r0_2 = ov18_021F2CB4(pokedexApp, a1);
    if (var_r6 < temp_r0_2) {
        var_r6 = temp_r0_2;
    }
    temp_r0_3 = ov18_021F2CC0(pokedexApp, a1);
    if (var_r6 > temp_r0_3) {
        var_r6 = temp_r0_3;
    }
    ManagedSprite_SetPositionXY(temp_r7[sp4], sp8[1], (s16)var_r6);
    temp_r7_2 = ov18_021F2CB4(pokedexApp, a1);
    temp_r0_4 = ov18_021F2CC0(pokedexApp, a1) - temp_r7_2;
    sp0 = temp_r0_4;
    temp_r4 = ov18_021F8950(pokedexApp, a4);
    temp_r0_5 = (u32)(temp_r0_4 << 8) / temp_r4;
    temp_r2 = (var_r6 - temp_r7_2) << 8;
    var_r3 = 0;
    var_r6_2 = 0;
    var_r7 = 0;
    do {
        if ((temp_r2 >= var_r6_2) && (temp_r2 < (u32)(var_r7 + temp_r0_5))) {
            if (pokedexApp->unk_1859 != var_r3) {
                pokedexApp->unk_1859 = var_r3;
                return 1;
            }
            break;
        }
        var_r3 += 1;
        var_r6_2 += temp_r0_5;
        var_r7 += temp_r0_5;
    } while ((u32)var_r3 <= temp_r4);
    return 0;
}
