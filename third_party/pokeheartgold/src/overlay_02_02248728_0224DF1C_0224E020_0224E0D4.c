#include "global.h"

#include "error_handling.h"
#include "follow_mon.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_02.h"
#include "task.h"
#include "unk_02062108.h"

MapObjectSwapTask *ov02_0224E0D4(LocalMapObject *arg0, LocalMapObject *arg1) {
    MapObjectSwapTask *v0;

    v0 = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(MapObjectSwapTask));
    v0->state = 0;
    v0->obj1 = arg0;
    v0->obj2 = arg1;
    return v0;
}

static s32 ov02_0224E224(MapObjectSwapPos *arg0, MapObjectSwapPos *arg1);

static s32 ov02_0224E26C(u32 arg0);

static s32 ov02_0224E2A0(u32 arg0);

static s32 ov02_0224E2D4(s32 arg0);

s32 ov02_0224E0EC(TaskManager *arg0);

s32 ov02_0224E0EC(TaskManager *taskManager) {
    FieldSystem *fieldSystem;
    s32 v1;
    u32 v0;
    u32 a1;
    MapObjectSwapTask *ptr;

    fieldSystem = TaskManager_GetFieldSystem(taskManager);
    ptr = TaskManager_GetEnvironment(taskManager);
    v0 = ptr->state;
    v1 = 0;
    switch (v0) {
    case 0:
        MapObject_UnpauseMovement(FollowMon_GetMapObject(fieldSystem));
        ptr->state = (u32)(ptr->state + 1);
    case 1:
        if ((MapObject_AreBitsSetForMovementScriptInit(ptr->obj1) != 0) && (MapObject_AreBitsSetForMovementScriptInit(ptr->obj2) != 0)) {
            MapObject_PauseMovement(FollowMon_GetMapObject(fieldSystem));
            ptr->state = (u32)(ptr->state + 1);
        }
        break;
    case 2:
        ptr->pos1.x = MapObject_GetXCoord(ptr->obj1);
        ptr->pos1.z = MapObject_GetZCoord(ptr->obj1);
        ptr->dir1 = MapObject_GetFacingDirection(ptr->obj1);
        ptr->pos2.x = MapObject_GetXCoord(ptr->obj2);
        ptr->pos2.z = MapObject_GetZCoord(ptr->obj2);
        a1 = ov02_0224E224(&ptr->pos1, &ptr->pos2);
        MapObject_SetHeldMovement(ptr->obj1, a1);
        MapObject_SetHeldMovement(ptr->obj2, ov02_0224E2D4(a1));
        ptr->state = (u32)(ptr->state + 1);
        break;
    case 3:
        if ((MapObject_AreBitsSetForMovementScriptInit(ptr->obj1) != 0) && (MapObject_AreBitsSetForMovementScriptInit(ptr->obj2) != 0)) {
            ptr->state = (u32)(ptr->state + 1);
        }
        break;
    case 4:
        MapObject_SetHeldMovement(ptr->obj1, ov02_0224E2A0((u8)MapObject_GetFacingDirection(ptr->obj1)));
        MapObject_SetHeldMovement(ptr->obj2, ov02_0224E26C((u8) ptr->dir1));
        ptr->state = (u32)(ptr->state + 1);
        break;
    case 5:
        if ((MapObject_AreBitsSetForMovementScriptInit(ptr->obj1) != 0) && (MapObject_AreBitsSetForMovementScriptInit(ptr->obj2) != 0)) {
            MapObject_ClearHeldMovementIfActive(ptr->obj1);
            MapObject_ClearHeldMovementIfActive(ptr->obj2);
            v1 = 1;
            Heap_Free(ptr);
        }
        break;
    }
    return v1;
}

static s32 ov02_0224E224(MapObjectSwapPos *arg0, MapObjectSwapPos *arg1) {
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 v4;

    v3 = *(s32 *)arg1;
    v2 = *(s32 *)arg0;
    v4 = 0xD;
    if (v2 == v3) {
        v1 = *(s32 *)((u8 *)arg1 + 4);
        v0 = *(s32 *)((u8 *)arg0 + 4);
        if (v0 > v1) {
            v4 = 0xC;
        } else if (v0 >= v1) {
            GF_AssertFail();
        }
    } else if (arg0->z == arg1->z) {
        if (v2 > v3) {
            v4 = 0xE;
        } else if (v2 < v3) {
            v4 = 0xF;
        } else {
            GF_AssertFail();
        }
    } else {
        GF_AssertFail();
    }
    return v4;
}

static s32 ov02_0224E26C(u32 arg0) {
    switch (arg0) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    default:
        GF_AssertFail();
        return 0;
    }
}

static s32 ov02_0224E2A0(u32 arg0) {
    switch (arg0) {
    case 0:
        return 1;
    case 1:
        return 0;
    case 2:
        return 3;
    case 3:
        return 2;
    default:
        GF_AssertFail();
        return 0;
    }
}

static s32 ov02_0224E2D4(s32 arg0) {
    u32 v0;

    v0 = arg0 - 0xC;
    switch (v0) {
    case 0:
        return 0xD;
    case 1:
        return 0xC;
    case 2:
        return 0xF;
    case 3:
        return 0xE;
    default:
        GF_AssertFail();
        return 0;
    }
}

s32 ov02_0224E308(s32 arg0);

s32 ov02_0224E308(s32 arg0) {
    if (arg0 == 0x165) {
        return 1;
    }
    return 0;
}
