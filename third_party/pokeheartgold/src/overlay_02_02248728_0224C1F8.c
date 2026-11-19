#include "global.h"

#include "field_types_def.h"
#include "heap.h"
#include "overlay_02.h"
#include "task.h"
#include "constants/heap.h"
#include "screen_fade.h"
#include "unk_02062108.h"
#include "overlay_01.h"
#include "player_avatar.h"
#include "pokemon_types_def.h"
#include "overlay_01_021FCD2C.h"

extern s32 (*ov02_022536F0[4])(TaskManager *, FieldSystem *, FieldMoveTaskEnvironment *);

extern u32 ov02_02253794;
u32 ov02_0224C234(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);
u32 ov02_0224C2A8(void *param_1, void *param_2, FieldMoveTaskEnvironment *param_3);
extern u32 ov02_02253884;
u32 ov02_0224C2EC(void *param_1, void *param_2, FieldMoveTaskEnvironment *param_3);
u32 ov02_0224C338(void *param_1, void *param_2, FieldMoveTaskEnvironment *param_3);

s32 ov01_022062CC(FieldSystem *);

BOOL ov02_0224C1F8(TaskManager *taskManager) {
    FieldSystem *pFVar1;
    FieldMoveTaskEnvironment *ptr;
    s32 iVar2;

    pFVar1 = TaskManager_GetFieldSystem(taskManager);
    ptr = TaskManager_GetEnvironment(taskManager);
    do {
        iVar2 = ov02_022536F0[ptr->state](taskManager, pFVar1, ptr);
        if (iVar2 == 2) {
            Heap_Free(ptr);
            return 1;
        }
    } while (iVar2 == 1);
    return 0;
}

u32 ov02_0224C234(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
    SysTask *iVar1;
    EventObjectMovementMan *pEVar2;

    if (param_3->type == 2) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_FIELD1);
    } else {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7fff, 6, 1, HEAP_ID_FIELD1);
    }
    iVar1 = ov01_021FCD2C(param_2, HEAP_ID_FIELD1);
    param_3->cameraTask = iVar1;
    ov01_021FCD8C(iVar1, 1, 0xfff6a000, 1);
    pEVar2 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)&ov02_02253794);
    param_3->playerMovement = pEVar2;
    param_3->state = param_3->state + 1;
    return 0;
}

u32 ov02_0224C2A8(void *param_1, void *param_2, FieldMoveTaskEnvironment *param_3) {
    BOOL BVar1;
    EventObjectMovementMan *pEVar2;

    BVar1 = EventObjectMovementMan_IsFinish(param_3->playerMovement);
    if (BVar1 == 1) {
        EventObjectMovementMan_Delete(param_3->playerMovement);
        pEVar2 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)&ov02_02253794);
        param_3->playerMovement = pEVar2;
    }
    BVar1 = IsPaletteFadeFinished();
    if (BVar1 == 0) {
        return 0;
    }
    ov01_021FCD8C(param_3->cameraTask, 2, 0, 0x3c);
    param_3->state = param_3->state + 1;
    return 1;
}

u32 ov02_0224C2EC(void *param_1, void *param_2, FieldMoveTaskEnvironment *param_3) {
    BOOL BVar1;
    s32 iVar2;
    EventObjectMovementMan *pEVar3;

    BVar1 = EventObjectMovementMan_IsFinish(param_3->playerMovement);
    if (BVar1 == 0) {
        return 0;
    }
    EventObjectMovementMan_Delete(param_3->playerMovement);
    iVar2 = param_3->frame;
    param_3->frame = iVar2 + 1;
    if (iVar2 + 1 < 4) {
        pEVar3 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)&ov02_02253794);
        param_3->playerMovement = pEVar3;
        return 0;
    }
    pEVar3 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)&ov02_02253884);
    param_3->playerMovement = pEVar3;
    param_3->state = param_3->state + 1;
    return 0;
}

u32 ov02_0224C338(void *param_1, void *param_2, FieldMoveTaskEnvironment *param_3) {
    BOOL BVar1;
    s32 iVar2;

    BVar1 = EventObjectMovementMan_IsFinish(param_3->playerMovement);
    if (BVar1 == 0) {
        return 0;
    }
    iVar2 = ov01_021FCD6C(param_3->cameraTask);
    if (iVar2 == 0) {
        return 0;
    }
    ov01_021FCD78(param_3->cameraTask);
    EventObjectMovementMan_Delete(param_3->playerMovement);
    return 2;
}

FieldMoveTaskEnvironment *FieldMoveTask_CreateDigEnvironment(FieldSystem *fieldSystem, Pokemon *mon, u8 slotno, enum HeapID heapID) {
    FieldMoveTaskEnvironment *fieldMoveTaskEnvironment;

    fieldMoveTaskEnvironment = ov02_0224C660(heapID, sizeof(FieldMoveTaskEnvironment));
    fieldMoveTaskEnvironment->type = 1;
    fieldMoveTaskEnvironment->fieldSystem = fieldSystem;
    fieldMoveTaskEnvironment->mapObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
    fieldMoveTaskEnvironment->mon = mon;
    if ((ov01_02206268(fieldSystem) != 0) && (slotno == ov01_022062CC(fieldSystem))) {
        fieldMoveTaskEnvironment->followMonActive = 1;
    } else {
        fieldMoveTaskEnvironment->followMonActive = 0;
    }
    return fieldMoveTaskEnvironment;
}
