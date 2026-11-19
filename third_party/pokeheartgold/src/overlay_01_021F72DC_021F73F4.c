#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

struct MapObject;
typedef struct MapObject MapObject;
void *sub_0205F40C(MapObject *mapObj);
typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
} UnkStruct_ov5_021ED01C;
void ov01_021F95A8(const MapObject *param0, Billboard **param1);
typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
    Billboard *unk_04;
    UnkStruct_ov5_021ED01C unk_08;
} UnkStruct_ov5_021EB2EC;
void MapObject_SetFlagsBits(MapObject *mapObj, u32 flag);
void ov01_021F9610(Billboard *param0, UnkStruct_ov5_021ED01C *param1);

void ov01_021F73F4(MapObject *mapObj);
void ov01_021F7408(MapObject *mapObj);

void ov01_021F73F4(MapObject *mapObj) {
    UnkStruct_ov5_021EB2EC *v0 = sub_0205F40C(mapObj);
    ov01_021F95A8(mapObj, &v0->unk_04);
}

void ov01_021F7408(MapObject *mapObj) {
    UnkStruct_ov5_021EB2EC *v0 = sub_0205F40C(mapObj);

    if (v0->unk_04 != 0L) {
        ov01_021F9610(v0->unk_04, &v0->unk_08);
    }

    ov01_021F95A8(mapObj, &v0->unk_04);
    MapObject_SetFlagsBits(mapObj, (1 << 21));
}
