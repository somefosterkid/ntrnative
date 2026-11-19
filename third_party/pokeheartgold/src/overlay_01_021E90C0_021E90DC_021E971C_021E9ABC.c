#include "global.h"

#include "field/map_prop_animation.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "task.h"

s32 ov01_021E9ABC(TaskManager *arg0);

s32 ov01_021E9ABC(TaskManager *taskManager) {
    FieldSystem *fieldSystem;
    u8 *ptr;

    fieldSystem = TaskManager_GetFieldSystem(taskManager);
    ptr = TaskManager_GetEnvironment(taskManager);
    if (MapPropOneShotAnimationManager_IsAnimationLoopFinished(fieldSystem->mapPropOneShotAnimationManager, (s32)*ptr) != 0) {
        Heap_Free(ptr);
        return 1;
    }
    return 0;
}
