#include "global.h"

#include "heap.h"
#include "overlay_106.h"
#include "overlay_106_legendary_cinematic.h"

void ov106_021E5BBC(LegendaryCinematic *cinematic) {
    unsigned short new_var;
    s32 v1;
    LegendaryCinematicModel *v4;
    u16 v2;
    u16 v5;
    new_var = 0U;
    v1 = 0;
    if (cinematic->numModels > new_var) {
        do {
            v5 = 0;
            v4 = &cinematic->models[v1];
            do {
                if (v4->anmData[v5] != NULL) {
                    NNS_G3dFreeAnmObj(&cinematic->allocator, v4->anmObj[v5]);
                    Heap_Free(v4->anmData[v5]);
                }
                v5 += 1;
            } while (v5 < 4U);
            Heap_Free(v4->fileHeader);
            v2 = v1 + 1;
            v1 = (s32)v2;
        } while (v2 < cinematic->numModels);
    }
}
