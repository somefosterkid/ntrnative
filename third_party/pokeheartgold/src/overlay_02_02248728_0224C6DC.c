#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "field_system.h"
#include "screen_fade.h"
#include "unk_02005D10.h"
#include "unk_02062108.h"
#include "field_warp_tasks.h"
#include "save_local_field_data.h"
#include "unk_0203BA5C.h"
#include "overlay_01_021FCD2C.h"
#include "overlay_02.h"

extern u32 ov02_02253820;

u32 ov02_0224C6DC(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

s32 GetFlyWarpData(s32, Location *);

u32 ov02_0224C6DC(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
    BOOL BVar1;
    EventObjectMovementMan *pEVar2;

    BVar1 = EventObjectMovementMan_IsFinish(param_3->followMonMovement);
    if (BVar1 == 0) {
        return 0;
    }
    EventObjectMovementMan_Delete(param_3->followMonMovement);
    pEVar2 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)&ov02_02253820);
    param_3->playerMovement = pEVar2;
    pEVar2 = EventObjectMovementMan_Create(param_2->followMon.mapObject, (MovementScriptCommand *)&ov02_02253820);
    param_3->followMonMovement = pEVar2;
    param_3->state = param_3->state + 1;
    return 0;
}

u32 ov02_0224C71C(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

u32 ov02_0224C71C(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
    SysTask *iVar1;
    EventObjectMovementMan *pEVar2;

    iVar1 = ov01_021FCD2C(param_2, HEAP_ID_FIELD1);
    param_3->cameraTask = iVar1;
    ov01_021FCD8C(iVar1, 1, 0xfff6a000, 0xf);
    pEVar2 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)&ov02_02253820);
    param_3->playerMovement = pEVar2;
    param_3->state = param_3->state + 1;
    PlaySE(0x64d);
    return 0;
}

extern u32 ov02_02253794;

u32 ov02_0224C75C(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

u32 ov02_0224C75C(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
    BOOL BVar1;
    EventObjectMovementMan *pEVar2;
    s32 iVar3;

    BVar1 = EventObjectMovementMan_IsFinish(param_3->playerMovement);
    if (BVar1 == 0) {
        return 0;
    }
    EventObjectMovementMan_Delete(param_3->playerMovement);
    pEVar2 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)&ov02_02253794);
    param_3->playerMovement = pEVar2;
    if (param_3->followMonActive != 0) {
        EventObjectMovementMan_Delete(param_3->followMonMovement);
        pEVar2 = EventObjectMovementMan_Create(param_2->followMon.mapObject, (MovementScriptCommand *)&ov02_02253794);
        param_3->followMonMovement = pEVar2;
    }
    iVar3 = param_3->frame;
    param_3->frame = iVar3 + 1;
    if (iVar3 + 1 < 8) {
        return 0;
    }
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_FIELD1);
    param_3->state = param_3->state + 1;
    return 0;
}

u32 ov02_0224C7D4(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

u32 ov02_0224C7D4(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
    BOOL BVar1;
    EventObjectMovementMan *pEVar2;

    BVar1 = EventObjectMovementMan_IsFinish(param_3->playerMovement);
    if (BVar1 == 1) {
        EventObjectMovementMan_Delete(param_3->playerMovement);
        pEVar2 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)&ov02_02253794);
        param_3->playerMovement = pEVar2;
        if (param_3->followMonActive != 0) {
            EventObjectMovementMan_Delete(param_3->followMonMovement);
            pEVar2 = EventObjectMovementMan_Create(param_2->followMon.mapObject,
                (MovementScriptCommand *)&ov02_02253794);
            param_3->followMonMovement = pEVar2;
        }
    }
    BVar1 = IsPaletteFadeFinished();
    if (BVar1 == 0) {
        return 0;
    }
    EventObjectMovementMan_Delete(param_3->playerMovement);
    if (param_3->followMonActive != 0) {
        EventObjectMovementMan_Delete(param_3->followMonMovement);
    }
    ov01_021FCD78(param_3->cameraTask);
    param_3->state = param_3->state + 1;
    return 1;
}

s32 ov02_0224C840(TaskManager *taskManager, FieldSystem *arg1, FieldMoveTaskEnvironment *arg2) {
    Location location;
    LocalFieldData *localFieldData;
    u16 spawnId;

    localFieldData = Save_LocalFieldData_Get(arg1->saveData);
    spawnId = LocalFieldData_GetBlackoutSpawn(localFieldData);
    GetFlyWarpData(spawnId, &location);
    GetSpecialSpawnWarpData(spawnId, LocalFieldData_GetSpecialSpawnWarpPtr(localFieldData));
    sub_02053B04(taskManager, &location, arg2->type);
    return 2;
}
