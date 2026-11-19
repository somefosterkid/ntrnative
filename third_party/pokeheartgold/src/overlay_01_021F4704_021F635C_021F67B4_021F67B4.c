#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "gf_3d_render.h"

NNSG3dResMdl *ov01_021F67B4(NARC *landDataNARC, const int mapModelFileSize, NNSG3dRenderObj *mapRenderObj, NNSG3dResFileHeader **mapModelFile, NNSG3dResTex *mapTexture);

NNSG3dResMdl *ov01_021F67B4(NARC *landDataNARC, const int mapModelFileSize, NNSG3dRenderObj *mapRenderObj, NNSG3dResFileHeader **mapModelFile, NNSG3dResTex *mapTexture) {
    NARC_ReadFile(landDataNARC, mapModelFileSize, *mapModelFile);

    if (mapTexture != NULL) {
        if (GF3dRender_ResTexIsLoaded(mapTexture) == TRUE) {
            BOOL textureBound = GF3dRender_BindModelSet(*mapModelFile, mapTexture);
            GF_ASSERT(textureBound);
        }
    }

    NNSG3dResMdlSet *mapResMdlSet = NNS_G3dGetMdlSet(*mapModelFile);

    GF_ASSERT(mapResMdlSet->dict.numEntry == 1);
    NNSG3dResMdl *mapModel = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(*mapModelFile), 0);

    GF_ASSERT(mapModel);
    NNS_G3dRenderObjInit(mapRenderObj, mapModel);

    return mapModel;
}
