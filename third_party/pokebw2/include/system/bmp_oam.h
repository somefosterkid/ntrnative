#ifndef POKEBW2_SYSTEM_BMP_OAM_H
#define POKEBW2_SYSTEM_BMP_OAM_H

#include "types.h"
#include "gfl/bmp.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Bitmaps shown as OAM sprites (bmp_oam.c): a bitmap is cut into 32x16 cell actors whose characters are uploaded from
// it. Our names; swan has none for this file

typedef struct {
    GFLBitmap *bitmap;
    s16 x;
    s16 y;
    u32 palette;
    u32 paletteOffset;
    u8 priority;
    u8 bgPriority;
    u16 surface;
    // CLACT_VRAM_MAIN or CLACT_VRAM_SUB
    u32 vramType;
} BmpOamActorSetup;

BmpOamSys *BmpOam_Init(HeapID heapId, ClActUnit *unit);
void BmpOam_Exit(BmpOamSys *sys);
BmpOamActor *BmpOam_ActorAdd(BmpOamSys *sys, const BmpOamActorSetup *setup);
void BmpOam_ActorDel(BmpOamActor *actor);
void BmpOam_ActorSetDrawEnable(BmpOamActor *actor, BOOL enable);
BOOL BmpOam_ActorGetDrawEnable(BmpOamActor *actor);
// Uploads the bitmap's pixels to the actors' characters
void BmpOam_ActorBmpTrans(BmpOamActor *actor);
void BmpOam_ActorGetPos(BmpOamActor *actor, s16 *x, s16 *y);
void BmpOam_ActorSetPos(BmpOamActor *actor, s16 x, s16 y);
// GX_OAM_MODE_*
void BmpOam_ActorSetObjMode(BmpOamActor *actor, u32 mode);
void BmpOam_ActorSetPriority(BmpOamActor *actor, u8 priority);
void BmpOam_ActorSetBgPriority(BmpOamActor *actor, u32 bgPriority);
void BmpOam_ActorSetPaletteOffset(BmpOamActor *actor, u32 offset);

#endif // POKEBW2_SYSTEM_BMP_OAM_H
