#ifndef BWPORT_FIELD_FIELD_FLASH_H
#define BWPORT_FIELD_FIELD_FLASH_H

// field_flash.c and ov021_021c7de0.c: the darkness of a dark cave around the player, a sphere that Flash widens and
// fades. The types are pokebw2's (GPL-3.0, https://github.com/fuddlesworth/pokebw2, src/ov036/field_flash.c), with
// Black's layout, which is the same

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"

// fld_wipe_3dobj.c's sphere model, which it frees and draws at a scale with an alpha
typedef struct FieldFlashG3D FieldFlashG3D;
void FieldFlashG3D_Free(FieldFlashG3D *g3d);
void FieldFlashG3D_Draw(FieldFlashG3D *g3d, fx32 scale, u32 alpha);

typedef struct FieldFlashAnimation {
    fx32 scale;
    u8 alpha;
    u16 frame;
    u16 frameCount;
    fx32 scaleStart;
    fx32 scaleDelta;
    s16 alphaStart;
    s16 alphaDelta;
} FieldFlashAnimation;

typedef struct FieldFlash {
    FieldFlashG3D *g3d;
    FieldFlashAnimation animation;
    // 0 dark, 1 widening as Flash is used, 2 lit, 3 off; nextState is 4 when no change is asked for
    u16 state;
    u16 nextState;
} FieldFlash;

FieldFlash *FieldFlash_Create(HeapID heapId);

#endif // BWPORT_FIELD_FIELD_FLASH_H
