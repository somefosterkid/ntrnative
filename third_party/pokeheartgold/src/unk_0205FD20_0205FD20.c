#include "global.h"

struct MapObject;
typedef struct MapObject MapObject;
void sub_0205F41C(MapObject *mapObj);
void sub_02063A14(MapObject *param0);

void sub_0205FD20(MapObject *mapObj);

u32 MapObject_GetFlagsBitsMask(const MapObject *mapObj, u32 flag);
void sub_0205F430(MapObject *mapObj);
u32 sub_0205F5E8(const MapObject *mapObj, u32 flag);
int MapObject_CheckMovementPaused(const MapObject *mapObj);
void sub_02062400(MapObject *mapObj);
int sub_02063A1C(MapObject *param0);
int sub_0205FD98(const MapObject *mapObj);
void sub_0205FE0C(MapObject *mapObj);
void sub_0205FE24(MapObject *mapObj);
void sub_0205FE48(MapObject *mapObj);
void sub_0205FE6C(MapObject *mapObj);
void sub_0205FEA4(MapObject *mapObj);
BOOL sub_0205FD98(const MapObject *mapObj);
void sub_0205FD30(MapObject *mapObj);

void sub_0205FD20(MapObject *mapObj) {
    sub_0205F41C(mapObj);
    sub_02063A14(mapObj);
}

void sub_0205FD30(MapObject *mapObj) {
    if (sub_0205F5E8(mapObj, 1 << 1)) {
        return;
    }

    sub_0205FE0C(mapObj);
    sub_0205FE24(mapObj);
    sub_0205FE48(mapObj);

    if (MapObject_GetFlagsBitsMask(mapObj, (1 << 4))) {
        sub_02062400(mapObj);
    } else if (MapObject_CheckMovementPaused(mapObj) == 0 && sub_0205FD98(mapObj) == 1 && sub_02063A1C(mapObj) == 0) {
        sub_0205F430(mapObj);
    }

    sub_0205FE6C(mapObj);
    sub_0205FEA4(mapObj);
}
