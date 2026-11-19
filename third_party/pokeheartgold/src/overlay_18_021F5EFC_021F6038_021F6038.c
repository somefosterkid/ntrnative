#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);

s32 ov18_021F5FFC(PokedexAppData *, s32, u32);

void ov18_021F6038(PokedexAppData *pokedexApp) {
    u32 v2;
    u32 v0;
    u32 v1;

    v1 = 0;
    v2 = 0xE;
    do {
        v0 = (pokedexApp->unk_18C5 + v1) - 2;
        if (v0 >= (u32)pokedexApp->numSeenForms) {
            ov18_021F11C0(pokedexApp, v1 + 0xE, 0);
        } else {
            ov18_021F11C0(pokedexApp, v1 + 0xE, 1);
            ov18_021F5FFC(pokedexApp, v1 + 0xE, v0);
        }
        ov18_021F1294(pokedexApp, v1 + 0xE, 0x30, v2, 0);
        v1 += 1;
        v2 += 0x18;
    } while (v1 < 6U);
}
