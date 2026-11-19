#include "global.h"

#include "field_types_def.h"
#include "map_object.h"
#include "overlay_01_021F1AFC.h"
#include "player_avatar.h"
#include "constants/heap.h"
#include "error_handling.h"
#include "field_system.h"
#include "heap.h"
#include "overlay_01.h"
#include "sys_task.h"
#include "sys_task_api.h"


void ov01_021F3030(SysTask *task, void *data);

s32 ov01_021F1B38(PlayerAvatar *);
s32 ov01_021F1C30(PlayerAvatar *);

void ov01_021F2F24(PlayerAvatar *playerAvatar) {
    LocalMapObject *object;
    BOOL BVar1;
    s32 sVar2;
    u32 uVar3;

    object = PlayerAvatar_GetMapObject(playerAvatar);
    BVar1 = sub_0205F73C(object);
    if (((BVar1 != 0) && (sVar2 = PlayerAvatar_GetState(playerAvatar), sVar2 == 0)) && (uVar3 = MapObject_GetSpriteID(object), uVar3 == 0xc4 || (uVar3 == 0xc5))) {
        uVar3 = PlayerAvatar_GetTransitionFlags(playerAvatar);
        Field_PlayerAvatar_OrrTransitionFlags(playerAvatar, 1);
        Field_PlayerAvatar_ApplyTransitionFlags(playerAvatar);
        Field_PlayerAvatar_OrrTransitionFlags(playerAvatar, uVar3);
    }
    return;
}

SysTask *Field_PlayerMovementSavingSet(FieldSystem *fieldSystem) {
    LocalMapObject *object;
    PlayerAvatar *v2;
    SysTask *sysTask;
    s32 v0;
    s32 flags;
    PlayerMovementSavingWork *v1;

    v2 = fieldSystem->playerAvatar;
    v0 = PlayerAvatar_GetState(v2);
    if ((v0 != 0) && (v0 != 3)) {
        return NULL;
    }
    object = PlayerAvatar_GetMapObject(v2);
    v1 = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(PlayerMovementSavingWork));
    v1->unk_00 = 0;
    v1->fieldSystem = fieldSystem;
    v1->playerAvatar = v2;
    v1->playerState = v0;
    if (v0 == 0) {
        flags = 0x80;
    } else if (v0 == 3) {
        flags = 0x4000;
    } else {
        GF_AssertFail();
        flags = 0x80;
    }
    MapObject_UnpauseMovement(object);
    Field_PlayerAvatar_OrrTransitionFlags(v2, flags);
    Field_PlayerAvatar_ApplyTransitionFlags(v2);
    sysTask = SysTask_CreateOnMainQueue(ov01_021F3030, v1, 0xFFFFU);
    if (sysTask == NULL) {
        GF_AssertFail();
    }
    return sysTask;
}

void Field_PlayerMovementSavingClear(SysTask *task) {
    s32 v0;
    PlayerAvatar *v1;
    PlayerMovementSavingWork *ptr;

    if (task != NULL) {
        ptr = SysTask_GetData(task);
        v0 = ptr->playerState;
        v1 = ptr->playerAvatar;
        if (v0 == 0) {
            ov01_021F1B38(v1);
        } else if (v0 == 3) {
            ov01_021F1C30(v1);
        } else {
            GF_AssertFail();
            ov01_021F1B38(v1);
        }
        Heap_Free(ptr);
        SysTask_Destroy(task);
    }
}
