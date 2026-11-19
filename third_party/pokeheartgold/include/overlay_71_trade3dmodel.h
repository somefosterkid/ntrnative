#ifndef POKEHEARTGOLD_OVERLAY_71_TRADE3DMODEL_H
#define POKEHEARTGOLD_OVERLAY_71_TRADE3DMODEL_H

// Trade3DModel: merged by tools/hg/promote.py from 17 m2c local copies (1 variants, base
// src/overlay_71_02246B58_022473F0_02247424.c) and 1 accesses; fields at every accessed offset, pads elsewhere.

#include "global.h"

typedef struct Trade3DModel {
    void *modelData;
    NNSG3dRenderObj renderObj;
    NNSG3dResMdlSet *modelSet;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
    BOOL enabled;
    VecFx32 position;
    VecFx32 scale;
    VecFx16 rotation;
    u8 pad_86[0x2];
    int alpha;
} Trade3DModel;

// sizeof(Trade3DModel) == 0x8c

#endif // POKEHEARTGOLD_OVERLAY_71_TRADE3DMODEL_H
