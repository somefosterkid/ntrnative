#include "global.h"

#include "heap.h"
#include "map_object.h"
#include "overlay_01_021F944C.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_020689C8.h"

s32 sub_02023DA4(void *);

void ov01_021FAB9C(SysTask *arg0, void *arg1);

void ov01_021FAB9C(SysTask *task, void *ptr_) {
    ModelMapObjectWork *ptr = ptr_;
    LocalMapObject *localMapObject;
    s32 v0;
    OverworldAnimManager *a0;
    OverworldAnimManager *v1;

    v0 = ptr->state;
    localMapObject = ptr->mapObject;
    switch (v0) {
    case 0:
        if ((MapObject_CheckActive(localMapObject) == 0) || (ov01_021FA2D4(localMapObject) == 0)) {
            a0 = ptr->animManager;
            if (a0 != 0) {
                sub_02068B48(a0);
            }
            sub_02023DA4(ptr->billboard);
            Heap_Free(ptr->modelData);
            Heap_Free(ptr->animData);
            Heap_Free(ptr->texData);
            Heap_Free(ptr);
            SysTask_Destroy(task);
            return;
        }
        if (ptr->spriteId != MapObject_GetSpriteID(localMapObject)) {
            ptr->state = (s32)(ptr->state + 1);
            return;
        }
        return;
    case 1:
        if (ov01_021FA2D4(localMapObject) == 0) {
            v1 = ptr->animManager;
            if (v1 != 0) {
                sub_02068B48(v1);
            }
            sub_02023DA4(ptr->billboard);
            Heap_Free(ptr->modelData);
            Heap_Free(ptr->animData);
            Heap_Free(ptr->texData);
            Heap_Free(ptr);
            SysTask_Destroy(task);
        }
        break;
    }
}

#if defined(SDK_PORT) || defined(UBFIX)
// The same end as the two paths above. The tasks are only ended when their object is removed or stops being a model, but
// FieldMap_Exit destroys HEAP_ID_FIELD1, the effect manager and the billboard lists first, and the objects (and so the
// tasks) are only removed afterwards by the warp task: freeing then would hit a destroyed heap and a freed anim manager.
static void ModelMapObjectTask_End(SysTask *task, void *work) {
    ModelMapObjectWork *ptr = work;

    if (ptr->animManager != NULL) {
        sub_02068B48(ptr->animManager);
    }
    sub_02023DA4(ptr->billboard);
    Heap_Free(ptr->modelData);
    Heap_Free(ptr->animData);
    Heap_Free(ptr->texData);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}

// Ends every model map object task. FieldMap_Exit calls it before it tears the field down.
void ModelMapObjectTask_EndAll(void) {
    SysTaskQueue_ForEachWithFunc(gSystem.vwaitTaskQueue, ov01_021FAB9C, ModelMapObjectTask_End);
}
#endif
