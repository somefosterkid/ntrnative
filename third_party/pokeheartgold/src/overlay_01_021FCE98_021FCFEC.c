#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "heap.h"
#include "overlay_01_021FCE98.h"
#include "task.h"
#include "field_system.h"

s32 ov01_021FD014(TaskManager *taskman);

void ov01_021FCFEC(FieldSystem *fieldSystem) {
    SweetScentEffect *env;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(SweetScentEffect));
    ov01_021FD064(HEAP_ID_FIELD1, fieldSystem, env);
    TaskManager_Call(fieldSystem->taskman, (TaskFunc)(ov01_021FD014), env);
}
