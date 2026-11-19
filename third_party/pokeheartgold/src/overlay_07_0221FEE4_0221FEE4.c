#include "global.h"

#include "overlay_06.h"
#include "unk_02014DA0.h"

void *ov07_0221FEDC(NARC *, s32, enum HeapID);

ParticleSystem *ov07_0221FEE4(void *arg0, enum HeapID arg1, s32 arg2, s32 arg3);

ParticleSystem *ov07_0221FEE4(void *arg0, enum HeapID heapID, s32 arg2, s32 arg3) {
    s32 loadNow;
    ParticleSystem *emitter;

    loadNow = arg3;
    emitter = ov06_0221BA88(heapID);
    if (emitter == NULL) {
        return NULL;
    }
    sub_0201526C(emitter, ov07_0221FEDC(arg0, arg2, heapID), 0xAU, loadNow);
    return emitter;
}
