#include "global.h"
#include "field_system_def.h"

struct FieldSystem;

extern void *ov01_021F1740(FieldSystem *fieldSystem, int a1, VecFx32 *a2);

extern void *sub_02023F90(void *a0);

void *ov01_022003C4(FieldSystem *fs, int sel, VecFx32 *vec);

void *ov01_022003C4(FieldSystem *fs, int sel, VecFx32 *vec) {
    int models[2] = { 1, 0xC };
    void *r = ov01_021F1740(fs, models[sel], vec);
    NNS_G3dMdlSetMdlFogEnableFlagAll(sub_02023F90(r), 0);
    return r;
}
