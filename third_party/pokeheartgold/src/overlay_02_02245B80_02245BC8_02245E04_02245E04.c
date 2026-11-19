#include "global.h"

#include "constants/heap.h"

#include "overlay_02_app_02245e68.h"

#include "filesystem_files_def.h"
#include "heap.h"

Ov02App_02245E68 *ov02_02245E04(const Ov02Field3dEffectDesc *desc, enum HeapID heapID) {
    Ov02App_02245E68 *effect;

    effect = Heap_Alloc(heapID, sizeof(Ov02App_02245E68));
    MI_CpuFill8(effect, 0, sizeof(Ov02App_02245E68));
    effect->heapID = heapID;
    HeapExp_FndInitAllocator(&effect->allocator, heapID, 0x20);
    Field3dModel_LoadFromFilesystem(&effect->model, (NarcId)desc->narcId, desc->fileId, heapID);
    Field3dObject_InitFromModel(&effect->object, &effect->model);
    ov02_02245ED8(effect, (NarcId)desc->narcId, desc->animIds, desc->animCount);
    Field3dObject_SetActiveFlag(&effect->object, 1);
    effect->animSpeed = 1;
    return effect;
}
