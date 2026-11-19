#include "global.h"
#include "simple3dmodel.h"

#include "unk_02069660.h"

void sub_02069670(Simple3DModel *dest, NNSG3dResFileHeader *modelSetHeader, u32 modelIndex);

void sub_02069670(Simple3DModel *dest, NNSG3dResFileHeader *modelSetHeader, u32 modelIndex) {
    sub_02069660(dest);

    dest->modelSetHeader = modelSetHeader;
    dest->isTextureBound = 0;
    dest->modelSet = NNS_G3dGetMdlSet(modelSetHeader);
    dest->g3DModel = NNS_G3dGetMdlByIdx(dest->modelSet, modelIndex);
    dest->texture = NNS_G3dGetTex(modelSetHeader);
}
