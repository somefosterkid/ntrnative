#include "global.h"

#include "map_object.h"
#include "overlay_01.h"
#include "field_types_def.h"


u32 ov01_021F771C(MapObjectManager *param_1);

void ov01_021F7704(LocalMapObject *object) {
    *(s8 *)((u8 *)(sub_0205F40C(object)) + 0x15) = 0;
    MapObject_GetFacingVector(object)->y = 0;
}

u32 ov01_021F771C(MapObjectManager *param_1) {
    LocalMapObject *object;
    u8 *puVar1;

    object = MapObjectManager_GetFirstActiveObjectByID(param_1, 0xfd);
    puVar1 = sub_0205F40C(object);
    return *(u32 *)puVar1;
}
