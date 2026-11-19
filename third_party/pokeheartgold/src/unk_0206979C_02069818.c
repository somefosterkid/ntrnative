#include "global.h"
#include "simple3dmodel.h"

struct Simple3DAnimation;
struct Simple3DModel;
typedef struct Simple3DAnimation {
    u32 flags;
    fx32 currFrame;
    void *animSet;
    void *g3DAnim;
    NNSG3dAnmObj *animObj;
    NNSFndAllocator allocator;
} Simple3DAnimation;
void Heap_Free(void *ptr);
enum Simple3DAnimationFlags {
    ANIM_OWNS_SET = 1 << 0,
    ANIM_REACHED_END = 1 << 1,
};
void HeapExp_FndInitAllocator(NNSFndAllocator *pAllocator, u32 heapID, int alignment);
static void sub_0206983C(Simple3DAnimation *anim, const NNSG3dResMdl *model, u32 heapID);
static void sub_02069870(Simple3DAnimation *anim, const NNSG3dResMdl *model, const NNSG3dResTex *texture);

void sub_02069818(Simple3DAnimation *anim);
static void sub_0206983C(Simple3DAnimation *anim, const NNSG3dResMdl *model, u32 heapID);
void sub_02069864(Simple3DAnimation *anim, const Simple3DModel *model, u32 heapID);
static void sub_02069870(Simple3DAnimation *anim, const NNSG3dResMdl *model, const NNSG3dResTex *texture);
void sub_02069884(Simple3DAnimation *anim, const Simple3DModel *model);

void sub_02069818(Simple3DAnimation *anim) {
    if (anim->flags & ANIM_OWNS_SET) {
        Heap_Free(anim->animSet);
        anim->flags &= ~ANIM_OWNS_SET;
    }

    anim->animSet = 0L;
    anim->g3DAnim = 0L;
}

static void sub_0206983C(Simple3DAnimation *anim, const NNSG3dResMdl *model, u32 heapID) {
    HeapExp_FndInitAllocator(&anim->allocator, heapID, 4);
    anim->animObj = NNS_G3dAllocAnmObj(&anim->allocator, anim->g3DAnim, model);

    ((anim->animObj != 0L) ? (void)0 : GF_AssertFail());
}

void sub_02069864(Simple3DAnimation *anim, const Simple3DModel *model, u32 heapID) {
    sub_0206983C(anim, model->g3DModel, heapID);
}

static void sub_02069870(Simple3DAnimation *anim, const NNSG3dResMdl *model, const NNSG3dResTex *texture) {
    NNS_G3dAnmObjInit(anim->animObj, anim->g3DAnim, model, texture);
}

void sub_02069884(Simple3DAnimation *anim, const Simple3DModel *model) {
    sub_02069870(anim, model->g3DModel, model->texture);
}
