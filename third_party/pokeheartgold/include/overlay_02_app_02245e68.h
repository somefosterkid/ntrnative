#ifndef POKEHEARTGOLD_OVERLAY_02_APP_02245E68_H
#define POKEHEARTGOLD_OVERLAY_02_APP_02245E68_H

#include "field/overlay_01_021FB878.h"

// A field 3D effect: one model, one render object, and a heap-allocated array of animations.
typedef struct Ov02App_02245E68 {
    Field3dModel model;
    Field3dObject object;
    Field3DModelAnimation *anims;
    NNSFndAllocator allocator;
    u8 animCount;
    u8 padding_9D;
    u16 animSpeed;
    enum HeapID heapID;
} Ov02App_02245E68;

// sizeof(Ov02App_02245E68) == 0xA4

// Description of one effect in the ov02 tables (the stride there is 8 or 12 bytes, the trailing bytes belong to the table).
typedef struct Ov02Field3dEffectDesc {
    u16 narcId;
    u8 fileId;
    u8 animCount;
    u16 *animIds;
} Ov02Field3dEffectDesc;

Ov02App_02245E68 *ov02_02245E04(const Ov02Field3dEffectDesc *desc, enum HeapID heapID);
void ov02_02245E68(Ov02App_02245E68 *effect);
void ov02_02245ED8(Ov02App_02245E68 *effect, NarcId narcId, u16 *animIds, u8 animCount);
s32 ov02_02245FD0(Ov02App_02245E68 *effect);

#endif // POKEHEARTGOLD_OVERLAY_02_APP_02245E68_H
