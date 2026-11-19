#include "global.h"

#include "unk_02014DA0.h"

ParticleSystem *ov95_021E5DD0(enum HeapID arg0);

ParticleSystem *ov95_021E5E18(enum HeapID arg0, enum NarcId arg1, s32 arg2);

ParticleSystem *ov95_021E5E18(enum HeapID heapID, enum NarcId narcId, s32 fileId) {
    ParticleSystem *emitter;

    emitter = ov95_021E5DD0(heapID);
    sub_0201526C(emitter, sub_02015264(narcId, fileId, heapID), 0xAU, 1);
    return emitter;
}
