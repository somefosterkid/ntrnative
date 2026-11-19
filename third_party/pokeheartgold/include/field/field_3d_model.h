#ifndef POKEHEARTGOLD_FIELD_FIELD_3D_MODEL_H
#define POKEHEARTGOLD_FIELD_FIELD_3D_MODEL_H

#include "global.h"

// The field's 3D model / animation / object records (the functions are in field/overlay_01_021FB878.h). Kept apart from
// that header so a file that only holds them as members does not pull in area_data.h and its types.

typedef struct Field3dModel {
    void *dataRaw;
    NNSG3dResMdlSet *modelSet;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} Field3dModel;

typedef struct Field3DModelAnimation {
    void *fileDataRaw;
    void *animRaw;
    NNSG3dAnmObj *animObj;
    fx32 frame;
    BOOL shouldRetainData; // if FALSE, fileDataRaw is freed on unload
} Field3DModelAnimation;

typedef struct Field3dObject {
    NNSG3dRenderObj renderObj;
    VecFx32 matrix;
    VecFx32 scale;
    BOOL active;
    u16 rotation[3];
    u16 padding;
} Field3dObject;

#endif // POKEHEARTGOLD_FIELD_FIELD_3D_MODEL_H
