// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/bmp_oam.h, the lines in use so far.
#ifndef BWPORT_SYSTEM_BMP_OAM_H
#define BWPORT_SYSTEM_BMP_OAM_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Bitmaps shown as OAM sprites (bmp_oam.c): a bitmap is cut into 32x16 cell actors whose characters are uploaded from
// it. pokebw2's guessed names are in the comments

// pokebw2's BmpOam_Init and BmpOam_Exit
BmpOamSys *func_02025538(HeapID heapId, ClActUnit *unit);
void func_02025588(BmpOamSys *sys);

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

// pokebw2's BmpOam_ActorAdd
BmpOamActor *func_020255a0(BmpOamSys *sys, const BmpOamActorSetup *setup);
// pokebw2's BmpOam_ActorSetDrawEnable
void func_02025774(BmpOamActor *actor, BOOL enable);
// pokebw2's BmpOam_ActorDel
void func_0202570c(BmpOamActor *actor);

// pokebw2's BmpOam_ActorDel
// pokebw2's BmpOam_ActorAdd
// pokebw2's BmpOam_ActorSetObjMode
// GX_OAM_MODE_*
void func_0202597c(BmpOamActor *actor, u32 mode);

// pokebw2's BmpOam_ActorGetPos
void func_020258e8(BmpOamActor *actor, s16 *x, s16 *y);
// pokebw2's BmpOam_ActorSetPaletteOffset
void func_02025a54(BmpOamActor *actor, u32 offset);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BmpOam_ActorBmpTrans
// Uploads the bitmap's pixels to the actors' characters
void func_020257d0(BmpOamActor *actor);
// pokebw2's BmpOam_ActorGetDrawEnable
BOOL func_020257c4(BmpOamActor *actor);
// pokebw2's BmpOam_ActorSetPriority
void func_020259c4(BmpOamActor *actor, u8 priority);
// pokebw2's BmpOam_ActorSetBgPriority
void func_02025a0c(BmpOamActor *actor, u32 bgPriority);

// pokebw2's BmpOam_ActorSetPos
void func_0202590c(BmpOamActor *actor, s16 x, s16 y);
// pokebw2's BmpOam_ActorDel
// pokebw2's BmpOam_ActorAdd

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BmpOam_ActorBmpTrans
// Uploads the bitmap's pixels to the actors' characters

// pokebw2's BmpOam_ActorSetPos

#endif // BWPORT_SYSTEM_BMP_OAM_H
