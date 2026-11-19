#include "global.h"
#include "billboard.h"
#include "filesystem.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct Field3dModel;

struct Field3dObject;

struct NARC;

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov66_02234798_t;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct {
    Field3dModel unk_00;
    u32 unk_10;
} UnkStruct_ov66_02234958;

void ov45_02230CD8(UnkStruct_ov66_02234798 *param0);

void ov45_02230D20(UnkStruct_ov66_02234798 *param0);

void ov45_02230D5C(UnkStruct_ov66_02234798 *param0);

void ov45_022309E8(UnkStruct_ov66_02234958 *param0, NARC *param1, u32 param2, enum HeapID heapID);

void ov45_02230CB0(UnkStruct_ov66_02234798 *param0);

void ov45_02230CB0(UnkStruct_ov66_02234798 *param0) {
    extern void (*ov45_02254F1C[3])(UnkStruct_ov66_02234798 *);

    if ((param0->unk_00_0 == 0) && (param0->unk_01_0 == 1)) {
        ov45_02254F1C[param0->unk_01_1](param0);
    }
}

void sub_02023EE0(Billboard *billboard, int animNum);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

extern const u8 ov45_02254C34[4];

s32 ov45_022309C4(s32 arg0, s32 arg1);

void ov45_02230CD8(UnkStruct_ov66_02234798 *param0) {
    u32 v0;
    u32 v1;

    if ((param0->unk_02 % 4) == 0) {
        v0 = param0->unk_02 / 4;
        v1 = ov45_022309C4(1, ov45_02254C34[v0]);

        sub_02023EE0(param0->unk_08, v1);
        sub_02023F40(param0->unk_08, 0);
    }

    if ((param0->unk_02 + param0->unk_03) < (4 * 4)) {
        param0->unk_02 += param0->unk_03;
    } else {
        param0->unk_02 = 0;
    }
}
