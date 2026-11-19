#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "filesystem.h"
#include "filesystem_files_def.h"

BOOL Field3dModelAnimation_FrameAdvanceAndCheck(Field3DModelAnimation *anim, fx32 frameDelta) {
    fx32 frameCount = NNS_G3dAnmObjGetNumFrame(anim->animObj);
    BOOL finished = FALSE;

    if (frameDelta > 0) {
        if (anim->frame + frameDelta < frameCount) {
            anim->frame += frameDelta;
        } else {
            anim->frame = frameCount;
            finished = TRUE;
        }
    } else {
        if (anim->frame + frameDelta >= 0) {
            anim->frame += frameDelta;
        } else {
            anim->frame = 0;
            finished = TRUE;
        }
    }

    NNS_G3dAnmObjSetFrame(anim->animObj, anim->frame);
    return finished;
}
