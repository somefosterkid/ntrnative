#include "global.h"

#include "constants/heap.h"

#include "overlay_02_app_02245e68.h"

#include "filesystem_files_def.h"
#include "heap.h"

void ov02_02245ED8(Ov02App_02245E68 *effect, NarcId narcId, u16 *animIds, u8 animCount) {
    s32 i;

    if (effect->animCount != 0) {
        for (i = 0; i < effect->animCount; i++) {
            Field3dObject_RemoveAnimation(&effect->object, &effect->anims[i]);
            Field3dModelAnimation_Unload(&effect->anims[i], &effect->allocator);
        }
        Heap_Free(effect->anims);
    }
    effect->animCount = animCount;
    effect->anims = Heap_Alloc(effect->heapID, effect->animCount * sizeof(Field3DModelAnimation));
    MI_CpuFill8(effect->anims, 0, effect->animCount * sizeof(Field3DModelAnimation));
    for (i = 0; i < effect->animCount; i++) {
        Field3dModelAnimation_LoadFromFilesystem(&effect->anims[i], &effect->model, narcId, *animIds, effect->heapID, &effect->allocator);
        Field3dObject_AddAnimation(&effect->object, &effect->anims[i]);
        animIds++;
    }
}
