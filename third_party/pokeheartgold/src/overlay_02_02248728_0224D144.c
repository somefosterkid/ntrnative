#include "global.h"

#include "overlay_02_0224D0C8.h"

void ov02_0224D144(Field3dAnimatedModel *param0, NNSFndAllocator *param1) {
    int v0;

    Field3dModel_Unload(&param0->model);

    for (v0 = 0; v0 < param0->animCount; v0++) {
        Field3dModelAnimation_Unload(&param0->anims[v0], param1);
    }
}
