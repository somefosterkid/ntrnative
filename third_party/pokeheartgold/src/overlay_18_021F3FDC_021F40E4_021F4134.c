#include "global.h"

#include "application/pokedex/pokedex_internal.h"

void ov18_021F4134(PokedexAppData *pokedexApp) {
    s32 v0;
    s32 v1;

    if (pokedexApp->unk_18C8 == 0) {
        v0 = pokedexApp->args->x;
        if ((s32)((s32)(v0 + ((u32)(v0 >> 4) >> 0x1B)) >> 5) >= 0x17) {
            ov18_021F11C0(pokedexApp, 2, 0);
            return;
        }
        ov18_021F11C0(pokedexApp, 2, 1);
        return;
    }
    v1 = pokedexApp->args->x;
    if ((s32)((s32)(v1 + ((u32)(v1 >> 4) >> 0x1B)) >> 5) >= 0x17) {
        ov18_021F11C0(pokedexApp, 2, 1);
        return;
    }
    ov18_021F11C0(pokedexApp, 2, 0);
}
