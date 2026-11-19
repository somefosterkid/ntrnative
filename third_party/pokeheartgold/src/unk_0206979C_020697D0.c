#include "global.h"

#include "simple3danimation.h"

void sub_020697BC(Simple3DAnimation *anim, void *pRes, u32 unused);

void sub_020697D0(Simple3DAnimation *anim, Simple3DAnimation *src, u32 unused);

void sub_020697D0(Simple3DAnimation *anim, Simple3DAnimation *src, u32 unused) {
    sub_020697BC(anim, src->animSet, unused);
}
