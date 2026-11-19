#include "global.h"

#include "field_system_def.h"

#include "heap.h"
#include "map_header.h"
#include "map_object.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "task.h"
#include "unk_02055BF0.h"
#include "unk_02062108.h"

s32 ov01_021EA128(TaskManager *taskMan);

s32 ov01_021EA128(TaskManager *taskMan) {
    FieldSystem *fieldSystem;
    LocalMapObject *localMapObject;
    LocalMapObject *v1;
    LocalMapObject *object;
    u8 *ptr;
    u8 v0;
    u8 v2;

    fieldSystem = TaskManager_GetFieldSystem(taskMan);
    ptr = TaskManager_GetEnvironment(taskMan);
    v0 = *ptr;
    switch (v0) {
    case 0:
        object = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if ((u8)PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar) == 1) {
            MapObject_SetVisible(object, 1);
            v2 = 1;
        } else {
            MapObject_SetVisible(object, 0);
            v2 = 3;
        }
        *ptr = v2;
        NewFieldFadeEnvironment(taskMan, FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_FIELD2);
    default:
        break;
    case 1:
        localMapObject = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        MapObject_SetVisible(localMapObject, 0);
        MapObject_SetHeldMovement(localMapObject, 0xDU);
        *ptr += 1;
        break;
    case 2:
        v1 = PlayerAvatar_GetMapObject(fieldSystem->playerAvatar);
        if (MapObject_IsMovementPaused(v1) == 1) {
            MapObject_ClearHeldMovementIfActive(v1);
            *ptr += 1;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished() != 0) {
            Heap_Free(ptr);
            return 1;
        }
        break;
    }
    return 0;
}

u32 ov01_021EA1F4(FieldSystem *fieldSystem, s32 param_2);

u32 ov01_021EA1F4(FieldSystem *fieldSystem, s32 param_2) {
    BOOL BVar1;

    BVar1 = MapHeader_IsInBuilding(fieldSystem->location->mapId);
    if (BVar1 != 0) {
        if (((param_2 == 0x3f) || (param_2 == 0xa2)) || (param_2 == 0xc5)) {
            return 1;
        }
    } else {
        if (param_2 == 0x7c) {
            return 1;
        }
    }
    return 0;
}
