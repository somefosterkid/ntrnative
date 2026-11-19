#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

void sub_02020738(BillboardVRAMTransfer *transfer);
NNSG3dResMdlSet *sub_02024328(const BillboardResources *resources, NNSG3dResMdl **outModel, NNSG3dResTex **outTexture);
static void sub_02023AA0(const NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey);
void sub_02023B70(NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey);
static BOOL sub_02023BA4(const NNSG3dResTex *texture1, const NNSG3dResTex *texture2);
BillboardVRAMTransfer *sub_020206E0(BillboardListVRAMTransfer *listTransfer, const BillboardGfxSequence *gfxSequence, const NNSG3dResTex *texture, NNSGfdTexKey texKey, NNSGfdPlttKey plttKey, const fx32 index);
NNSG3dResTex *sub_02024374(const BillboardResources *resources);

static void sub_02023AA0(const NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey);
void sub_02023B38(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *outPlttKey);
void sub_02023B4C(NNSG3dResTex *texture, NNSG3dResMdlSet *modelSet, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey);
void sub_02023B70(NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey);
static BOOL sub_02023BA4(const NNSG3dResTex *texture1, const NNSG3dResTex *texture2);
void sub_02023C04(BillboardList *unused, Billboard *billboard);
void sub_02023C20(Billboard *billboard, const BillboardResources *resources);
void sub_02023C9C(const BillboardList *list, Billboard *billboard, const BillboardResources *resources);

static void sub_02023AA0(const NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey) {
    u32 texSize = NNS_G3dTexGetRequiredSize(texture);
    u32 tex4x4Size = NNS_G3dTex4x4GetRequiredSize(texture);
    u32 plttSize = NNS_G3dPlttGetRequiredSize(texture);

    if (texSize > 0) {
        *texKey = NNS_GfdAllocTexVram(texSize, 0, 0);
        ((*texKey != (u32)0x0) ? (void)0 : GF_AssertFail());
    } else {
        *texKey = 0;
    }

    if (tex4x4Size > 0) {
        *tex4x4Key = NNS_GfdAllocTexVram(tex4x4Size, 1, 0);
        ((*tex4x4Key != (u32)0x0) ? (void)0 : GF_AssertFail());
    } else {
        *tex4x4Key = 0;
    }

    if (plttSize > 0) {
        *plttKey = NNS_GfdAllocPlttVram(plttSize, texture->tex4x4Info.flag & NNS_G3D_RESPLTT_USEPLTT4, 0);
        ((*plttKey != (u32)0x0) ? (void)0 : GF_AssertFail());
    } else {
        *plttKey = 0;
    }
}

void sub_02023B38(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *outPlttKey) {
    NNS_G3dTexReleaseTexKey(texture, texKey, tex4x4Key);
    *outPlttKey = NNS_G3dPlttReleasePlttKey(texture);
}

void sub_02023B4C(NNSG3dResTex *texture, NNSG3dResMdlSet *modelSet, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey) {
    NNS_G3dTexSetTexKey(texture, *texKey, *tex4x4Key);
    NNS_G3dPlttSetPlttKey(texture, *plttKey);
    NNS_G3dBindMdlSet(modelSet, texture);
}

void sub_02023B70(NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *plttKey) {
    if (*texKey != (u32)0x0) {
        NNS_GfdFreeTexVram(*texKey);
    }

    if (*tex4x4Key != (u32)0x0) {
        NNS_GfdFreeTexVram(*tex4x4Key);
    }

    if (*plttKey != (u32)0x0) {
        NNS_GfdFreePlttVram(*plttKey);
    }
}

static BOOL sub_02023BA4(const NNSG3dResTex *texture1, const NNSG3dResTex *texture2) {
    if (texture1 == 0L || texture2 == 0L) {
        return 0;
    }

    u32 texSize1 = NNS_G3dTexGetRequiredSize(texture1);
    u32 tex4x4Size1 = NNS_G3dTex4x4GetRequiredSize(texture1);
    u32 plttSize1 = NNS_G3dPlttGetRequiredSize(texture1);
    u32 texSize2 = NNS_G3dTexGetRequiredSize(texture2);
    u32 tex4x4Size2 = NNS_G3dTex4x4GetRequiredSize(texture2);
    u32 plttSize2 = NNS_G3dPlttGetRequiredSize(texture2);

    BOOL ret;
    if (texSize1 != texSize2 || tex4x4Size1 != tex4x4Size2 || plttSize1 != plttSize2) {
        ret = 0;
    } else {
        ret = 1;
    }

    return ret;
}

void sub_02023C04(BillboardList *unused, Billboard *billboard) {
    if (billboard->vramTransfer) {
        sub_02020738(billboard->vramTransfer);
        billboard->vramTransfer = 0L;
    }
}

void sub_02023C20(Billboard *billboard, const BillboardResources *resources) {
    BOOL sameTexSize;
    NNSG3dResTex *originalTexture = billboard->texture;

    billboard->modelSet = sub_02024328(resources, &billboard->model, &billboard->texture);

    NNS_G3dRenderObjInit(&billboard->renderObj, billboard->model);

    if (billboard->state != BILLBOARD_STATE_ACTIVE) {
        sameTexSize = sub_02023BA4(originalTexture, billboard->texture);
    } else {
        sameTexSize = 0;
    }

    if (sameTexSize == 0) {
        if (billboard->state == BILLBOARD_STATE_VRAM_TRANSFER) {
            sub_02023B70(&billboard->texKey, &billboard->tex4x4Key, &billboard->plttKey);
        }

        sub_02023AA0(billboard->texture, &billboard->texKey, &billboard->tex4x4Key, &billboard->plttKey);
    }
}

void sub_02023C9C(const BillboardList *list, Billboard *billboard, const BillboardResources *resources) {
    billboard->animTexture = sub_02024374(resources);
    billboard->gfxSequence = resources->gfxSequence;
    billboard->vramTransfer = sub_020206E0(list->vramTransfer, &billboard->gfxSequence, billboard->animTexture, billboard->texKey, billboard->plttKey, billboard->frameNum);
}
