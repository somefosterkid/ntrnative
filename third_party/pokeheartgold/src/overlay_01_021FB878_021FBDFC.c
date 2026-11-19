#include "global.h"

#include "field/overlay_01_021FB878.h"
#include "overlay_01_021FB878.h"
#include "constants/heap.h"
#include "gf_gfx_loader.h"
#include "filesystem.h"
#include "filesystem_files_def.h"


void ov01_021FC030(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator);

void ov01_021FBDFC(Field3dModel *model) {
    u32 texKey;
    u32 tex4x4Key;

    if (model->texture != NULL) {
        NNS_G3dTexReleaseTexKey(model->texture, &texKey, &tex4x4Key);
        NNS_GfdDefaultFuncFreeTexVram(texKey);
        NNS_GfdDefaultFuncFreeTexVram(tex4x4Key);
        NNS_GfdDefaultFuncFreePlttVram(NNS_G3dPlttReleasePlttKey(model->texture));
    }
    memset(model, 0, sizeof(Field3dModel));
}

void Field3dModelAnimation_LoadFromFilesystem(Field3DModelAnimation *anim, const Field3dModel *model, NarcId narcId, s32 fileId, enum HeapID heapID, NNSFndAllocator *allocator) {
    void *data = GfGfxLoader_LoadFromNarc(narcId, fileId, 0, heapID, 0);

    ov01_021FC030(anim, model, data, allocator);
    anim->shouldRetainData = 0;
}
