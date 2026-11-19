#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);
s32 ov18_021F12C8(PokedexAppData *, s32, s16 *, s16 *, s32);

void ov18_021F4FC8(PokedexAppData *arg0, s32 arg1, s32 arg2);

void ov18_021F4FC8(PokedexAppData *arg0, s32 arg1, s32 arg2) {
    s16 sp4[2];

    ov18_021F12C8(arg0, arg2, &sp4[1], sp4, 1);
    sp4[0] = (s16)(sp4[0] - 0x10);
    ov18_021F1294(arg0, arg1, sp4[1], sp4[0], 1);
}
