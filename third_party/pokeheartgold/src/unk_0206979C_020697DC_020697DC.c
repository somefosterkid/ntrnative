#include "global.h"
struct Simple3DAnimation;

#include "gf_gfx_loader.h"
#include "simple3danimation.h"

void sub_020697A8(Simple3DAnimation *anim, void *pRes, u32 unused);

void sub_020697DC(Simple3DAnimation *anim, u32 unused, NARC *narc, s32 fileId, enum HeapID heapID, s32 atEnd);

void sub_020697DC(Simple3DAnimation *anim, u32 unused, NARC *narc, s32 fileId, enum HeapID heapID, s32 atEnd) {
    sub_020697A8(anim, GfGfxLoader_LoadFromOpenNarc(narc, fileId, 0, heapID, atEnd), unused);
}

s32 sub_0206979C(Simple3DAnimation *anim);

void sub_02069800(struct Simple3DAnimation *arg0, void *arg1);

void sub_02069800(Simple3DAnimation *arg0, void *pRes) {
    sub_0206979C(arg0);
    arg0->animSet = pRes;
    arg0->g3DAnim = NNS_G3dGetAnmByIdx(pRes, 0U);
}
