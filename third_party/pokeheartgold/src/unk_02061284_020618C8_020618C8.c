#include "global.h"

struct MapObject;

typedef struct MapObject MapObject;

u32 MapObject_GetInitialFacingDirection(const MapObject *mapObj);

void MapObject_SetFacingDirection(MapObject *mapObj, int dir);

int MapObject_GetFacingDirection(const MapObject *mapObj);

int sub_020611F4(int param0);

typedef struct {
    s8 unk_00;
    s8 unk_01;
    s8 unk_02;
    s8 unk_03;
    int unk_04;
} UnkStruct_0206496C;

int sub_020618C8(MapObject *mapObj, UnkStruct_0206496C *param1);

typedef struct {
    int a[5];
} _020FD77C_Array;

extern const _020FD77C_Array _020FD77C;

typedef struct {
    int a[5];
} _020FD7A4_Array;

extern const _020FD7A4_Array _020FD7A4;

int sub_020618C8(MapObject *mapObj, UnkStruct_0206496C *param1) {
    int v0, v1, *v2;
    _020FD77C_Array v3 = _020FD77C;
    _020FD7A4_Array v4 = _020FD7A4;

    if (param1->unk_00 == 2) {
        v2 = v3.a;
    } else {
        v2 = v4.a;
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

    {
        int v5 = MapObject_GetFacingDirection(mapObj);
        int v6 = MapObject_GetInitialFacingDirection(mapObj);

        if (v5 == v6) {
            param1->unk_00 = sub_020611F4(param1->unk_00);
        }
    }

    param1->unk_02 = 0;
    return 1;
}
