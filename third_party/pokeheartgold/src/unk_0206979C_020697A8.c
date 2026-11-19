#include "global.h"

#include "simple3danimation.h"

void sub_02069800(Simple3DAnimation *anim, void *pRes);

void sub_020697A8(Simple3DAnimation *anim, void *pRes, u32 unused);

void sub_020697A8(Simple3DAnimation *anim, void *pRes, u32 unused) {
    sub_02069800(anim, pRes);
    anim->flags |= 1;
}

void sub_020697BC(Simple3DAnimation *anim, void *pRes, u32 unused);

void sub_020697BC(Simple3DAnimation *anim, void *pRes, u32 unused) {
    sub_02069800(anim, pRes);
    anim->flags &= ~1;
}
