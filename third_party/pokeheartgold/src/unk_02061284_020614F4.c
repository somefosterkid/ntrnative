#include "global.h"

struct MapObject;

typedef struct MapObject MapObject;

u32 MapObject_GetMovement(const MapObject *mapObj);

int MapObject_GetXRange(const MapObject *mapObj);

int MapObject_GetYRange(const MapObject *mapObj);

int MapObject_GetInitialX(const MapObject *mapObj);

int MapObject_GetInitialZ(const MapObject *mapObj);

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_020647A0;

static void sub_020614F4(MapObject *mapObj, UnkStruct_020647A0 *param1);

static void sub_020614F4(MapObject *mapObj, UnkStruct_020647A0 *param1) {
    int v0, v1, v2, v3, v4;

    v1 = MapObject_GetInitialX(mapObj);
    v2 = MapObject_GetInitialZ(mapObj);
    v3 = MapObject_GetXRange(mapObj);
    v4 = MapObject_GetYRange(mapObj);
    v0 = MapObject_GetMovement(mapObj);

    switch (v0) {
    case 0x6:
        param1->unk_00 = v1 - v3;
        param1->unk_08 = v1;
        param1->unk_04 = v2 - v4;
        param1->unk_0C = v2;
        break;
    case 0x7:
        param1->unk_00 = v1;
        param1->unk_08 = v1 + v3;
        param1->unk_04 = v2 - v4;
        param1->unk_0C = v2;
        break;
    case 0x8:
        param1->unk_00 = v1 - v3;
        param1->unk_08 = v1;
        param1->unk_04 = v2;
        param1->unk_0C = v2 + v4;
        break;
    case 0x9:
        param1->unk_00 = v1;
        param1->unk_08 = v1 + v3;
        param1->unk_04 = v2;
        param1->unk_0C = v2 + v4;
        break;
    case 0xa:
        param1->unk_00 = v1 - v3;
        param1->unk_08 = v1;
        param1->unk_04 = v2 - v4;
        param1->unk_0C = v2 + v4;
        break;
    case 0xb:
        param1->unk_00 = v1;
        param1->unk_08 = v1 + v3;
        param1->unk_04 = v2 - v4;
        param1->unk_0C = v2 + v4;
        break;
    case 0xc:
        param1->unk_00 = v1 - v3;
        param1->unk_08 = v1 + v3;
        param1->unk_04 = v2 - v4;
        param1->unk_0C = v2;
        break;
    case 0xd:
        param1->unk_00 = v1 - v3;
        param1->unk_08 = v1 + v3;
        param1->unk_04 = v2;
        param1->unk_0C = v2 + v4;
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
    }
}

int MapObject_GetXCoord(const MapObject *mapObj);

int MapObject_GetZCoord(const MapObject *mapObj);

int GetDeltaXByFacingDirection(int dir);

int GetDeltaYByFacingDirection(int dir);

int sub_020615F0(MapObject *mapObj, int param1);

int sub_020615F0(MapObject *mapObj, int param1) {
    int v0, v1;
    UnkStruct_020647A0 v2;

    sub_020614F4(mapObj, &v2);

    v0 = MapObject_GetXCoord(mapObj) + GetDeltaXByFacingDirection(param1);
    v1 = MapObject_GetZCoord(mapObj) + GetDeltaYByFacingDirection(param1);

    if ((v2.unk_00 > v0) || (v2.unk_08 < v0)) {
        return 0;
    }

    if ((v2.unk_04 > v1) || (v2.unk_0C < v1)) {
        return 0;
    }

    return 1;
}
