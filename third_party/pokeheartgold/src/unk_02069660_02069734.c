#include "global.h"
#include "simple3dmodel.h"

#include "heap.h"
#include "unk_02069660.h"

void sub_02069734(Simple3DModel *model) {
    void *ptr;

    ptr = model->modelSetHeader;
    if (ptr != NULL) {
        Heap_Free(ptr);
    }
}

void sub_02069744(Simple3DModel *model) {
    if (model->texture != 0L) {
        NNSG3dTexKey texKey;
        NNSG3dTexKey tex4x4Key;
        NNSG3dPlttKey plttKey;

        NNS_G3dTexReleaseTexKey(model->texture, &texKey, &tex4x4Key);
        NNS_GfdFreeTexVram(texKey);
        NNS_GfdFreeTexVram(tex4x4Key);

        plttKey = NNS_G3dPlttReleasePlttKey(model->texture);
        NNS_GfdFreePlttVram(plttKey);

        model->texture = 0L;
    }
}
