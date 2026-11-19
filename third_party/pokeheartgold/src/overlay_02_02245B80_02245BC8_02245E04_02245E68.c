#include "global.h"

#include "heap.h"
#include "overlay_02_app_02245e68.h"

void ov02_02245E68(Ov02App_02245E68 *effect) {
    s32 i;

    Field3dObject_SetActiveFlag(&effect->object, 0);
    for (i = 0; i < effect->animCount; i++) {
        Field3dObject_RemoveAnimation(&effect->object, &effect->anims[i]);
        Field3dModelAnimation_Unload(&effect->anims[i], &effect->allocator);
    }
    effect->animCount = 0;
    Field3dModel_Unload(&effect->model);
    Heap_Free(effect->anims);
    Heap_Free(effect);
}
