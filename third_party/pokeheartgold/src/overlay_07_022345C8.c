#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"

#include "error_handling.h"
#include "sys_task.h"
#include "heap.h"
#include "sys_task_api.h"
#include "particle_system.h"
#include "overlay_07_particle_effect.h"

void ov07_0221FF2C(ParticleSystem *ps);

UnkStruct_ov12_02238004 *ov07_022345C8(UnkStruct_ov12_02237F98 *param0, ParticleEffectTemplate *particleEffectTemplate) {
    int v0;
    UnkStruct_ov12_02238004 *v1 = NULL;

    for (v0 = 0; v0 < 16; v0++) {
        if (param0->unk_08[v0] != NULL) {
            continue;
        }

        param0->unk_08[v0] = ov07_02234658(param0->heapID, particleEffectTemplate);
        param0->unk_08[v0]->unk_18 = NULL;

        v1 = param0->unk_08[v0];
        v1->unk_00 = v0;

        break;
    }

    if (v1 == NULL) {
        GF_ASSERT(FALSE);
    }

    return v1;
}

void ov07_02234604(UnkStruct_ov12_02237F98 *ptr) {
    s32 v1;
    UnkStruct_ov12_02238004 *v0;

    v1 = 0;
    do {
        v0 = ptr->unk_08[v1];
        if (v0 != NULL) {
            Heap_Free(v0);
        }
        v1 += 1;
    } while (v1 < 0x10);
    Heap_Free(ptr);
}

void ov07_02234628(UnkStruct_ov12_02237F98 *arg0) {
    SysTask *task;
    s32 v2;
    UnkStruct_ov12_02238004 *v0;
    UnkStruct_ov12_02238004 *v1;

    v2 = 0;
    do {
        v0 = arg0->unk_08[v2];
        if (v0 != NULL) {
            ov07_0221FF2C(v0->unk_10);
            v1 = arg0->unk_08[v2];
            if (v1->unk_20 != 0) {
                task = v1->unk_18;
                if (task != NULL) {
                    SysTask_Destroy(task);
                }
            }
        }
        v2 += 1;
    } while (v2 < 0x10);
}
