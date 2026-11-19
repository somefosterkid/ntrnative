#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F4620(PokedexAppData *pokedexApp) {
    s32 a1;

    a1 = 9;
    do {
        ov18_021F11C0(pokedexApp, a1, 0);
        a1 += 1;
    } while ((u32)a1 < 0x3BU);
}
