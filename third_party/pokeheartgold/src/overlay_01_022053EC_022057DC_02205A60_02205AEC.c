#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "task.h"

void ov01_02205B14();

void ov01_02205AEC(FieldSystem *fieldSystem);

void ov01_02205AEC(FieldSystem *fieldSystem) {
    u8 *env;

    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, 0x48);
    *env = 0;
    env[1] = 0;
    env[2] = 0;
    env[3] = 0;
    TaskManager_Call(fieldSystem->taskman, (TaskFunc)(ov01_02205B14), env);
    return;
}
