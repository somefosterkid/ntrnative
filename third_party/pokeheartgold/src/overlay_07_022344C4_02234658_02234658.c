#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_07_particle_effect.h"

ParticleSystem *ov07_0221FE88(enum HeapID, s32, s32);

UnkStruct_ov12_02238004 *ov07_02234658(enum HeapID arg0, ParticleEffectTemplate *arg1) {
    UnkStruct_ov12_02238004 *temp_r0;

    temp_r0 = Heap_Alloc(arg0, sizeof(UnkStruct_ov12_02238004));
    if (temp_r0 == NULL) {
        GF_AssertFail();
        return NULL;
    }
    temp_r0->heapID = arg0;
    temp_r0->particleEffectTemplate = *arg1;
    temp_r0->unk_1C = ov07_02234718(temp_r0->particleEffectTemplate.resourceID);
    temp_r0->unk_10 = ov07_0221FE88(temp_r0->heapID, temp_r0->particleEffectTemplate.resourceID, 1);
    return temp_r0;
}
