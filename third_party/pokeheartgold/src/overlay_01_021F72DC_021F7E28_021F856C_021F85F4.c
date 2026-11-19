#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

struct MapObject;

typedef struct MapObject MapObject;

void sub_02023EE0(Billboard *billboard, int animNum);

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

fx32 sub_02023F70(const Billboard *billboard);

typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
} UnkStruct_ov5_021ED01C;

typedef struct {
    s8 unk_00;
    s8 unk_01;
    s8 unk_02;
    u8 unk_03_0 : 1;
    u8 unk_03_1 : 7;
    Billboard *unk_04;
    UnkStruct_ov5_021ED01C unk_08;
} UnkStruct_ov5_021EBA0C;

extern const int ov01_022089F8[4];

void ov01_021F85F4(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EBA0C *param2, int param3);

void ov01_021F85F4(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EBA0C *param2, int param3) {
    fx32 v0;

    if ((param3 != param2->unk_00) || (param2->unk_02 != 0x2)) {
        sub_02023EE0(param1, ov01_022089F8[param3]);
        sub_02023F40(param1, (FX32_ONE * 15));
        param2->unk_03_1 = 0;
    }

    v0 = sub_02023F70(param1);

    switch (param2->unk_03_1) {
    case 0:
        v0 -= (FX32_ONE);

        if (v0 <= (FX32_ONE * 10)) {
            param2->unk_03_1++;
        }
        break;
    case 1:
        v0 += (FX32_ONE);

        if (v0 >= (FX32_ONE * 15)) {
            param2->unk_03_1++;
            param2->unk_01 = 0;
        }
        break;
    case 2:
        param2->unk_01++;

        if (param2->unk_01 >= 30) {
            param2->unk_01 = 0;
            param2->unk_03_1 = 0;
        }
    }

    sub_02023F40(param1, v0);
    sub_02023F04(param1, 0);
}
