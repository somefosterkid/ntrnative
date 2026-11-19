#include "global.h"

#include "field_types_def.h"
#include "heap.h"
#include "overlay_02.h"
#include "task.h"

enum FieldWarpStateResult {
    STATE_RESULT_NEXT_STATE = 0,
    STATE_RESULT_REPEAT_STATE,
    STATE_RESULT_END_TASK,
};

typedef enum FieldWarpStateResult (*FieldWarpStateFunc)(TaskManager *, FieldSystem *, FieldMoveTaskEnvironment *);

extern const FieldWarpStateFunc ov02_02253700[4];

BOOL Task_FieldEscapeRope(TaskManager *task) {
    enum FieldWarpStateResult stateResult;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(task);
    FieldMoveTaskEnvironment *env = TaskManager_GetEnvironment(task);

    do {
        stateResult = ov02_02253700[env->state](task, fieldSystem, env);

        if (stateResult == STATE_RESULT_END_TASK) {
            Heap_Free(env);
        }
    } while (stateResult == STATE_RESULT_REPEAT_STATE);

    return FALSE;
}
