#include "global.h"

#include "constants/heap.h"

#include "field_system.h"
#include "field_types_def.h"
#include "heap.h"
#include "overlay_01_021F6CFC.h"
#include "overlay_01_unk_struct_ov5_021_eae78.h"
#include "save.h"
#include "task.h"

void ov01_021F729C(FieldSystem *fieldSystem);

void ov01_021F729C(FieldSystem *fieldSystem) {
    UnkStruct_ov5_021EAE78 *env;
    TaskManager *taskman;

    taskman = fieldSystem->taskman;
    env = Heap_AllocAtEnd(HEAP_ID_FIELD2, sizeof(UnkStruct_ov5_021EAE78));
    ov01_021F722C(env);
    env->fieldSystem = fieldSystem;
    env->saveData = fieldSystem->saveData;
    env->unk_44 = 0;
    if (taskman == NULL) {
        FieldSystem_CreateTask(fieldSystem, ov01_021F7100, env);
        return;
    }
    TaskManager_Call(taskman, ov01_021F7100, env);
    return;
}
