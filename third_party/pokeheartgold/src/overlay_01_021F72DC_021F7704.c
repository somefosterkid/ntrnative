#include "global.h"

#include "map_object.h"
#include "overlay_01.h"
#include "field_types_def.h"
#include "overlay_01_021F944C.h"


Billboard *ov01_021F771C(MapObjectManager *param_1);

void ov01_021F7704(LocalMapObject *object) {
    ((MapObjectBillboardWorkB *)sub_0205F40C(object))->unk_15 = 0;
    MapObject_GetFacingVector(object)->y = 0;
}

Billboard *ov01_021F771C(MapObjectManager *param_1) {
    LocalMapObject *object;
    MapObjectBillboardWorkB *work;

    object = MapObjectManager_GetFirstActiveObjectByID(param_1, 0xfd);
    work = (MapObjectBillboardWorkB *)sub_0205F40C(object);
    return work->billboard;
}
