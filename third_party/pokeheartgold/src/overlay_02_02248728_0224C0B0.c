#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "screen_fade.h"
#include "unk_02062108.h"
#include "field_warp_tasks.h"
#include "save_local_field_data.h"
#include "field_system.h"
#include "overlay_02.h"
#include "player_avatar.h"
#include "overlay_01_021FCD2C.h"

extern u32 ov02_02253794;

u32 ov02_0224C0B0(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

s32 ov02_0224C1B8(TaskManager *arg0, FieldSystem *arg1, void *arg2);


u32 ov02_0224C0B0(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
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
    if (param_3->type == 2) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_FIELD1);
    } else {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7fff, 6, 1, HEAP_ID_FIELD1);
    }
    param_3->state = param_3->state + 1;
    return 0;
}

u32 ov02_0224C14C(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

u32 ov02_0224C14C(void *param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
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

s32 ov02_0224C1B8(TaskManager *taskManager, FieldSystem *arg1, void *arg2) {
    sub_02053B04(taskManager, LocalFieldData_GetSpecialSpawnWarpPtr(Save_LocalFieldData_Get(arg1->saveData)), *(s32 *)((u8 *)arg2 + 0xC));
    return 2;
}

void *ov02_0224C1D8(FieldSystem *fieldSystem, enum HeapID heapID, int a2) {
    FieldMoveTaskEnvironment *v0;

    v0 = ov02_0224C660(heapID, sizeof(FieldMoveTaskEnvironment));
    v0->type = a2;
    v0->fieldSystem = fieldSystem;
    v0->mapObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    return v0;
}
