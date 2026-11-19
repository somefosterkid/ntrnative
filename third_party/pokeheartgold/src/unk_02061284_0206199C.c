#include "global.h"

struct MapObject;

typedef struct MapObject MapObject;

void *sub_0205F394(MapObject *mapObj);

typedef struct {
    s8 unk_00;
    s8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_02065550;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    UnkStruct_02065550 unk_04;
} UnkStruct_02064C28;

extern int (*const _020FD548[])(MapObject *, UnkStruct_02064C28 *);

int sub_020619C0(MapObject *mapObj, UnkStruct_02064C28 *param1);

void sub_0206199C(MapObject *mapObj);

void sub_0206199C(MapObject *mapObj) {
    UnkStruct_02064C28 *v0 = sub_0205F394(mapObj);

    while (_020FD548[v0->unk_00](mapObj, v0) == 1) {
        (void)0;
    }
}

u32 MapObject_GetInitialFacingDirection(const MapObject *mapObj);

void MapObject_SetFacingDirection(MapObject *mapObj, int dir);

void MapObject_SetNextFacingDirection(MapObject *mapObj, int dir);

int sub_020611F4(int param0);

int sub_02062050(MapObject *mapObj);

int sub_020619C0(MapObject *mapObj, UnkStruct_02064C28 *param1) {
    int v0 = MapObject_GetInitialFacingDirection(mapObj);

    if (param1->unk_02 == 1) {
        v0 = sub_020611F4(v0);
    }

    MapObject_SetNextFacingDirection(mapObj, v0);

    if (sub_02062050(mapObj) == 0) {
        MapObject_SetFacingDirection(mapObj, v0);
    }

    param1->unk_00 = 1;
    return 1;
}
