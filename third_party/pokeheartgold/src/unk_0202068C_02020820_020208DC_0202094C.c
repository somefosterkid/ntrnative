#include "global.h"

int sub_0202094C(NNSG3dResTex *texture, const NNSG3dResDictTexData *textureData);

int sub_0202094C(NNSG3dResTex *texture, const NNSG3dResDictTexData *textureData) {
    u32 textureFormat = (textureData->texImageParam & NNS_G3D_TEXIMAGE_PARAM_TEXFMT_MASK) >> NNS_G3D_TEXIMAGE_PARAM_TEXFMT_SHIFT;
    u32 texelSize;

    switch (textureFormat) {
    case GX_TEXFMT_PLTT4:
        texelSize = 4;
        break;
    case GX_TEXFMT_PLTT16:
        texelSize = 2;
        break;
    case GX_TEXFMT_PLTT256:
        texelSize = 1;
        break;
    case GX_TEXFMT_A3I5:
        texelSize = 1;
        break;
    case GX_TEXFMT_A5I3:
        texelSize = 1;
        break;
    default:
        return 0;
    }

    u32 sSize = (textureData->texImageParam & NNS_G3D_TEXIMAGE_PARAM_S_SIZE_MASK) >> NNS_G3D_TEXIMAGE_PARAM_S_SIZE_SHIFT;
    sSize <<= 4;

    u32 tSize = (textureData->texImageParam & NNS_G3D_TEXIMAGE_PARAM_T_SIZE_MASK) >> NNS_G3D_TEXIMAGE_PARAM_T_SIZE_SHIFT;
    tSize <<= 4;

    return (sSize * tSize) / texelSize;
}
