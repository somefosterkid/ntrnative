#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "error_handling.h"

static void sub_0202403C(NNSG3dResMdl *model, const NNSG3dResTex *texture, u8 textureIdx);

static void sub_02024140(NNSG3dResMdl *model, const NNSG3dResTex *texture, u8 plttIdx);

void sub_02023FEC(Billboard *billboard);

void sub_02023FEC(Billboard *billboard) {
    BillboardTexPlttIndex indexes = BillboardGfxSequence_GetTexPlttIndexAt(&billboard->gfxSequence, billboard->frameNum >> FX32_SHIFT);

    sub_0202403C(billboard->model, billboard->animTexture, indexes.textureIdx);
    sub_02024140(billboard->model, billboard->animTexture, indexes.plttIdx);
}

static void sub_020240C4(NNSG3dResMat *material, const NNSG3dResDictTexToMatIdxData *texToMatDict, u32 textureAddress);

static void sub_0202403C(NNSG3dResMdl *model, const NNSG3dResTex *texture, u8 textureIdx) {
    NNSG3dResMat *material = NNS_G3dGetMat(model);
    const NNSG3dResDict *textureDict = (NNSG3dResDict *)((u8 *)material + material->ofsDictTexToMatList);
    const NNSG3dResDictTexData *textureData = NNS_G3dGetTexDataByIdx(texture, textureIdx);
    u32 textureAddress = textureData->texImageParam & NNS_G3D_TEXIMAGE_PARAM_TEX_ADDR_MASK;

    for (int i = 0; i < textureDict->numEntry; ++i) {
        const NNSG3dResDictTexToMatIdxData *texToMatDict = (NNSG3dResDictTexToMatIdxData *)NNS_G3dGetResDataByIdx(textureDict, i);

        if (texToMatDict->flag & 1) {
            sub_020240C4(material, texToMatDict, textureAddress);
        }
    }
}

static void sub_020240C4(NNSG3dResMat *material, const NNSG3dResDictTexToMatIdxData *texToMatDict, u32 textureAddress) {
    u8 *texMaterials = (u8 *)material + texToMatDict->offset;

    for (int i = 0; i < texToMatDict->numIdx; i++) {
        NNSG3dResMatData *materialData = NNS_G3dGetMatDataByIdx(material, *(texMaterials + i));
        GF_ASSERT((materialData->texImageParam & NNS_G3D_TEXIMAGE_PARAM_TEX_ADDR_MASK) + textureAddress <= NNS_G3D_TEXIMAGE_PARAM_TEX_ADDR_MASK);
        materialData->texImageParam += textureAddress;
    }
}

static void sub_020241CC(NNSG3dResMat *material, const NNSG3dResDictPlttToMatIdxData *plttToMatDict, u32 plttAddress);

static void sub_02024140(NNSG3dResMdl *model, const NNSG3dResTex *texture, u8 plttIdx) {
    NNSG3dResMat *material = NNS_G3dGetMat(model);
    const NNSG3dResDict *plttDict = (NNSG3dResDict *)((u8 *)material + material->ofsDictPlttToMatList);
    const NNSG3dResDictPlttData *plttData = NNS_G3dGetPlttDataByIdx(texture, plttIdx);
    u32 plttAddress = plttData->offset;

    if (!(plttData->flag & 1)) {
        plttAddress >>= 1;
    }

    for (u32 i = 0; i < plttDict->numEntry; ++i) {
        const NNSG3dResDictPlttToMatIdxData *plttToMatDict = (NNSG3dResDictPlttToMatIdxData *)NNS_G3dGetResDataByIdx(plttDict, i);

        if (plttToMatDict->flag & 1) {
            sub_020241CC(material, plttToMatDict, plttAddress);
        }
    }
}

static void sub_020241CC(NNSG3dResMat *material, const NNSG3dResDictPlttToMatIdxData *plttToMatDict, u32 plttAddress) {
    u8 *plttMaterials = (u8 *)material + plttToMatDict->offset;

    for (u32 i = 0; i < plttToMatDict->numIdx; i++) {
        NNSG3dResMatData *materialData = NNS_G3dGetMatDataByIdx(material, *(plttMaterials + i));
        GF_ASSERT(((materialData->texPlttBase & REG_G3_TEXPLTT_BASE_PLTT_BASE_MASK) + plttAddress) <= REG_G3_TEXPLTT_BASE_PLTT_BASE_MASK);
        materialData->texPlttBase += plttAddress;
    }
}

void sub_020236BC(Billboard *billboard);

void sub_02024248(BillboardList *list);

void sub_02024248(BillboardList *list) {
    for (int i = 0; i < list->capacity; i++) {
        sub_020236BC(&list->billboards[i]);
        list->freeBillboards[i] = list->billboards + i;
    }

    list->freeBillboardHead = 0;
}

Billboard *sub_02024280(BillboardList *list);

Billboard *sub_02024280(BillboardList *list) {
    if (list->freeBillboardHead >= list->capacity) {
        return NULL;
    }

    Billboard *billboard = list->freeBillboards[list->freeBillboardHead];
    list->freeBillboardHead++;

    return billboard;
}
