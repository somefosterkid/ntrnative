#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_02_move_tasks.h"
#include "heap.h"
#include "task.h"

BOOL ov02_022508D8(TaskManager *taskManager);

BOOL ov02_022508B4(FieldSystem *fieldSystem);

BOOL ov02_022508B4(FieldSystem *fieldSystem) {
    ScaleBounceTask *env;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(ScaleBounceTask));
    env->hold = 0;
    env->frame = 0;
    TaskManager_Call(fieldSystem->taskman, (TaskFunc)(ov02_022508D8), env);
    return 1;
}
