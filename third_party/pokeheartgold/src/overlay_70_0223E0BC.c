#include "global.h"

#include "heap.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "sprite.h"
#include "overlay_70_gtsapplication_state.h"

void *ov70_0223E094(NARC *, u32, void **, s32);

void ov70_0223E0BC(u32 arg0, u32 arg1, s32 arg2, s32 arg3, Sprite *arg4, void *arg5, PokemonIcon *arg6);

void ov70_0223E120(GTSApplicationState *param_1);

void ov70_0223E0BC(u32 arg0, u32 form, s32 isEgg, s32 arg3, Sprite *arg4, void *arg5, PokemonIcon *arg6) {
    void *v2;
    void *v1;
    u32 v0;
    void *ptr;

    v0 = arg0;
    ptr = ov70_0223E094(arg5, GetMonIconNaixEx(arg0, isEgg, form), &v2, 0x3D);
    v1 = ptr;
    MIi_CpuCopyFast(*(void **)((u8 *)v2 + 0x14), arg6->vramOffset, 0x200U);
    arg6->tileData = (s32)(((arg3 * 0x10) + 0xC) << 5);
    arg6->sprite = arg4;
    arg6->paletteIndex = (s32)(GetMonIconPaletteEx(v0, form, (u32)isEgg) + 3);
    Heap_Free(ptr);
}

void ov70_0223E114(BoxPokemon *boxMon, GTSPokemonCriteria *criteria);

void ov70_0223E114(BoxPokemon *boxMon, GTSPokemonCriteria *criteria) {
    criteria->level = CalcBoxMonLevel(boxMon);
}

void ov70_0223E120(GTSApplicationState *param_1) {
    s32 iVar1;
    PokemonIcon *puVar2;

    puVar2 = param_1->selectPokemonIconHeapPtr;
    iVar1 = 0;
    do {
        if (puVar2->sprite != NULL) {
            DC_FlushRange(puVar2->vramOffset, 0x200);
            GX_LoadOBJ(puVar2->vramOffset, puVar2->tileData, 0x200);
            Sprite_SetPaletteOverride(puVar2->sprite, puVar2->paletteIndex);
        }
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (iVar1 < 0x1e);
    Heap_Free(param_1->selectPokemonIconHeapPtr);
    return;
}
