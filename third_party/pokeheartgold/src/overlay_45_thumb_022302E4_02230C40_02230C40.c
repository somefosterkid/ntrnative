#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "filesystem.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov66_02234798_t;

s32 ov42_02228188(const UnkStruct_ov63_0222BEC0 *param0, int param1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_02023EE0(Billboard *billboard, int animNum);

int sub_02023F04(Billboard *billboard, fx32 numFrames);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

u32 ov45_022309C4(BOOL param0, u32 param1);

fx32 ov45_022309D0(u32 param0, s32 param1, u32 param2);

void ov45_02230C40(UnkStruct_ov66_02234798 *param0);

void ov45_02230C40(UnkStruct_ov66_02234798 *param0) {
    u32 v0;
    u32 v1;
    fx32 v2;
    u16 v3;
    u16 v4;

    v0 = ov42_02228188(param0->unk_04, 6);
    v1 = ov45_022309C4(0, v0);
    v3 = ov42_02228188(param0->unk_04, 9);
    v4 = ov42_02228188(param0->unk_04, 8);
    v2 = ov45_022309D0(v3, v4, 4);

    sub_02023EE0(param0->unk_08, v1);

    if (param0->unk_85 == v1) {
        sub_02023F40(param0->unk_08, 0);
        sub_02023F04(param0->unk_08, v2 + param0->unk_88);
    } else {
        sub_02023F40(param0->unk_08, 0);
        sub_02023F04(param0->unk_08, v2);
    }
}
