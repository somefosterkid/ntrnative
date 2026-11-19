#ifndef POKEHEARTGOLD_SIMPLE3DMODEL_H
#define POKEHEARTGOLD_SIMPLE3DMODEL_H

#include "global.h"

// A 3D model made from one resource file (ov01_021F19F4) and the render object drawing it.
typedef struct Simple3DModel {
    BOOL isTextureBound;
    NNSG3dResFileHeader *modelSetHeader;
    NNSG3dResMdlSet *modelSet;
    NNSG3dResMdl *g3DModel;
    NNSG3dResTex *texture;
} Simple3DModel; // 0x14 bytes at 32 bits

typedef struct Simple3DRenderObj {
    NNSG3dRenderObj g3DRenderObj;
} Simple3DRenderObj;

#endif // POKEHEARTGOLD_SIMPLE3DMODEL_H
