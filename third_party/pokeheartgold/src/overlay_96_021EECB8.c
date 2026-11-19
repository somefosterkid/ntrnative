#include "global.h"

#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon_icon_idx.h"
#include "sprite_system.h"

s32 ov96_021EED14(Sprite *, void *, s32, s32);

void ov96_021EECB8(ManagedSprite *arg0, void *arg1, s32 arg2, NARC *arg3, s32 arg4, enum HeapID arg5);

void ov96_021EECB8(ManagedSprite *managedSprite, void *arg1, s32 arg2, NARC *narc, s32 arg4, enum HeapID heapID) {
    NNSG2dCharacterData *ppCharData;
    void *ptr;

    ptr = GfGfxLoader_GetCharDataFromOpenNarc(narc, (s32)GetMonIconNaixEx((u32) * (u16 *)arg1, 0, (u32) * (u16 *)((u8 *)arg1 + 2)), 0, &ppCharData, heapID);
    if (ptr == NULL) {
        GF_AssertFail();
    }
    ov96_021EED14(managedSprite->sprite, ppCharData->pRawData, 0x200, arg4);
    Heap_Free(ptr);
    ManagedSprite_SetPaletteOverride(managedSprite, arg2 + GetMonIconPaletteEx((u32) * (u16 *)arg1, (u32) * (u16 *)((u8 *)arg1 + 2), 0U));
}
