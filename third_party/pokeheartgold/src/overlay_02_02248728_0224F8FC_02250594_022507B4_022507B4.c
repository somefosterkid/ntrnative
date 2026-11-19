#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "heap.h"
#include "overlay_02.h"
#include "overlay_02_move_tasks.h"
#include "task.h"

s32 ov02_022507E8(TaskManager *taskman);

void ov02_022507B4(FieldSystem *fieldSystem, u32 a1) {
    FollowMonMoveTask *env;

    if ((u32)(FollowMon_GetSpecies(fieldSystem->followMon.mapObject) - 0x32) > 1U) {
        env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(FollowMonMoveTask));
        env->repeat = a1;
        env->count = 0;
        TaskManager_Call(fieldSystem->taskman, (TaskFunc)(ov02_022507E8), env);
    }
}
