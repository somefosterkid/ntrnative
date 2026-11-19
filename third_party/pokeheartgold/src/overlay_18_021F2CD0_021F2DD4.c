#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F2CB4(PokedexAppData *, s32);

s32 ov18_021F2CC0(PokedexAppData *, s32);

int ov18_021F2DD4(PokedexAppData *pokedexApp, int a1, int a2, int a3) {
    s32 start;
    s32 end;
    u32 total;

    start = ov18_021F2CB4(pokedexApp, a2);
    end = ov18_021F2CC0(pokedexApp, a2);
    total = ov18_021F8950(pokedexApp, a3);
    if (a1 != total) {
        end = (u32)((end - start) << 8) / total;
        end = start + ((u32)(end * a1) >> 8);
    }
    return end;
}
