#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "pokedex.h"
#include "pokemon.h"
#include "sprite.h"
#include "sprite_system.h"
#include "wild_encounter.h"

void ov18_021F21FC(void *arg0, s32 arg1, s32 arg2);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F209C(PokedexAppData *pokedexApp, u32 species, int a2, int a3) {
    u32 uVar1;
    u16 uVar3;
    s32 iVar4;

    if (((pokedexApp->unk_185C != 2) || (species == 0)) || (pokedexApp->unk_1030[a2].unk_2 == 1)) {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3], 0);
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3 + 1], 0);
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3 + 2], 0);
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3 + 3], 0);
        return;
    }
    if (pokedexApp->unk_185F_4 == 0) {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3], 0);
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3 + 1], 0);
        a3 = a3 + 2;
    } else {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3 + 2], 0);
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3 + 3], 0);
    }
    pokedexApp->unk_185F_4 ^= 1;
    iVar4 = Pokedex_GetSeenFormByIdx(pokedexApp->args->pokedex, species, 0);
    if (species == 0xac) {
        if (iVar4 == 2) {
            iVar4 = 1;
        } else {
            iVar4 = 0;
        }
    }
    uVar1 = GetMonBaseStat_HandleAlternateForm(species, iVar4, 6);
    ov18_021F21FC(pokedexApp, a3, uVar1 & 0xffff);
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3], 1);
    uVar3 = GetMonBaseStat_HandleAlternateForm(species, iVar4, 7);
    if (!((uVar3 != 0) && ((uVar1 & 0xffff) != uVar3))) {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3 + 1], 0);
        return;
    }
    ov18_021F21FC(pokedexApp, a3 + 1, uVar3);
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a3 + 1], 1);
    return;
}
#else
void ov18_021F209C(PokedexAppData *pokedexApp, u32 species, int a2, int a3) {
    int iVar2;
    u32 uVar1;
    u16 uVar3;
    s32 iVar4;

    if (((pokedexApp->unk_185C != 2) || (species == 0)) || (pokedexApp->unk_1030[a2].unk_2 == 1)) {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + a3 * 4), 0);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + a3 * 4 + 4), 0);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + a3 * 4 + 8), 0);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + a3 * 4 + 0xc), 0);
        return;
    }
    if (pokedexApp->unk_185F_4 == 0) {
        iVar4 = a3 * 4;
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + iVar4), 0);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + iVar4 + 4), 0);
        a3 = a3 + 2;
    } else {
        iVar4 = a3 * 4;
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + iVar4 + 8), 0);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + iVar4 + 0xc), 0);
    }
    pokedexApp->unk_185F_4 ^= 1;
    iVar4 = Pokedex_GetSeenFormByIdx(pokedexApp->args->pokedex, species, 0);
    if (species == 0xac) {
        if (iVar4 == 2) {
            iVar4 = 1;
        } else {
            iVar4 = 0;
        }
    }
    uVar1 = GetMonBaseStat_HandleAlternateForm(species, iVar4, 6);
    ov18_021F21FC(pokedexApp, a3, uVar1 & 0xffff);
    iVar2 = a3 * 4;
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + iVar2), 1);
    uVar3 = GetMonBaseStat_HandleAlternateForm(species, iVar4, 7);
    if (!((uVar3 != 0) && ((uVar1 & 0xffff) != uVar3))) {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + iVar2 + 4), 0);
        return;
    }
    ov18_021F21FC(pokedexApp, a3 + 1, uVar3);
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + iVar2 + 4), 1);
    return;
}
#endif
