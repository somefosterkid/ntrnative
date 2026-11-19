#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F34C4(PokedexAppData *, u32);

void ov18_021F34EC(PokedexAppData *, u32);

void ov18_021F3544(PokedexAppData *, u32);

void ov18_021F3560(PokedexAppData *, u32, u32, u32);

void ov18_021F3618(PokedexAppData *pokedexApp, int a1) {
    switch (a1) {
    case 0:
        ov18_021F34EC(pokedexApp, 1);
        ov18_021F3544(pokedexApp, 0);
        return;
    case 1:
        ov18_021F34EC(pokedexApp, 0);
        ov18_021F3544(pokedexApp, 0);
        return;
    case 2:
        ov18_021F34C4(pokedexApp, 1);
        ov18_021F3544(pokedexApp, 1);
        ov18_021F118C(pokedexApp, 5, 0x43);
        ov18_021F118C(pokedexApp, 0xb, 0x44);
        ov18_021F3560(pokedexApp, pokedexApp->dexSearchCriteria[4], pokedexApp->dexSearchCriteria[5], 0);
        return;
    case 3:
        ov18_021F34C4(pokedexApp, 1);
        ov18_021F3544(pokedexApp, 1);
        ov18_021F118C(pokedexApp, 5, 0x29);
        ov18_021F118C(pokedexApp, 0xb, 0x2a);
        ov18_021F3560(pokedexApp, pokedexApp->dexSearchCriteria[6], pokedexApp->dexSearchCriteria[7], 1);
        return;
    default:
        ov18_021F34C4(pokedexApp, 1);
        ov18_021F3544(pokedexApp, 0);
        return;
    }
}
