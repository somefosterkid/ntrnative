#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "pokemon.h"
#include "sprite.h"
#include "sprite_system.h"

s32 ov18_021F3CA8(PokedexAppData *, s32, u8 *, u8 *);

void ov18_021F5EFC(PokedexAppData *pokedexApp, s32 arg1, s32 arg2);

void ov18_021F5EFC(PokedexAppData *pokedexApp, s32 arg1, s32 arg2) {
    u8 form[2];
    s32 a;
    s32 b;

    ov18_021F3CA8(pokedexApp, arg1, &form[0], &form[1]);
    if ((u32)(((u8 *)pokedexApp)[0x18C7] << 26) >> 31 == 0) {
        a = 1;
        b = 2;
        ov18_021F11C0(pokedexApp, 3, 0);
        ov18_021F11C0(pokedexApp, 4, 0);
    } else {
        a = 3;
        b = 4;
        ov18_021F11C0(pokedexApp, 1, 0);
        ov18_021F11C0(pokedexApp, 2, 0);
    }
    {
        u8 *fl = &((u8 *)pokedexApp)[0x18C7];
        *fl = *fl & ~0x20 | ((((u32)(u8)(1 ^ ((u32)(*fl << 26) >> 31))) << 31) >> 26);
    }
    ov18_021F1A7C(pokedexApp, pokedexApp->curSpecies, form[0], form[1], 2, a, arg2);
    ManagedSprite_SetPositionXYWithSubscreenOffset(pokedexApp->unk_0670[a], 0x40, 0x78, FX32_CONST(512));
    ov18_021F1A7C(pokedexApp, pokedexApp->curSpecies, form[0], form[1], 0, b, arg2);
    ManagedSprite_SetPositionXYWithSubscreenOffset(pokedexApp->unk_0670[b], 0xC0, GetMonPicHeightBySpeciesGenderForm(pokedexApp->curSpecies, form[1], 0, form[0], 0) + 0x78, FX32_CONST(512));
    ov18_021F11C0(pokedexApp, a, 1);
    ov18_021F11C0(pokedexApp, b, 1);
}
