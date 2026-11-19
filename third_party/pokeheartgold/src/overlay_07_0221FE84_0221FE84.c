#include "global.h"

#include "overlay_06.h"
#include "unk_02014DA0.h"

void ov07_0221FE84(void);

void ov07_0221FE84(void) {
}

ParticleSystem *ov07_0221FE88(enum HeapID arg0, s32 arg1, s32 arg2);

ParticleSystem *ov07_0221FE88(enum HeapID heapID, s32 fileId, s32 loadNow) {
    ParticleSystem *emitter;

    emitter = ov06_0221BA40(heapID);
    sub_0201526C(emitter, sub_02015264(NARC_a_0_2_9, fileId, heapID), 0xAU, loadNow);
    return emitter;
}

ParticleSystem *ov07_0221FEB0(enum HeapID arg0, enum NarcId arg1, s32 arg2, s32 arg3);

ParticleSystem *ov07_0221FEB0(enum HeapID heapID, enum NarcId narcId, s32 fileId, s32 loadNow) {
    s32 v0;
    ParticleSystem *emitter;

    v0 = loadNow;
    emitter = ov06_0221BA40(heapID);
    sub_0201526C(emitter, sub_02015264(narcId, fileId, heapID), 0xAU, loadNow);
    return emitter;
}
