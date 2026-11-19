#include "global.h"
#include "billboard.h"

struct MapObject;

typedef struct MapObject MapObject;

void sub_02023EE0(Billboard *billboard, int animNum);

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

int ov01_021F9344(const MapObject *param0);

typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
} UnkStruct_ov5_021ED01C;

int ov01_021FA44C(int param0);

typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
    Billboard *unk_04;
    UnkStruct_ov5_021ED01C unk_08;
} UnkStruct_ov5_021EB2EC;

void ov01_021F7A90(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3);

typedef struct {
    fx32 a[7];
} ov01_02208A64_Array;

extern const ov01_02208A64_Array ov01_02208A64;

void ov01_021F7A90(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3) {
    if (param3 != param2->unk_02) {
        int v0 = ov01_021FA44C(param3);

        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);

        param2->unk_00 = 0;
    }

    if (param2->unk_03 != 0x6) {
        param2->unk_00 = 0;
    }

    if (ov01_021F9344(mapObj) == 0) {
        ov01_02208A64_Array v1 = ov01_02208A64;

        sub_02023F04(param1, v1.a[param2->unk_00]);
        param2->unk_00++;

        if (v1.a[param2->unk_00] == 0) {
            param2->unk_00 = 0;
        }
    }
}

void ov01_021F7B14(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3);

typedef struct {
    fx32 a[4];
} ov01_02208A08_Array;

extern const ov01_02208A08_Array ov01_02208A08;

void ov01_021F7B14(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3) {
    if (param3 != param2->unk_02) {
        int v0 = ov01_021FA44C(param3);

        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);

        param2->unk_00 = 0;
    }

    if (param2->unk_03 != 0x7) {
        param2->unk_00 = 0;
    }

    if (ov01_021F9344(mapObj) == 0) {
        ov01_02208A08_Array v1 = ov01_02208A08;

        sub_02023F04(param1, v1.a[param2->unk_00]);
        param2->unk_00++;

        if (v1.a[param2->unk_00] == 0) {
            param2->unk_00 = 0;
        }
    }
}

void ov01_021F7B90(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3);

typedef struct {
    fx32 a[8];
} ov01_02208A80_Array;

extern const ov01_02208A80_Array ov01_02208A80;

void ov01_021F7B90(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3) {
    if (param3 != param2->unk_02) {
        int v0 = ov01_021FA44C(param3);

        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);

        param2->unk_00 = 0;
    }

    if (param2->unk_03 != 0x8) {
        param2->unk_00 = 0;
    }

    if (ov01_021F9344(mapObj) == 0) {
        ov01_02208A80_Array v1 = ov01_02208A80;

        sub_02023F04(param1, v1.a[param2->unk_00]);

        param2->unk_00++;
        if (v1.a[param2->unk_00] == 0) {
            param2->unk_00 = 0;
        }
    }
}
