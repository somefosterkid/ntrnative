#include "global.h"

#include "field/overlay_01_021FB878.h"
#include "overlay_01_021FB878.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "heap.h"

void ov01_021FBE70(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator) {
    ov01_021FC030(anim, model, data, allocator);
    anim->shouldRetainData = TRUE;
}

void Field3dModelAnimation_Unload(Field3DModelAnimation *anim, NNSFndAllocator *allocator) {
    if (anim->fileDataRaw) {
        NNS_G3dFreeAnmObj(allocator, anim->animObj);

        if (anim->shouldRetainData == 0) {
            Heap_Free(anim->fileDataRaw);
        }
    }

    memset(anim, 0, sizeof(Field3DModelAnimation));
}

void Field3dModelAnimation_FrameAdvanceAndLoop(Field3DModelAnimation *anim, fx32 frameDelta) {
    fx32 frameCount = NNS_G3dAnmObjGetNumFrame(anim->animObj);

    if (frameDelta > 0) {
        anim->frame = (anim->frame + frameDelta) % frameCount;
    } else {
        anim->frame = anim->frame + frameDelta;

        if (anim->frame < 0) {
            anim->frame += frameCount;
        }
    }

    NNS_G3dAnmObjSetFrame(anim->animObj, anim->frame);
}
