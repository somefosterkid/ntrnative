#include "global.h"
#include "unk_struct_02069_ca8.h"

struct MapObject;

typedef struct MapObject MapObject;

void MapObject_SetSingleMovement(MapObject *mapObj);

void MapObject_ClearSingleMovement(MapObject *mapObj);

void MapObject_ClearEndMovement(MapObject *mapObj);

int sub_02065D24(MapObject *mapObj, UnkStruct_02069CA8 *param1);

void sub_02065D58(MapObject *mapObj, UnkStruct_02069CA8 *param1);

int sub_02065FBC(MapObject *mapObj);

int sub_02065BE8(MapObject *mapObj, UnkStruct_02069CA8 *param1);

int sub_02065BE8(MapObject *mapObj, UnkStruct_02069CA8 *param1) {
    MapObject_ClearSingleMovement(mapObj);
    MapObject_ClearEndMovement(mapObj);

    if (sub_02065D24(mapObj, param1) == 1) {
        sub_02065D58(mapObj, param1);

        if (sub_02065FBC(mapObj) == 1) {
            MapObject_SetSingleMovement(mapObj);
            param1->unk_00++;
            return 1;
        }
    }

    return 0;
}
