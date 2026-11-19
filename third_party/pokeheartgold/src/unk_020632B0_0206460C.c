#include "global.h"

#include "approaching_trainer_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

s32 sub_0206460C(SysTask *task);
void sub_02064618(SysTask *task);

s32 sub_0206460C(SysTask *task) {
    ApproachingTrainerData *data = SysTask_GetData(task);
    return data->done;
}

void sub_02064618(SysTask *task) {
    ApproachingTrainerData *data = SysTask_GetData(task);
    Heap_FreeExplicit(HEAP_ID_FIELD1, data);
    SysTask_Destroy(task);
}
