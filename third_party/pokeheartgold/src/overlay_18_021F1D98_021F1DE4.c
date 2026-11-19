#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#include "heap.h"
#include "pokedex.h"
#include "sprite.h"
#include "sprite_system.h"
#include "wild_encounter.h"

s32 ov18_021F111C(PokedexAppData *, s32, u32, s32, s32);

u32 ov18_021F9694(u32, s32 arg1);

void ov18_021F1DE4(PokedexAppData *pokedexApp, u32 species, int a2, int a3) {
    PokedexAppData *var_r5;
    s32 var_r6;
    u32 var_r4;

    var_r5 = pokedexApp;
    var_r4 = species;
    var_r6 = a3;
    if ((var_r5->unk_185C != 2) || (var_r4 == 0) || (var_r5->unk_1030[a2].unk_2 == 1) || ((var_r4 == 0x1E7) && (Pokedex_GetSeenFormByIdx(var_r5->args->pokedex, (s32)species, 0) == 1))) {
        ManagedSprite_SetDrawFlag(var_r5->unk_0670[var_r6], 0);
        return;
    }
    ManagedSprite_SetDrawFlag(var_r5->unk_0670[var_r6], 1);
    var_r4 = ov18_021F9694(var_r4, 37);
    ov18_021F111C(var_r5, var_r6, var_r4, 0x80, 2);
    Heap_Free((void *)var_r4);
}
