#include "global.h"

#include "application/pokedex/pokedex_internal.h"


void ov18_021F3B60(PokedexAppData *pokedexApp, int a1) {
    s32 v0;
    s32 v1;

    if (a1 == 1) {
        v0 = 0x2C;
        do {
            ov18_021F11C0(pokedexApp, v0, 1);
            v0 += 1;
        } while ((u32)v0 <= 0x3AU);
        ov18_021F11C0(pokedexApp, 0x2B, 0);
        return;
    }
    v1 = 0x2C;
    do {
        ov18_021F11C0(pokedexApp, v1, 0);
        v1 += 1;
    } while ((u32)v1 <= 0x3AU);
    ov18_021F3BA4(pokedexApp);
}

void ov18_021F3BA4(PokedexAppData *pokedexApp) {
    if (pokedexApp->dexSearchCriteria[9] == 0xE) {
        ov18_021F11C0(pokedexApp, 0x2B, 0);
        return;
    }
    ov18_021F11C0(pokedexApp, 0x2B, 1);
    ov18_021F118C(pokedexApp, 0x2B, (s32)pokedexApp->dexSearchCriteria[9]);
}
