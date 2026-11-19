#include "global.h"

struct MapObject;

typedef struct MapObject MapObject;

void MapObject_SetFacingDirection(MapObject *mapObj, int dir);

int MapObject_GetFacingDirection(const MapObject *mapObj);

typedef struct {
    s8 unk_00;
    s8 unk_01;
    s8 unk_02;
    s8 unk_03;
    int unk_04;
} UnkStruct_0206496C;

int sub_020617AC(MapObject *mapObj, UnkStruct_0206496C *param1);

int sub_020617AC(MapObject *mapObj, UnkStruct_0206496C *param1) {
    int v0, v1, *v2;
    int v3[5] = { 0, 2, 1, 3, -1 };
    int v4[5] = { 0, 3, 1, 2, -1 };

    if (param1->unk_00 == 2) {
        v2 = v3;
    } else {
        v2 = v4;
    }

    v1 = MapObject_GetFacingDirection(mapObj);

    for (v0 = 0; v2[v0] != -1; v0++) {
        if (v1 == v2[v0]) {
            break;
        }
    }

    GF_ASSERT(v2[v0] != -1);

    v0++;

    if (v2[v0] == -1) {
        v0 = 0;
    }

    v1 = v2[v0];

    MapObject_SetFacingDirection(mapObj, v1);

    param1->unk_02 = 0;
    return 1;
}
