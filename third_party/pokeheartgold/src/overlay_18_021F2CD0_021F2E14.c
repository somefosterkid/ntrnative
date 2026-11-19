#include "global.h"

#include "application/pokedex/pokedex_internal.h"

u32 ov18_021F2E14(PokedexAppData *pokedexApp, int a1, u32 a2, u32 a3) {
    s16 x;
    s16 y;

    ManagedSprite_GetPositionXY(pokedexApp->unk_0670[a1], &x, &y);
    if (a2 >= y) {
        return (a2 - y) / a3;
    }
    return (y - a2) / a3;
}
