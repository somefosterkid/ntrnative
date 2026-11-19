#include "global.h"

void sub_020697D0(void *, void *, s32);

s32 sub_02069864(void *, void *, s32);

s32 sub_02069884(void *, void *);

void sub_02069894(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4);

typedef struct Simple3DAnimation {
    u32 flags;
    fx32 currFrame;
    void *animSet;
    void *g3DAnim;
    NNSG3dAnmObj *animObj;
    NNSFndAllocator allocator;
} Simple3DAnimation;
void sub_020698B8(Simple3DAnimation *anim);

void sub_02069894(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4) {
    sub_020697D0(arg0, arg2, arg3);
    sub_02069864(arg0, arg1, arg4);
    sub_02069884(arg0, arg1);
}

void sub_020698B8(Simple3DAnimation *anim) {
    if (anim->animObj) {
        NNS_G3dFreeAnmObj(&anim->allocator, anim->animObj);
        anim->animObj = 0L;
    }
}
