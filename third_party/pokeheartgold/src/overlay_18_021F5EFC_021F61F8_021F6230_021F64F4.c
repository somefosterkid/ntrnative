#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F6230(PokedexAppData *, s32, s32 *, s32, s32);

s32 ov18_021F6244(PokedexAppData *, s32, s32 *, s32, s32);

extern s32 ov18_021FA304;

int ov18_021F64F4(PokedexAppData *pokedexApp, int a1) {
    s32 v1;
    s32 v2;
    u32 v0;
    int new_var;
    v1 = ov18_021F6230(pokedexApp, 5, &ov18_021FA304, 0x60, 0x56);
    v2 = ov18_021F6244(pokedexApp, 5, &ov18_021FA304, 0x60, 0x56);
    v0 = pokedexApp->unk_18DC.unk_20.nMaps - 1;
    if (a1 != v0) {
        v2 = (((u32)((((u32)((v2 - v1) << 8)) / v0) * (new_var = a1))) >> 8) + v1;
    }
    return v2;
}
