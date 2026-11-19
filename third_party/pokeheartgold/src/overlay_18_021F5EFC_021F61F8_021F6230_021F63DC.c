#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

u32 ov18_021F6230(PokedexAppData *, s32, s32 *, s32, s32);

u32 ov18_021F6244(PokedexAppData *, s32, s32 *, s32, s32);

extern s32 ov18_021FA304;

BOOL ov18_021F63DC(PokedexAppData *pokedexApp, u32 x, u32 y) {
    s16 sp4[2];
    u32 temp_r2;
    s32 var_r3;
    u32 var_r6_2;
    u32 temp_r7;
    u32 var_r6;
    s32 var_r7;
    u32 temp_r4;
    u32 temp_r0;
    u32 temp_r0_2;
    u32 temp_r0_3;

    var_r6 = y;
    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[5], &sp4[1], &sp4[0]);
    temp_r0 = ov18_021F6230(pokedexApp, 5, &ov18_021FA304, 0x60, 0x56);
    if (var_r6 < temp_r0) {
        var_r6 = temp_r0;
    }
    temp_r0_2 = ov18_021F6244(pokedexApp, 5, &ov18_021FA304, 0x60, 0x56);
    if (var_r6 > temp_r0_2) {
        var_r6 = temp_r0_2;
    }
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[5], sp4[1], (s16)var_r6);
    temp_r7 = ov18_021F6230(pokedexApp, 5, &ov18_021FA304, 0x60, 0x56);
    temp_r0_2 = ov18_021F6244(pokedexApp, 5, &ov18_021FA304, 0x60, 0x56) - temp_r7;
    temp_r4 = pokedexApp->unk_18DC.unk_20.nMaps - 1;
    temp_r0_3 = (u32)(temp_r0_2 << 8) / temp_r4;
    temp_r2 = (var_r6 - temp_r7) << 8;
    var_r3 = 0;
    var_r6_2 = 0;
    var_r7 = 0;
    do {
        if ((temp_r2 >= var_r6_2) && (temp_r2 < (u32)(var_r7 + temp_r0_3))) {
            if (pokedexApp->unk_18CA != var_r3) {
                pokedexApp->unk_18CA = var_r3;
                return 1;
            }
            break;
        }
        var_r3 += 1;
        var_r6_2 += temp_r0_3;
        var_r7 += temp_r0_3;
    } while ((u32)var_r3 <= temp_r4);
    return 0;
}
