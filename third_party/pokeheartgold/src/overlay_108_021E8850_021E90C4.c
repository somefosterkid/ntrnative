#include "global.h"

#include "heap.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021E90C4(SafariDecoration3dView *view, SafariDecoration3dModel *model) {
    u32 texKey;
    u32 tex4x4Key;
    NNSG3dResTex *pTex;
    s32 v0;
    void *ptr;
    SafariDecoration3dAnim *anim;

    v0 = 0;
    if ((s32) model->numAnims > 0) {
        anim = model->anims;
        do {
            ov108_021E9204(model, anim);
            ov108_021E91D4(view, anim);
            v0 += 1;
            anim++;
        } while (v0 < (s32) model->numAnims);
    }
    pTex = model->tex;
    if (pTex != NULL) {
        NNS_G3dTexReleaseTexKey(pTex, &texKey, &tex4x4Key);
        NNS_GfdDefaultFuncFreeTexVram(texKey);
        NNS_GfdDefaultFuncFreeTexVram(tex4x4Key);
        NNS_GfdDefaultFuncFreePlttVram(NNS_G3dPlttReleasePlttKey(model->tex));
    }
    ptr = model->file;
    if (ptr != NULL) {
        Heap_Free(ptr);
    }
    MI_CpuFill8(model, 0U, sizeof(SafariDecoration3dModel));
}
