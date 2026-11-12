// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/nsbtx_to_clwk.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_NSBTX_TO_CLWK_H
#define BWPORT_SYSTEM_NSBTX_TO_CLWK_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

// pokebw2's NsbtxToClwk_LoadTextureEx without its fade and palette (its NsbtxToClwk_LoadTexture's parameters)
// Loads a texture of a model file, width by height tiles, into the actor's characters and palette
void func_ov194_021f51e4(ClActor *actor, u32 arcId, u32 fileId, u8 texture, u16 width, u16 height, u32 offset,
                         u32 vramType, HeapID heapId);

#endif // BWPORT_SYSTEM_NSBTX_TO_CLWK_H
