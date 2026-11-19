#include "global.h"

struct MapObject;

typedef struct MapObject MapObject;

void *sub_0205F394(MapObject *mapObj);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    s16 unk_04;
    u16 unk_06;
    MapObject *unk_08;
} UnkStruct_02069F48;

int sub_02066150(MapObject *mapObj, UnkStruct_02069F48 *param1);

extern int (*const _020FE3D4[])(MapObject *, UnkStruct_02069F48 *);

int sub_02066064(MapObject *mapObj, UnkStruct_02069F48 *param1);

int sub_020660A0(MapObject *mapObj, UnkStruct_02069F48 *param1);

void sub_02066024(MapObject *mapObj);

void sub_02066024(MapObject *mapObj) {
    UnkStruct_02069F48 *v0 = sub_0205F394(mapObj);

    if (sub_02066150(mapObj, v0) == 0) {
        return;
    }

    while (_020FE3D4[v0->unk_00](mapObj, v0) == 1) {
        (void)0;
    }
}
