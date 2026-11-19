#include "global.h"
#include "simple3dmodel.h"
#include "simple3danimation.h"

s32 sub_0206979C(void *);

s32 sub_02069818(void *);

void sub_020698B8(Simple3DAnimation *anim);
void sub_020698D0(Simple3DAnimation *anim);
enum Simple3DAnimationFlags {
    ANIM_OWNS_SET = 1 << 0,
    ANIM_REACHED_END = 1 << 1,
};
BOOL sub_020698E8(Simple3DAnimation *anim, fx32 frameDelta, BOOL loop);
BOOL sub_02069948(const Simple3DAnimation *anim);
void sub_02069958(Simple3DRenderObj *renderObj);

void sub_020698D0(Simple3DAnimation *anim) {
    sub_020698B8(anim);
    sub_02069818(anim);
    sub_0206979C(anim);
}

BOOL sub_020698E8(Simple3DAnimation *anim, fx32 frameDelta, BOOL loop) {
    u32 reachedEnd = 0;
    fx32 animLength = NNS_G3dAnmObjGetNumFrame(anim->animObj);

    anim->currFrame += frameDelta;

    if (frameDelta > 0) {
        if (anim->currFrame >= animLength) {
            reachedEnd = 1;

            if (loop == 1) {
                anim->currFrame -= animLength;
            } else {
                anim->currFrame = animLength;
            }
        }
    } else {
        if (anim->currFrame <= 0) {
            reachedEnd = 1;

            if (loop == 1) {
                anim->currFrame += animLength;
            } else {
                anim->currFrame = 0;
            }
        }
    }

    NNS_G3dAnmObjSetFrame(anim->animObj, anim->currFrame);

    if (reachedEnd == 1) {
        anim->flags |= ANIM_REACHED_END;
    } else {
        anim->flags &= ~ANIM_REACHED_END;
    }

    return reachedEnd;
}

BOOL sub_02069948(const Simple3DAnimation *anim) {
    return (anim->flags & ANIM_REACHED_END) != 0;
}

void sub_02069958(Simple3DRenderObj *renderObj) {
    memset(renderObj, 0, sizeof(Simple3DRenderObj));
}
