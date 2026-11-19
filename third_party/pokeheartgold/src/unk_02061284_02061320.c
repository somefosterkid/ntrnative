#include "global.h"

#include "field_types_def.h"
#include "unk_02061284.h"

void sub_02061284(LocalMapObject *param_1, u32 param_2);

void sub_02061320(LocalMapObject *param_1) {
    sub_02061284(param_1, 9);
    return;
}

void sub_0206132C(LocalMapObject *param_1) {
    sub_02061284(param_1, 10);
    return;
}

struct MapObject;

typedef struct MapObject MapObject;

void MapObject_SetFacingDirection(MapObject *mapObj, int dir);

void *sub_0205F394(MapObject *mapObj);

void sub_02060F78(MapObject *mapObj);

typedef struct {
    u16 unk_00;
    s16 unk_02;
    int unk_04;
} UnkStruct_0206450C;

int sub_02061F5C(MapObject *mapObj, int param1, int param2);

void sub_02061338(MapObject *mapObj);

void sub_02061338(MapObject *mapObj) {
    UnkStruct_0206450C *v0 = sub_0205F394(mapObj);
    int v1 = sub_02061F5C(mapObj, v0->unk_04, -1);

    if (v1 != -1) {
        MapObject_SetFacingDirection(mapObj, v1);
    } else {
        switch (v0->unk_00) {
        case 0:
            v0->unk_02--;

            if (v0->unk_02 <= 0) {
                v0->unk_02 = sub_02061E20(_020FD7B8, -1);
                MapObject_SetFacingDirection(mapObj, sub_02061E44(v0->unk_04, -1));
            }
        }
    }

    sub_02060F78(mapObj);
}

void sub_0206139C(void) {
    return;
}

void sub_020613D0(LocalMapObject *param_1, u32 param_2, u32 param_3, u32 param_4);

void sub_020613A0(LocalMapObject *param_1) {
    sub_020613D0(param_1, 0xc, 0xb, 0);
    return;
}
