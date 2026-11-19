#include "global.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon_icon_idx.h"

#include "application/pokedex/pokedex_internal.h"

void *ov18_021F148C(PokedexAppData *, u32, u32, NNSG2dCharacterData **);

s32 ov18_021F14B4(PokedexAppData *, void *, s32, s32);

void ov18_021F14FC(PokedexAppData *arg0, u32 arg1, u32 arg2, s32 arg3);

void ov18_021F14FC(PokedexAppData *arg0, u32 species, u32 form, s32 arg3) {
    NNSG2dCharacterData *v1;
    s32 v0;
    void *ptr;

    v0 = arg3;
    ptr = ov18_021F148C(arg0, species, form, &v1);
    ov18_021F14B4(arg0, v1->pRawData, v0, GetBattleMonIconPaletteEx(species, form, 0));
    Heap_Free(ptr);
}
