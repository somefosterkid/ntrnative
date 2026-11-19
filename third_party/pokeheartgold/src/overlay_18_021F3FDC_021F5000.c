#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F4F98(PokedexAppData *, s32, s32, s32, s32);
s32 ov18_021F4FC8(PokedexAppData *, s32, s32);

void ov18_021F5000(PokedexAppData *pokedexApp, s32 arg1) {
    ov18_021F4F98(pokedexApp, 8, (u16)(0xFFFFC000 - arg1), 0x80, 0x68);
    ov18_021F4F98(pokedexApp, 9, (u16)(0x4000 - arg1), 0x80, 0x68);
    ov18_021F4FC8(pokedexApp, 2, 8);
    ov18_021F4FC8(pokedexApp, 4, 9);
}
