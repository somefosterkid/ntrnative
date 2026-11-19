#include "global.h"

#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021E9144(SafariDecoration3dModel *model) {
    MtxFx33 v1;
    s32 v0[6];
    s32 v2;
    SafariDecoration3dAnim *v3;

    v2 = 0;
    if ((s32) model->numAnims > 0) {
        v3 = model->anims;
        do {
            ov108_021E9210(v3);
            v2 += 1;
            v3++;
        } while (v2 < (s32) model->numAnims);
    }
    v0[3] = 0;
    v0[4] = 0;
    v0[5] = 0;
    v0[0] = 0x1000;
    v0[1] = 0x1000;
    v0[2] = 0x1000;
    MTX_Identity33_(&v1);
    GF3dRender_DrawModel(&model->renderObj, (VecFx32 *)&v0[3], &v1, (VecFx32 *)&v0[0]);
}

void ov108_021E9198(SafariDecoration3dView *view, SafariDecoration3dModel *model, SafariDecoration3dAnim *anim, s32 fileId) {
    NNSG3dAnmObj *pAnmObj;
    void *pRes;

    pRes = GfGfxLoader_LoadFromOpenNarc(view->narcAnims, fileId, 0, view->heapID, 0);
    anim->resource = pRes;
    anim->anmRes = NNS_G3dGetAnmByIdx(pRes, 0U);
    pAnmObj = NNS_G3dAllocAnmObj(&view->allocator, anim->anmRes, model->mdl);
    anim->anmObj = pAnmObj;
    NNS_G3dAnmObjInit(pAnmObj, anim->anmRes, model->mdl, model->tex);
}

void ov108_021E91D4(SafariDecoration3dView *view, SafariDecoration3dAnim *anim) {
    if (anim->resource != 0) {
        NNS_G3dFreeAnmObj(&view->allocator, anim->anmObj);
        Heap_Free(anim->resource);
        MI_CpuFill8(anim, 0, sizeof(SafariDecoration3dAnim));
    }
    return;
}
