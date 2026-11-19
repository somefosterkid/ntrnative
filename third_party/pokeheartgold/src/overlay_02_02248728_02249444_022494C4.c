#include "global.h"

#include "field_system_def.h"
#include "field_types_def.h"
#include "map_object.h"
#include "player_avatar.h"
#include "sys_task_api.h"
#include "sys_task.h"
#include "overlay_02_hmcut_in.h"

HMCutIn *ov02_0224955C(FieldSystem *fieldSystem);

void ov02_022499B8(SysTask *cutInTask, void *hmCutInPtr);

SysTask *ov02_022494C4(FieldSystem *fieldSystem, Pokemon *mon, LocalMapObject *playerObject, LocalMapObject *followMonObject);

SysTask *ov02_022494C4(FieldSystem *fieldSystem, Pokemon *mon, LocalMapObject *playerObject, LocalMapObject *followMonObject) {
    HMCutIn *cutIn;
    LocalMapObject *object;
    fx32 delta;
    VecFx32 playerPos;
    VecFx32 objectPos;

    cutIn = ov02_0224955C(fieldSystem);
    cutIn->mon = mon;
    cutIn->playerGender = 0;
    cutIn->playerSpriteSet = 2;
    cutIn->mode = 3;
    cutIn->playerObject = playerObject;
    cutIn->followMonObject = followMonObject;
    object = PlayerAvatar_GetMapObject(cutIn->fieldSystem->playerAvatar);
    MapObject_CopyPositionVector(object, &playerPos);
    MapObject_CopyPositionVector(cutIn->playerObject, &objectPos);
    delta = FX_Div(objectPos.x - playerPos.x, FX32_CONST(2));
    cutIn->playerSpriteOffset.x += delta;
    cutIn->playerSpriteOffset.z = objectPos.z - playerPos.z;
    return SysTask_CreateOnMainQueue(ov02_022499B8, cutIn, 0x86);
}
