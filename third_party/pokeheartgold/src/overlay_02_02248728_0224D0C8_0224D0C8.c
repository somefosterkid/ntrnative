#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "filesystem_files_def.h"
#include "overlay_02_0224D0C8.h"

void ov02_0224D0C8(Field3dAnimatedModel *param0, u32 param1, u32 param2, u32 param3, NNSFndAllocator *param4) {
    u32 uVar1;
    Field3DModelAnimation *anim;

    memset(param0, 0, sizeof(Field3dAnimatedModel));
    Field3dModel_LoadFromFilesystem(&param0->model, NARC_a_1_3_4, param1, HEAP_ID_FIELD1);
    Field3dObject_InitFromModel(&param0->object, &param0->model);
    param0->animCount = param3;
    uVar1 = 0;
    if (uVar1 < param0->animCount) {
        anim = &param0->anims[0];
        do {
            Field3dModelAnimation_LoadFromFilesystem(anim, &param0->model, NARC_a_1_3_4, param2 + uVar1, HEAP_ID_FIELD1, param4);
            Field3dObject_AddAnimation(&param0->object, anim);
            uVar1 = uVar1 + 1;
            anim = anim + 1;
        } while (uVar1 < param0->animCount);
    }
    return;
}
