#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);

void ov18_021F40E4(PokedexAppData *arg0);

void ov18_021F40E4(PokedexAppData *arg0) {
    u16 px = arg0->args->x / 32;
    u16 py = arg0->args->y / 32;
    if (px >= 23) {
        px -= 22;
    }
    ov18_021F1294(arg0, 2, px * 8 + 68, py * 8 + 44, 2);
    ov18_021F4134(arg0);
}
