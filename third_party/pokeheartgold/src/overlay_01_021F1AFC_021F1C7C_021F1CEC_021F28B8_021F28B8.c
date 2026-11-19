#include "global.h"

#include "field_system.h"
#include "overlay_01_021F1AFC.h"
#include "player_avatar.h"

void *ov01_021F30D0(u32 size);

WaterfallTaskEnv *ov01_021F28B8(FieldSystem *fieldSystem, int direction, const MonRideTask *monRideTask);

WaterfallTaskEnv *ov01_021F28B8(FieldSystem *fieldSystem, int direction, const MonRideTask *monRideTask) {
    WaterfallTaskEnv *taskEnv = ov01_021F30D0(sizeof(WaterfallTaskEnv));

    taskEnv->direction = direction;
    taskEnv->fieldSystem = fieldSystem;
    taskEnv->playerAvatar = fieldSystem->playerAvatar;
    taskEnv->playerObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);

    if (monRideTask != 0L) {
        taskEnv->monRideTask = *monRideTask;
    }

    return taskEnv;
}
