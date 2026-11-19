#include "global.h"

struct MapObject;

typedef struct MapObject MapObject;

void MapObject_SetSingleMovement(MapObject *mapObj);

void MapObject_ClearSingleMovement(MapObject *mapObj);

void MapObject_ClearEndMovement(MapObject *mapObj);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    s16 unk_04;
    u16 unk_06;
    MapObject *unk_08;
} UnkStruct_02069F48;

int sub_020661F0(MapObject *mapObj, UnkStruct_02069F48 *param1);

int sub_0206623C(MapObject *mapObj, UnkStruct_02069F48 *param1);

int sub_02066064(MapObject *mapObj, UnkStruct_02069F48 *param1);

int sub_02066064(MapObject *mapObj, UnkStruct_02069F48 *param1) {
    MapObject_ClearSingleMovement(mapObj);
    MapObject_ClearEndMovement(mapObj);

    if (sub_020661F0(mapObj, param1) == 1) {
        if (sub_0206623C(mapObj, param1) == 1) {
            MapObject_SetSingleMovement(mapObj);
            param1->unk_00++;
            return 1;
        }
    }

    return 0;
}
