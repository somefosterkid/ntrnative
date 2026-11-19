// NitroSystem's G3D binding functions that the Gen 4 games call and Black doesn't (g3d.h, "What the Gen 4 games
// call"), written from their behaviour next to kernel.c's. Not in Black's ROM, so the DS build doesn't list this file
#include "nnsys/g3d.h"

// What binding a texture leaves in a material's texImageParam besides the material's own repeat, flip and coordinate
// mode (GBATEK: 3D texture attributes): the VRAM offset, size, format and color 0 bit
#define TEXIMAGE_MATERIAL_BITS                                                                                         \
    (REG_G3_TEXIMAGE_PARAM_TGEN_MASK | REG_G3_TEXIMAGE_PARAM_FT_MASK | REG_G3_TEXIMAGE_PARAM_FS_MASK |                 \
     REG_G3_TEXIMAGE_PARAM_RT_MASK | REG_G3_TEXIMAGE_PARAM_RS_MASK)

static NNSG3dResDict *texToMatDict_(NNSG3dResMat *mat) {
    return (NNSG3dResDict *)((u8 *)mat + mat->ofsDictTexToMatList);
}

// As kernel.c's bindMdlTex_Internal_: the texture's place in VRAM into each material of the list, and the scale of the
// texture coordinates from its original size
static void bindTex_(NNSG3dResMat *mat, NNSG3dResDictTexToMatIdxData *list, const NNSG3dResTex *tex,
                     const NNSG3dResDictTexData *texData) {
    const u8 *matIds = (u8 *)mat + list->offset;
    u32 width = texData->extraParam & NNS_G3D_TEXIMAGE_PARAMEX_ORIGW_MASK;
    u32 height = (texData->extraParam & NNS_G3D_TEXIMAGE_PARAMEX_ORIGH_MASK) >> NNS_G3D_TEXIMAGE_PARAMEX_ORIGH_SHIFT;
    NNSGfdTexKey key = (texData->texImageParam & NNS_G3D_TEXIMAGE_PARAM_TEXFMT_MASK) ==
                               ((u32)GX_TEXFMT_COMP4x4 << NNS_G3D_TEXIMAGE_PARAM_TEXFMT_SHIFT)
                           ? tex->tex4x4Info.vramKey
                           : tex->texInfo.vramKey;
    u32 vramOffset = NNS_GfdGetTexKeyAddr(key) >> NNS_GFD_KEY_ADDR_SHIFT;
    u32 i;

    for (i = 0; i < list->numIdx; i++) {
        NNSG3dResMatData *matData = NNS_G3dGetMatDataByIdx(mat, matIds[i]);
        matData->texImageParam |= texData->texImageParam + vramOffset;
        matData->magW = width != matData->origWidth ? FX_Div(width << FX32_SHIFT, matData->origWidth << FX32_SHIFT)
                                                    : FX32_ONE;
        matData->magH = height != matData->origHeight
                            ? FX_Div(height << FX32_SHIFT, matData->origHeight << FX32_SHIFT)
                            : FX32_ONE;
    }
    list->flag |= NNS_G3D_MATLIST_BOUND;
}

BOOL NNS_G3dForceBindMdlTex(NNSG3dResMdl *mdl, const NNSG3dResTex *tex, u32 texToMatListIdx, u32 texIdx) {
    NNSG3dResMat *mat = NNS_G3dGetMat(mdl);
    const NNSG3dResDictTexData *texData = NNS_G3dGetTexDataByIdx(tex, texIdx);
    NNSG3dResDictTexToMatIdxData *list = NNS_G3dGetResDataByIdx(texToMatDict_(mat), texToMatListIdx);

    if (list != NULL && texData != NULL && !(list->flag & NNS_G3D_MATLIST_BOUND)) {
        bindTex_(mat, list, tex, texData);
        return TRUE;
    }
    return FALSE;
}

void NNS_G3dReleaseMdlTex(NNSG3dResMdl *mdl) {
    NNSG3dResMat *mat = NNS_G3dGetMat(mdl);
    const NNSG3dResDict *dict = texToMatDict_(mat);
    u32 i;
    u32 j;

    for (i = 0; i < dict->numEntry; i++) {
        NNSG3dResDictTexToMatIdxData *list = NNS_G3dGetResDataByIdx(dict, i);
        if (list->flag & NNS_G3D_MATLIST_BOUND) {
            const u8 *matIds = (u8 *)mat + list->offset;
            for (j = 0; j < list->numIdx; j++) {
                NNS_G3dGetMatDataByIdx(mat, matIds[j])->texImageParam &= TEXIMAGE_MATERIAL_BITS;
            }
            list->flag &= ~NNS_G3D_MATLIST_BOUND;
        }
    }
}

BOOL NNS_G3dBindMdlSet(NNSG3dResMdlSet *mdlSet, const NNSG3dResTex *tex) {
    BOOL result = TRUE;
    u32 i;

    for (i = 0; i < mdlSet->dict.numEntry; i++) {
        NNSG3dResMdl *mdl = NNS_G3dGetMdlByIdx(mdlSet, i);
        result &= NNS_G3dBindMdlTex(mdl, tex);
        result &= NNS_G3dBindMdlPltt(mdl, tex);
    }
    return result;
}

void NNS_G3dReleaseMdlSet(NNSG3dResMdlSet *mdlSet) {
    u32 i;

    for (i = 0; i < mdlSet->dict.numEntry; i++) {
        NNSG3dResMdl *mdl = NNS_G3dGetMdlByIdx(mdlSet, i);
        NNS_G3dReleaseMdlTex(mdl);
        NNS_G3dReleaseMdlPltt(mdl);
    }
}
