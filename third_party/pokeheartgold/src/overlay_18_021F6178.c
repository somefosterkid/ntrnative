#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F121C(PokedexAppData *, s32, s16, u32, s32);

s32 ov18_021F61F8(PokedexAppData *, s32, void *, s32);
void ov18_021F61DC(PokedexAppData *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4);

void ov18_021F6178(PokedexAppData *pokedexApp, int a1) {
    u32 v0;

    v0 = 0;
    do {
        ov18_021F121C(pokedexApp, v0 + 0xE, 0, a1, 0);
        v0 += 1;
    } while (v0 < 6U);
}

void ov18_021F619C(PokedexAppData *pokedexApp, s8 a1, s8 a2, int a3) {
    if (a1 == 0) {
        ov18_021F118C(pokedexApp, a3, 7);
    } else {
        ov18_021F118C(pokedexApp, a3, 5);
    }
    if (a1 == (a2 - 1)) {
        ov18_021F118C(pokedexApp, a3 + 1, 0xA);
        return;
    }
    ov18_021F118C(pokedexApp, a3 + 1, 8);
}

void ov18_021F61DC(PokedexAppData *pokedexApp, s32 a1, s32 arg2, void *arg3, s32 arg4) {
    ov18_021F118C(pokedexApp, a1, ov18_021F61F8(pokedexApp, arg2, arg3, arg4));
}
