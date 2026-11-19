#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "pokedex.h"
#include "sprite.h"
#include "sprite_system.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov18_021F2A2C(PokedexAppData *pokedexApp, int a1, int a2) {
    BOOL BVar1;

    if ((a2 == 1) && (BVar1 = Pokedex_GetInternationalViewFlag(pokedexApp->args->pokedex), BVar1 == 1)) {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a1], 1);
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a1 + 1], 1);
        return;
    }
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a1], 0);
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a1 + 1], 0);
    return;
}
#else
void ov18_021F2A2C(PokedexAppData *pokedexApp, int a1, int a2) {
    BOOL BVar1;

    if ((a2 == 1) && (BVar1 = Pokedex_GetInternationalViewFlag(pokedexApp->args->pokedex), BVar1 == 1)) {
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + a1 * 4), 1);
        ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + a1 * 4 + 4), 1);
        return;
    }
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + a1 * 4), 0);
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)(pokedexApp->filler_0770 + a1 * 4 + 4), 0);
    return;
}
#endif

void ov18_021F2A84(PokedexAppData *pokedexApp, int a1, int a2) {
    BOOL BVar1;

    if ((a2 == 1) && (BVar1 = Pokedex_GetInternationalViewFlag(pokedexApp->args->pokedex), BVar1 == 1)) {
        ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a1], 1);
        return;
    }
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[a1], 0);
    return;
}

void ov18_021F2AC0(PokedexAppData *pokedexApp, int a1) {
    if (pokedexApp->unk_1858 == 0) {
        ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a1], 0x90, 0x80);
        return;
    }
    ManagedSprite_SetPositionXY(pokedexApp->unk_0670[a1], 0x70, 0x80);
    return;
}
