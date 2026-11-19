#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F10E8(PokedexAppData *, u32);

s32 ov18_021F4CC4(PokedexAppData *);

void ov18_021F4A50(PokedexAppData *pokedexApp) {
    u32 v0;

    v0 = 1;
    do {
        ov18_021F10E8(pokedexApp, v0);
        v0 += 1;
    } while (v0 < 0xAU);
    ov18_021F4CC4(pokedexApp);
}
