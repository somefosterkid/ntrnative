#include "global.h"

#include "approaching_trainer_data.h"
#include "constants/heap.h"
#include "error_handling.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

void sub_02064618(SysTask *task);
void sub_02064630(SysTask *task, void *inData);

void sub_020645AC(SysTask *task);
SysTask *sub_020645B4(FieldSystem *fieldSystem, LocalMapObject *mapObj, PlayerAvatar *playerAvatar, int direction, int sightRange, int unused, int approachType, int approachNum);

void sub_020645AC(SysTask *task) {
    sub_02064618(task);
}

SysTask *sub_020645B4(FieldSystem *fieldSystem, LocalMapObject *mapObj, PlayerAvatar *playerAvatar, int direction, int sightRange, int unused, int approachType, int approachNum) {
    SysTask *task;
    ApproachingTrainerData *data = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(ApproachingTrainerData));
    GF_ASSERT(data != NULL);

    memset(data, 0, sizeof(ApproachingTrainerData));

    data->direction = direction;
    data->sightRange = sightRange;
    data->unused = unused;
    data->approachType = approachType;
    data->approachNum = approachNum;
    data->fieldSystem = fieldSystem;
    data->mapObj = mapObj;
    data->playerAvatar = playerAvatar;

    task = SysTask_CreateOnMainQueue(sub_02064630, data, 0xFF);
    GF_ASSERT(task != NULL);

    return task;
}
