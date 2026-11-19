#include "global.h"

#include "overlay_02_app_02245e68.h"

s32 ov02_02245FD0(Ov02App_02245E68 *effect) {
    s32 finished;
    s32 i;

    finished = 0;
    for (i = 0; i < effect->animCount; i++) {
        finished += Field3dModelAnimation_FrameAdvanceAndCheck(&effect->anims[i], FX32_CONST(effect->animSpeed));
    }
    Field3dObject_Draw(&effect->object);
    if (finished == effect->animCount) {
        return 1;
    }
    return 0;
}
