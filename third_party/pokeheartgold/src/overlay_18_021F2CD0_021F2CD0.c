#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

u8 ov18_021F2C98(PokedexAppData *, s32);

int ov18_021F2CD0(PokedexAppData *pokedexApp, int a1, u32 x, u32 y) {
    s16 sp0[2];
    s16 temp_r1;
    s16 temp_r3;
    u32 temp_r0;
    u32 temp_r3_2;

    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a1], &sp0[1], &sp0[0]);
    temp_r0 = ov18_021F2C98(pokedexApp, a1);
    temp_r3 = sp0[1];
    if ((x >= (u32)(temp_r3 - 0xB)) && (x <= (u32)(temp_r3 + 0xB))) {
        temp_r3_2 = temp_r0 >> 1;
        temp_r1 = sp0[0];
        if ((y >= (u32)(temp_r1 - temp_r3_2)) && (y <= (u32)(temp_r1 + temp_r3_2))) {
            return 1;
        }
    }
    return 0;
}
