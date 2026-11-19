#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F3A64(PokedexAppData *pokedexApp, u32 a1, int a2) {
    if (a1 == 0) {
        ov18_021F118C(pokedexApp, a2, 0x3A);
        ov18_021F118C(pokedexApp, a2 + 1, 0x35);
        return;
    }
    if (a1 == 0x98) {
        ov18_021F118C(pokedexApp, a2, 0x38);
        ov18_021F118C(pokedexApp, a2 + 1, 0x37);
        return;
    }
    ov18_021F118C(pokedexApp, a2, 0x38);
    ov18_021F118C(pokedexApp, a2 + 1, 0x35);
}
