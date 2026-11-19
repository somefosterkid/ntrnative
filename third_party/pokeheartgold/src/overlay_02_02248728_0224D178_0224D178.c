#include "global.h"

#include "overlay_02_0224D0C8.h"

BOOL ov02_0224D178(Field3dAnimatedModel *param0) {
    int v0;
    BOOL v1 = 1;

    for (v0 = 0; v0 < param0->animCount; v0++) {
        v1 &= Field3dModelAnimation_FrameAdvanceAndCheck(&param0->anims[v0], FX32_ONE);
    }

    return v1;
}

void ov02_0224D1AC(Field3dAnimatedModel *param0) {
    int v0;

    for (v0 = 0; v0 < param0->animCount; v0++) {
        Field3dModelAnimation_FrameAdvanceAndLoop(&param0->anims[v0], FX32_ONE);
    }
}
