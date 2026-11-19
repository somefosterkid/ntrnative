#include "global.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021F1AFC.h"
#include "unk_02062108.h"

s32 ov01_021F2894(RockClimbTaskEnv *taskEnv);

s32 ov01_021F2894(RockClimbTaskEnv *taskEnv) {
    if (MapObject_IsMovementPaused(taskEnv->playerObject) == 0) {
        return 0;
    }
    MapObject_ClearHeldMovementIfActive(taskEnv->playerObject);
    ov01_021F1640(taskEnv->animManager);
    return 1;
}
