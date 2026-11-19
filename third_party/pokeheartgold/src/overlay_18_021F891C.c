#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "pokedex_util.h"


u32 ov18_021F8970(u32 x);

u16 ov18_021F891C(PokedexAppData *pokedexApp, BOOL a1) {
    u32 uVar1;
    if (a1 == 0) {
        return pokedexApp->unk_0878.unk_7B4;
    }
    uVar1 = Pokedex_ConvertToCurrentDexNo((u32)pokedexApp->unk_1858, (u32)pokedexApp->unk_0878.unk_000[pokedexApp->unk_0878.unk_7B4 - 1][0]);
#if defined(SDK_PORT) || defined(NONMATCHING)
    return uVar1; /* r0 on DS = result of Pokedex_ConvertToCurrentDexNo() */
#endif
}

u32 ov18_021F8950(PokedexAppData *pokedexApp, int a1) {
    if (a1 == 0) {
        return ov18_021F891C(pokedexApp, a1) / 15U;
    }
    return (u32)(ov18_021F891C(pokedexApp, a1) - 1) / 15U;
}

u32 ov18_021F8970(u32 x) {
    return x; // the asm is a bare bx lr: callers use r0, the argument, as the result
}
