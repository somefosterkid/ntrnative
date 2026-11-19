#include "global.h"

#include "field_system_def.h"
#include "follow_mon.h"
#include "map_object.h"
#include "player_avatar.h"
#include "sys_task_api.h"
#include "overlay_02_hmcut_in.h"

void ov02_02249444(FieldSystem *fieldSystem, BOOL set);

void ov02_02249444(FieldSystem *fieldSystem, BOOL set) {
    MapObject_SetVisible(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar), set);
}

HMCutIn *ov02_0224955C(FieldSystem *fieldSystem);

void ov02_02249584(SysTask *task, void *data);

void ov02_02249984(SysTask *task, void *data);

void ov02_022499B8(SysTask *task, void *data);

SysTask *ov02_02249458(FieldSystem *fieldSystem, int mode, Pokemon *mon, int gender);

SysTask *ov02_02249458(FieldSystem *fieldSystem, int mode, Pokemon *mon, int gender) {
    HMCutIn *cutIn = ov02_0224955C(fieldSystem);

    cutIn->mon = mon;
    cutIn->playerGender = gender;
    cutIn->playerSpriteSet = cutIn->playerGender;
    cutIn->mode = mode;
    cutIn->playerObject = PlayerAvatar_GetMapObject(cutIn->fieldSystem->playerAvatar);
    if (mode == 0) {
        return SysTask_CreateOnMainQueue(ov02_02249584, cutIn, 0x86U);
    }
    if (mode == 2) {
        cutIn->followMonObject = FollowMon_GetMapObject(fieldSystem);
        return SysTask_CreateOnMainQueue(ov02_022499B8, cutIn, 0x86U);
    }
    return SysTask_CreateOnMainQueue(ov02_02249984, cutIn, 0x86U);
}
