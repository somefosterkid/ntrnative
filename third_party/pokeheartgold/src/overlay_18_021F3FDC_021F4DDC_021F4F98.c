#include "global.h"

#include "application/pokedex/pokedex_internal.h"

s32 ov18_021F1294(PokedexAppData *, s32, s16, s16, s32);
s32 ov18_021F4EB0(s32, s32 *, s32 *);

void ov18_021F4F98(PokedexAppData *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void ov18_021F4F98(PokedexAppData *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    ov18_021F4EB0(arg2, &arg3, &arg4);
    ov18_021F1294(arg0, arg1, *((s16 *)&arg3), *((s16 *)&arg4), 1);
}
