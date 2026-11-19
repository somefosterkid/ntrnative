#include "global.h"
#include "field_types_def.h"
#include "unk_02061284.h"

struct MapObject;

typedef struct MapObject MapObject;

void MapObject_SetFacingDirection(MapObject *mapObj, int dir);

void *sub_0205F394(MapObject *mapObj);

typedef struct {
    int unk_00;
    int unk_04;
} UnkStruct_020648F4;

void sub_0206166C(MapObject *mapObj);

void sub_02061648(LocalMapObject *param_1, u32 param_2);

void sub_0206166C(MapObject *mapObj) {
    UnkStruct_020648F4 *v0 = sub_0205F394(mapObj);

    switch (v0->unk_04) {
    case 0:
        MapObject_SetFacingDirection(mapObj, v0->unk_00);
        v0->unk_04++;
        break;
    case 1:
        break;
    }
}

void sub_02061690(LocalMapObject *param_1) {
    sub_02061648(param_1, 0);
    return;
}

void sub_0206169C(LocalMapObject *param_1) {
    sub_02061648(param_1, 1);
    return;
}

void sub_020616A8(LocalMapObject *param_1) {
    sub_02061648(param_1, 2);
    return;
}
