#include "global.h"

#include "error_handling.h"
#include "field_types_def.h"
#include "map_object.h"
#include "unk_02054648.h"

void ov01_02205604(LocalMapObject *object, s32 *p_x, s32 *p_y);

struct MapObject;
struct ObjectEventGfxRenderDetailsEntry;
typedef struct MapObject MapObject;
void ov01_0220329C(MapObject *mapObj, int param1);
void ov01_02205784(MapObject *mapObj);

void ov01_02205604(LocalMapObject *object, s32 *p_x, s32 *p_y) {
    u8 uVar1;
    u32 uVar2;
    uVar1 = MapObject_GetFacingDirection(object);
    uVar2 = MapObject_GetXCoord(object);
    *p_x = uVar2;
    uVar2 = MapObject_GetZCoord(object);
    *p_y = uVar2;
    switch (uVar1) {
    case 0:
        *p_y = (*p_y) + 1;
        return;

    case 1:
        *p_y = (*p_y) + (-1);
        return;

    case 2:
        *p_x = (*p_x) + 1;
        return;

    case 3:
        *p_x = (*p_x) + (-1);
        return;

    default:
        GF_AssertFail();
        return;
    }
}

void ov01_02205664(LocalMapObject *param_1, u32 *param_2, u32 *param_3);

void ov01_02205664(LocalMapObject *param_1, u32 *param_2, u32 *param_3) {
    u8 uVar1;
    u32 uVar2;
    uVar1 = MapObject_GetFacingDirection(param_1);
    uVar2 = MapObject_GetPreviousXCoord(param_1);
    *param_2 = uVar2;
    uVar2 = MapObject_GetPreviousZCoord(param_1);
    *param_3 = uVar2;
    switch (uVar1) {
    case 0:
        *param_3 = (*param_3) + 1;
        return;

    case 1:
        *param_3 = (*param_3) - 1;
        return;

    case 2:
        *param_2 = (*param_2) + 1;
        return;

    case 3:
        *param_2 = (*param_2) - 1;
        return;

    default:
        GF_AssertFail();
        return;
    }
}

u8 ov01_022056C4(LocalMapObject *param_1, u32 param_2);

u8 ov01_022056C4(LocalMapObject *param_1, u32 param_2) {
    u8 uVar1;
    FieldSystem *fieldSystem;
    u32 x;
    u32 z;

    fieldSystem = MapObject_GetFieldSystem(param_1);
    x = MapObject_GetXCoord(param_1);
    z = MapObject_GetZCoord(param_1);
    switch (param_2) {
    case 0:
        z = z - 1;
        break;
    case 1:
        z = z + 1;
        break;
    case 3:
        x = x - 1;
        break;
    case 2:
        x = x + 1;
        break;
    case 4:
        x = x + 1;
        z = z + 1;
        break;
    case 5:
        x = x - 1;
        z = z + 1;
    }
    uVar1 = GetMetatileBehavior(fieldSystem, x, z);
    return uVar1;
}

void ov01_02205720(LocalMapObject *playerObj, LocalMapObject *tsurePokeObj, s32 a2, s32 a3);

void ov01_02205720(LocalMapObject *playerObj, LocalMapObject *tsurePokeObj, s32 a2, s32 a3) {
    VecFx32 VStack_20;
    s32 iStack_14;

    iStack_14 = a3;
    MapObject_CopyPositionVector(playerObj, &VStack_20);
    switch (a2) {
    case 0:
        VStack_20.z = VStack_20.z + -0x10000;
        break;
    case 1:
        VStack_20.z = VStack_20.z + 0x10000;
        break;
    case 3:
        VStack_20.x = VStack_20.x + 0x10000;
        break;
    case 2:
        VStack_20.x = VStack_20.x + -0x10000;
        break;
    }
    MapObject_SetPositionFromVectorAndDirection(tsurePokeObj, &VStack_20, a3);
    return;
}

void ov01_02205784(MapObject *mapObj) {
    ov01_0220329C(mapObj, 0);
}
