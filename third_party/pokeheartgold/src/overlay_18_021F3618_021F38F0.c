#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F38F0(PokedexAppData *pokedexApp, int a1, u16 a2) {
    u32 v0;
    u32 v1;
    u32 v2;

    if (a2 == 0x3E7) {
        v2 = 0x4A4;
    } else {
        v2 = (u32)(((u32)(0x2710 * a2) / 254U) + 5) / 10U;
    }
    v0 = v2 / 12U;
    v1 = v2 % 12U;
    if (v0 >= 0xAU) {
        ov18_021F118C(pokedexApp, a1, (v0 / 10U) + 0x2B);
        ov18_021F11C0(pokedexApp, a1, 1);
    } else {
        ov18_021F11C0(pokedexApp, a1, 0);
    }
    ov18_021F118C(pokedexApp, a1 + 1, (v0 % 10U) + 0x2B);
    ov18_021F11C0(pokedexApp, a1 + 1, 1);
    ov18_021F118C(pokedexApp, a1 + 2, (v1 / 10U) + 0x2B);
    ov18_021F11C0(pokedexApp, a1 + 2, 1);
    ov18_021F118C(pokedexApp, a1 + 3, (v1 % 10U) + 0x2B);
    ov18_021F11C0(pokedexApp, a1 + 3, 1);
    ov18_021F11C0(pokedexApp, a1 + 4, 0);
}
