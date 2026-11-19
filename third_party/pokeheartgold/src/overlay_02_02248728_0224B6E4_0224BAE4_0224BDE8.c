#include "global.h"

#include "overlay_02_prop_anim_tasks.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "task.h"

s32 sub_02054C20(FieldSystem *, u32, MapProp **, s32 *);

BOOL ov02_0224BE24(TaskManager *taskManager);

void ov02_0224BDE8(FieldSystem *fieldSystem, u8 direction, u8 length);

void ov02_0224BDE8(FieldSystem *fieldSystem, u8 direction, u8 length) {
    s32 iVar1;
    MapPropAnimTask *env;

    iVar1 = sub_02054C20(fieldSystem, 0xd0, 0, 0);
    if (iVar1 != 0) {
        env = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(MapPropAnimTask));
        env->length = length;
        env->direction = direction;
        env->state = 0;
        TaskManager_Call(fieldSystem->taskman, (TaskFunc)(ov02_0224BE24), env);
        return;
    }
    GF_AssertFail();
    return;
}
