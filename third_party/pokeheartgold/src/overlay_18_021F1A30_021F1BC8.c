#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "pokedex.h"
#include "sprite_system.h"

void ov18_021F1BC8(PokedexAppData *pokedexApp, u16 species, s32 arg2, s32 arg3);

void ov18_021F1BC8(PokedexAppData *pokedexApp, u16 species, s32 arg2, s32 arg3) {
    s32 v1;
    s32 v2;
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[arg2 + pokedexApp->unk_185F_0], 0);
    pokedexApp->unk_185F_0 ^= 1;
    arg2 = arg2 + pokedexApp->unk_185F_0;
    if (species == 0) {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[arg3], 1);
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[arg2], 0);
        return;
    }
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[arg3], 0);
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[arg2], 1);
    v2 = Pokedex_SpeciesGetLastSeenGender(pokedexApp->args->pokedex, species, 0U);
    v1 = Pokedex_GetSeenFormByIdx(pokedexApp->args->pokedex, (s32)species, 0);
    if (species == 0xAC) {
        if (v1 == 2) {
            v1 = 1;
            v2 = 1;
        } else {
            v1 = 0;
        }
    }
    ov18_021F1A7C(pokedexApp, species, (u8)v1, (u8)v2, 2, arg2, 0);
}
