#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "overlay_01_02203A18.h"
#include "task.h"

s32 ov01_02203AD8(TaskManager *taskManager);

void ov01_02203AB4(FieldSystem *fieldSystem, LocalMapObject *partnerPokeObj, s32 a2) {
    UnkOv01_02203AB4Env *env;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(UnkOv01_02203AB4Env));
    env->arg = a2;
    env->partnerPokeObj = partnerPokeObj;
    TaskManager_Call(fieldSystem->taskman, (TaskFunc)(ov01_02203AD8), env);
    return;
}
