#include "global.h"
#include "simple3dmodel.h"

#include "gf_gfx_loader.h"
#include "gf_3d_render.h"

s32 sub_02069670(void *, void *, s32);

void sub_020696C4(void *arg0, s32 arg1, NARC *arg2, s32 arg3, enum HeapID arg4, s32 arg5);

void sub_020696E8(Simple3DModel *model);

void sub_020696C4(void *arg0, s32 arg1, NARC *narc, s32 fileId, enum HeapID heapID, s32 atEnd) {
    sub_02069670(arg0, GfGfxLoader_LoadFromOpenNarc(narc, fileId, 0, heapID, atEnd), arg1);
}

void sub_020696E8(Simple3DModel *model) {
    GF3dRender_AllocAndLoadTexResources(model->texture);
    NNS_G3dBindMdlSet(model->modelSet, model->texture);
    model->isTextureBound = 1;
}
