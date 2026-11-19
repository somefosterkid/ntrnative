#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_96_sprite_res_man.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

void *ov96_021E8A20(void *ptr);

void *ov96_021E8A20(void *ptr) {
    return ptr;
}

extern s32 ov96_0221AEC4;

s32 ov96_021E8A24(void);

s32 ov96_021E8A24(void) {
    return (s32)&ov96_0221AEC4;
}

s32 ov96_021E8A2C(void);

s32 ov96_021E8A2C(void) {
    return 0xC;
}

Ov96SpriteResMan *ov96_021E8A30(enum HeapID heapID) {
    Ov96SpriteResMan *man;

    man = Heap_Alloc(heapID, sizeof(Ov96SpriteResMan));
    man->heapID = heapID;
    return man;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021E8A44(Ov96SpriteResMan *man, u32 maxEntries, SpriteList *spriteList) {
    GfGfxResType type;

    if (maxEntries > 0x10U) {
        GF_AssertFail();
    }
    man->maxEntries = maxEntries;
    man->numLoaded = 0;
    man->unk_C = 0;
    man->mode = 0;
    man->entries = Heap_Alloc(man->heapID, sizeof(Ov96SpriteResEntry) * maxEntries);
    if (spriteList == NULL) {
        man->ownedSpriteList = G2dRenderer_Init(man->maxEntries, &man->renderer, man->heapID);
        man->spriteList = man->ownedSpriteList;
        man->ownsSpriteList = TRUE;
    } else {
        man->spriteList = spriteList;
        man->ownsSpriteList = FALSE;
    }
    type = GF_GFX_RES_TYPE_CHAR;
    do {
        man->resMan[type] = Create2DGfxResObjMan(man->maxEntries, type, man->heapID);
        type++;
    } while (type < 4);
}
#else
void ov96_021E8A44(Ov96SpriteResMan *man, u32 maxEntries, SpriteList *list) {
    void *arg0 = man;
    u32 arg1 = maxEntries;
    SpriteList *arg2 = list;

    SpriteList *spriteList;
    GfGfxResType type;
    s32 v0;
    void *v1;

    if (arg1 > 0x10U) {
        GF_AssertFail();
    }
    *(u16 *)((u8 *)arg0 + 4) = (u16)arg1;
    *(s16 *)((u8 *)arg0 + 6) = 0;
    *(s32 *)((u8 *)arg0 + 0xC) = 0;
    *(s32 *)((u8 *)arg0 + 8) = 0;
    *(void **)((u8 *)arg0 + 0x154) = Heap_Alloc(*(enum HeapID *)arg0, 0x2C * arg1);
    if (arg2 == NULL) {
        spriteList = G2dRenderer_Init((s32) * (u16 *)((u8 *)arg0 + 4), arg0 + 0x1C, *(enum HeapID *)arg0);
        *(SpriteList **)((u8 *)arg0 + 0x14) = spriteList;
        *(SpriteList **)((u8 *)arg0 + 0x18) = spriteList;
        v0 = 1;
    } else {
        *(SpriteList **)((u8 *)arg0 + 0x18) = arg2;
        v0 = 0;
    }
    *(s32 *)((u8 *)arg0 + 0x10) = v0;
    type = GF_GFX_RES_TYPE_CHAR;
    v1 = arg0;
    do {
        *(GF_2DGfxResMan **)((u8 *)v1 + 0x144) = Create2DGfxResObjMan((s32) * (u16 *)((u8 *)arg0 + 4), type, *(enum HeapID *)arg0);
        type += 1;
        v1 += 4;
    } while (type < 4);
}
#endif
