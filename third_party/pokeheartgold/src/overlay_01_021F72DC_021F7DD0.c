#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

struct MapObject;

typedef struct MapObject MapObject;

void *sub_0205F40C(MapObject *mapObj);

void MapObject_SetFacingVector(MapObject *mapObj, const VecFx32 *vec);

typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
} UnkStruct_ov5_021ED01C;

void ov01_021F95A8(const MapObject *param0, Billboard **param1);

typedef struct {
    s8 unk_00;
    s8 unk_01;
    s8 unk_02;
    u8 unk_03_0 : 1;
    u8 unk_03_1 : 7;
    Billboard *unk_04;
    UnkStruct_ov5_021ED01C unk_08;
} UnkStruct_ov5_021EBA0C;

void ov01_021F7DD0(MapObject *mapObj);

void MapObject_SetFlagsBits(MapObject *mapObj, u32 flag);
void ov01_021F9610(Billboard *param0, UnkStruct_ov5_021ED01C *param1);
typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
    Billboard *unk_04;
    UnkStruct_ov5_021ED01C unk_08;
} UnkStruct_ov5_021EB2EC;
extern void (*const Unk_ov5_021FF3D4[])(MapObject *, Billboard *, UnkStruct_ov5_021EB2EC *, int);
extern void (*const Unk_ov5_021FF420[])(MapObject *, Billboard *, UnkStruct_ov5_021EB2EC *, int);
void ov01_021F7DFC(MapObject *mapObj);

void ov01_021F7DD0(MapObject *mapObj) {
    UnkStruct_ov5_021EBA0C *v0;
    VecFx32 v1 = { 0, 0, 0 };

    v0 = sub_0205F40C(mapObj);

    ov01_021F95A8(mapObj, &v0->unk_04);
    MapObject_SetFacingVector(mapObj, &v1);
}

void ov01_021F7DFC(MapObject *mapObj) {
    UnkStruct_ov5_021EB2EC *v0 = sub_0205F40C(mapObj);

    if (v0->unk_04 != 0L) {
        ov01_021F9610(v0->unk_04, &v0->unk_08);
    }

    ov01_021F95A8(mapObj, &v0->unk_04);
    MapObject_SetFlagsBits(mapObj, (1 << 21));
}
