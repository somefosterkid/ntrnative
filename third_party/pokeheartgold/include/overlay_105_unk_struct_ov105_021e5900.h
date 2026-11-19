#ifndef POKEHEARTGOLD_OVERLAY_105_UNK_STRUCT_OV105_021E5900_H
#define POKEHEARTGOLD_OVERLAY_105_UNK_STRUCT_OV105_021E5900_H

#include "camera.h"
#include "heap.h"

typedef struct UnkStruct_ov105_Model {
    NNSG3dRenderObj renderObj;
    NNSG3dResMdl *model;
    NNSG3dResFileHeader *header;
    void *animFiles[4];
    NNSG3dAnmObj *animObjs[4];
} UnkStruct_ov105_Model;

typedef struct UnkStruct_ov105_021E5900 {
    Camera *camera;
    UnkStruct_ov105_Model models[1];
    NNSFndAllocator allocator;
    VecFx32 target;
    u8 unk9c;
    u8 unk9d;
    u8 unk9e;
    u8 unk9f;
    u8 unkA0;
    u8 unkA1;
    u8 pad_A2[2];
    const u32 *unkA4;
} UnkStruct_ov105_021E5900;

// sizeof(UnkStruct_ov105_021E5900) == 0xa8

#endif // POKEHEARTGOLD_OVERLAY_105_UNK_STRUCT_OV105_021E5900_H
