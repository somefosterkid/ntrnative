#include "global.h"
#include "billboard.h"

void *sub_0202443C(const BillboardResources *resources, int value);

NNSG3dResMdlSet *sub_02024328(const BillboardResources *resources, NNSG3dResMdl **outModel, NNSG3dResTex **outTexture);

NNSG3dResMdlSet *sub_02024328(const BillboardResources *resources, NNSG3dResMdl **outModel, NNSG3dResTex **outTexture) {
    void *modelRes = sub_0202443C(resources, BILLBOARD_RESOURCE_MODEL_RES);
    NNSG3dResMdlSet *outModelSet = NNS_G3dGetMdlSet(modelRes);

    *outModel = NNS_G3dGetMdlByIdx(outModelSet, 0);

    if (outTexture) {
        *outTexture = NNS_G3dGetTex(modelRes);
    }

    return outModelSet;
}
