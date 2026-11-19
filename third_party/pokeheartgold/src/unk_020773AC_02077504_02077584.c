#include "global.h"

#include "unk_02014DA0.h"

ParticleSystem *sub_0207753C(int heapID);

ParticleSystem *sub_02077584(enum HeapID arg0, enum NarcId arg1, s32 arg2);

ParticleSystem *sub_02077584(enum HeapID heapID, enum NarcId narcId, s32 fileId) {
    ParticleSystem *emitter;

    emitter = sub_0207753C(heapID);
    sub_0201526C(emitter, sub_02015264(narcId, fileId, heapID), 0xAU, 1);
    return emitter;
}
