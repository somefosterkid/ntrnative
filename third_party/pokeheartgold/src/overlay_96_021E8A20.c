#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "comm_cmd_manager.h"
#include "pokeathlon/pokeathlon.h"
#include "heap.h"
#include "overlay_96_sprite_res_man.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "pokeathlon/pokeathlon.h"

void *ov96_021E8A20(void *ptr);

void *ov96_021E8A20(void *ptr) {
    return ptr;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov96_021E96F8(int, int, void *, void *);
void ov96_021E9718(int, int, void *, void *);
void ov96_021E9784(int, int, void *, void *);
void ov96_021E97B8(int, int, void *, void *);
void ov96_021E97EC(int, int, void *, void *);
void ov96_021E9820(int, int, void *, void *);
void ov96_021E9858(int, int, void *, void *);
void ov96_021E9870(int, int, void *, void *);
void ov96_021E98D0(int, int, void *, void *);
void ov96_021E98F4(int, int, void *, void *);
void ov96_021E9994(int, int, void *, void *);
void ov96_021E99B8(int, int, void *, void *);
u32 ov96_021E99F4(void);
u32 ov96_021E99F8(void);
u32 ov96_021E99FC(void);
u32 ov96_021E9A04(void);
u32 ov96_021E9A10(void);
u32 ov96_021E9A14(void);
u32 ov96_021E9A18(void);
u32 ov96_021E9A1C(void);
u32 ov96_021E9A24(void);
u32 sub_020342B8(void);
void *ov96_021E9A2C(int, void *, int);
void *ov96_021E9A38(int, void *, int);
void *ov96_021E9A48(int, void *, int);
void *ov96_021E9A54(int, void *, int);
void *ov96_021E9A68(int, void *, int);

const CommCmdTable ov96_0221AEC4[] = {
    { ov96_021E96F8, ov96_021E99F4, NULL },
    { ov96_021E9718, ov96_021E99F8, NULL },
    { ov96_021E9784, ov96_021E99FC, NULL },
    { ov96_021E97B8, ov96_021E9A04, NULL },
    { ov96_021E97EC, ov96_021E9A10, ov96_021E9A38 },
    { ov96_021E9820, ov96_021E9A14, NULL },
    { ov96_021E9858, sub_020342B8, ov96_021E9A2C },
    { ov96_021E9870, ov96_021E9A14, NULL },
    { ov96_021E98D0, ov96_021E9A14, NULL },
    { ov96_021E98F4, ov96_021E9A18, ov96_021E9A54 },
    { ov96_021E9994, ov96_021E9A1C, ov96_021E9A48 },
    { ov96_021E99B8, ov96_021E9A24, ov96_021E9A68 },
};
#else
extern const CommCmdTable ov96_0221AEC4[];
#endif

const CommCmdTable *ov96_021E8A24(void) {
    return ov96_0221AEC4;
}

int ov96_021E8A2C(void) {
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
