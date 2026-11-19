#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

struct MapObject;

typedef struct MapObject MapObject;

int MapObject_GetFacingDirection(const MapObject *mapObj);

u32 sub_0205F330(const MapObject *mapObj);

void *sub_0205F40C(MapObject *mapObj);

typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
} UnkStruct_ov5_021ED01C;

int ov01_021FA2D4(MapObject *param0);

BOOL ov01_021FA3E8(MapObject *param0, Billboard *param1);

typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
    Billboard *unk_04;
    UnkStruct_ov5_021ED01C unk_08;
} UnkStruct_ov5_021EB2EC;

void ov01_021F8C88(MapObject *mapObj, Billboard *param1);

extern void (*const ov01_02208B0C[])(MapObject *, Billboard *, UnkStruct_ov5_021EB2EC *, int);

void ov01_021F7C14(MapObject *mapObj);

void sub_02023EE0(Billboard *billboard, int animNum);
int sub_02023F04(Billboard *billboard, fx32 numFrames);
void sub_02023F40(Billboard *billboard, fx32 animFrameNum);
int ov01_021FA44C(int param0);
void ov01_021F8C30(Billboard *param0);
void ov01_021F7C80(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3);

extern void (*const Unk_ov5_021FF3D4[])(MapObject *, Billboard *, UnkStruct_ov5_021EB2EC *, int);
extern void (*const Unk_ov5_021FF420[])(MapObject *, Billboard *, UnkStruct_ov5_021EB2EC *, int);
void ov01_021F7CBC(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3);

void ov01_021F7C14(MapObject *mapObj) {
    int v0;
    int v1;
    UnkStruct_ov5_021EB2EC *v2 = sub_0205F40C(mapObj);
    Billboard *v3 = v2->unk_04;
    if (ov01_021FA2D4(mapObj) == 1) {
        return;
        v2 = sub_0205F40C(mapObj);
    }
    if (v3 == 0L) {
        return;
    }
    v0 = MapObject_GetFacingDirection(mapObj);
    v1 = sub_0205F330(mapObj);
    (v1 < (0x9 + 1)) ? ((void)0) : (GF_AssertFail());
    ov01_02208B0C[v1](mapObj, v3, v2, v0);
    v2->unk_02 = v0;
    v2->unk_03 = sub_0205F330(mapObj);
    ov01_021FA3E8(mapObj, v3);
    ov01_021F8C88(mapObj, v3);
}

void ov01_021F7C80(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3) {
    if ((param3 != param2->unk_02) || (param2->unk_03 == 0x9)) {
        int v0 = ov01_021FA44C(param3);

        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
    } else {
        ov01_021F8C30(param1);
    }

    sub_02023F04(param1, 0);
}

void ov01_021F7CBC(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3) {
    if (param2->unk_03 != 0x9) {
        sub_02023EE0(param1, 4);
        sub_02023F40(param1, 0);
    }

    sub_02023F04(param1, ((fx32)0x0000000000001000L));
}
