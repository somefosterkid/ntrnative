#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "follow_mon.h"
#include "overlay_02.h"
#include "player_avatar.h"


FieldMoveTaskEnvironment *CreateFieldEscapeRopeTaskEnv(FieldSystem *fieldSystem, enum HeapID heapID) {
    FieldMoveTaskEnvironment *fieldMoveTaskEnvironment;

    fieldMoveTaskEnvironment = ov02_0224C660(heapID, sizeof(FieldMoveTaskEnvironment));
    fieldMoveTaskEnvironment->type = 0;
    fieldMoveTaskEnvironment->fieldSystem = fieldSystem;
    fieldMoveTaskEnvironment->mapObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    if ((u32)(PlayerAvatar_GetState(fieldSystem->playerAvatar) - 1) <= 1U) {
        fieldMoveTaskEnvironment->followMonActive = 0;
    } else if (FollowMon_IsActive(fieldSystem) != 0) {
        fieldMoveTaskEnvironment->followMonActive = 1;
    } else {
        fieldMoveTaskEnvironment->followMonActive = 0;
    }
    return fieldMoveTaskEnvironment;
}
