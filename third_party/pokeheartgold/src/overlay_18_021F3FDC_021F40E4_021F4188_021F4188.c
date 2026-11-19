#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F1160(PokedexAppData *, s32, s32);

ManagedSprite *ov18_021F11EC(PokedexAppData *, ManagedSpriteTemplate *);

extern ManagedSpriteTemplate ov18_021FA4B8;

void ov18_021F4188(PokedexAppData *pokedexApp);

void ov18_021F4188(PokedexAppData *pokedexApp) {
    u32 i;

    for (i = 9; i < 0x3b; i++) {
        pokedexApp->unk_0670[i] = ov18_021F11EC(pokedexApp, &ov18_021FA4B8);
        ov18_021F1160(pokedexApp, i, 1);
        ov18_021F11C0(pokedexApp, i, 0);
    }
}
