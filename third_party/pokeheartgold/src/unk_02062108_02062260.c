#include "global.h"

#include "sys_task.h"
#include "unk_02062108.h"
#include "error_handling.h"
#include "heap.h"
#include "move_anim_data.h"
#include "sys_task_api.h"


BOOL EventObjectMovementMan_IsFinish(EventObjectMovementMan *movementMan) {
    MoveAnimData *data = SysTask_GetData((SysTask *)movementMan);

    return data->ended;
}

void EventObjectMovementMan_Delete(EventObjectMovementMan *movementMan) {
    MoveAnimData *data;

    data = SysTask_GetData((SysTask *)movementMan);
    if (MapObject_IsMovementPaused(data->mapObj) != 1) {
        GF_AssertFail();
    }
    MapObject_ClearHeldMovementIfActive(data->mapObj);
    Heap_FreeExplicit(HEAP_ID_FIELD1, data);
    SysTask_Destroy((SysTask *)movementMan);
}
