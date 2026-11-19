#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "sprite.h"
#include "sprite_system.h"

void ov18_021F2BFC(PokedexAppData *pokedexApp) {
    ManagedSprite_SetDrawFlag(pokedexApp->unk_0670[5], 0);
}

void ov18_021F2C10(PokedexAppData *pokedexApp, int a1, int a2) {
    if (pokedexApp->unk_1859 == 0) {
        ov18_021F118C(pokedexApp, a1, 7);
    } else {
        ov18_021F118C(pokedexApp, a1, 5);
    }
    if (pokedexApp->unk_1859 == ov18_021F8950(pokedexApp, a2)) {
        ov18_021F118C(pokedexApp, a1 + 1, 0xA);
        return;
    }
    ov18_021F118C(pokedexApp, a1 + 1, 8);
}

s32 ov18_021F2C74(PokedexAppData *, s32);

void ov18_021F2C5C(PokedexAppData *pokedexApp, int a1, int a2) {
    ov18_021F118C(pokedexApp, a1, ov18_021F2C74(pokedexApp, a2));
}
