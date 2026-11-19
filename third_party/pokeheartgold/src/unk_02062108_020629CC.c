#include "global.h"

#include "unk_02005D10.h"

struct MapObject;

typedef struct MapObject MapObject;

void MapObject_SetFlagsBits(MapObject *mapObj, u32 flag);

void sub_0205F328(MapObject *mapObj, u32 param1);

void *sub_0205F3E4(MapObject *mapObj);

void sub_0205F484(MapObject *mapObj);

void MapObject_IncrementMovementStep(MapObject *mapObj);

void MapObject_SetFacingVector(MapObject *mapObj, const VecFx32 *vec);

void sub_02060F24(MapObject *mapObj, int dir);

void sub_02060F78(MapObject *mapObj);

void sub_0206101C(MapObject *mapObj, int dir, fx32 distance);

int sub_02061070(MapObject *mapObj);

typedef struct JumpMovementData {
    fx32 distance;
    fx32 unk_04;
    u16 unk_08;
    u16 unk_0A;
    s8 dir;
    s8 duration;
    u8 unused;
    s8 unk_0F;
} JumpMovementData;

extern const fx32 *_0210FACC[];

BOOL MapObjectMovementCmd092_Step1(MapObject *mapObj);

BOOL MapObject_CheckVisible(void *);

s32 sub_02062958(MapObject *, s32, s32, s16, u16, s16, u16);
s32 MapObjectMovementCmd044_Step0(void *arg0);
s32 MapObjectMovementCmd045_Step0(void *arg0);
s32 MapObjectMovementCmd046_Step0(void *arg0);
s32 MapObjectMovementCmd047_Step0(void *arg0);

BOOL MapObjectMovementCmd092_Step1(MapObject *mapObj) {
    JumpMovementData *data = sub_0205F3E4(mapObj);

    if (data->distance) {
        sub_0206101C(mapObj, data->dir, data->distance);
        sub_02061070(mapObj);

        if (data->unk_04 >= ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)))) {
            data->unk_04 = 0;
            sub_02060F24(mapObj, data->dir);
            MapObject_SetFlagsBits(mapObj, (1 << 2));
        }

        fx32 distance = data->distance;

        if (distance < 0) {
            distance = -distance;
        }

        data->unk_04 += distance;
    }

    data->unk_0A += data->unk_08;

    if (data->unk_0A > 0xF00) {
        data->unk_0A = 0xF00;
    }

    VecFx32 v2;
    u16 jumpHeightIndex = data->unk_0A / 0x100;
    const fx32 *jumpHeightsTable = _0210FACC[data->unk_0F];

    v2.x = 0;
    v2.y = jumpHeightsTable[jumpHeightIndex];
    v2.z = 0;

    MapObject_SetFacingVector(mapObj, &v2);

    if (--(data->duration) > 0) {
        return 0;
    }

    VecFx32 v5 = { 0, 0, 0 };
    MapObject_SetFacingVector(mapObj, &v5);

    MapObject_SetFlagsBits(mapObj, (1 << 3) | (1 << 17) | (1 << 5));
    sub_02060F78(mapObj);
    sub_0205F484(mapObj);
    sub_0205F328(mapObj, 0);
    MapObject_IncrementMovementStep(mapObj);
    if (!MapObject_CheckVisible(mapObj)) {
        PlaySE(1606);
    }

    return 1;
}

s32 MapObjectMovementCmd044_Step0(void *arg0) {
    sub_02062958(arg0, 0, 0, 0x10, 2, 0, 0x100);
    return 1;
}

s32 MapObjectMovementCmd045_Step0(void *arg0) {
    sub_02062958(arg0, 1, 0, 0x10, 2, 0, 0x100);
    return 1;
}

s32 MapObjectMovementCmd046_Step0(void *arg0) {
    sub_02062958(arg0, 2, 0, 0x10, 2, 0, 0x100);
    return 1;
}

s32 MapObjectMovementCmd047_Step0(void *arg0) {
    sub_02062958(arg0, 3, 0, 0x10, 2, 0, 0x100);
    return 1;
}
