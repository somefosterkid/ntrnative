#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1324(PokedexAppData *, s32);

s32 ov18_021F17FC(PokedexAppData *);

s32 ov18_021F193C(PokedexAppData *);

s32 ov18_021F5E0C(PokedexAppData *);

void ov18_021F5DC0(PokedexAppData *pokedexApp) {
    ov18_021F1324(pokedexApp, 6);
    ov18_021F17FC(pokedexApp);
    ov18_021F193C(pokedexApp);
    ov18_021F5E0C(pokedexApp);
}

s32 ov18_021F10E8(PokedexAppData *, u32);

s32 ov18_021F13DC(PokedexAppData *, s32);

s32 ov18_021F18E0(PokedexAppData *);

s32 ov18_021F19EC(PokedexAppData *);

void ov18_021F5DE0(PokedexAppData *pokedexApp) {
    u32 v0;

    v0 = 1;
    do {
        ov18_021F10E8(pokedexApp, v0);
        v0 += 1;
    } while (v0 < 0x14U);
    ov18_021F13DC(pokedexApp, 6);
    ov18_021F18E0(pokedexApp);
    ov18_021F19EC(pokedexApp);
}
