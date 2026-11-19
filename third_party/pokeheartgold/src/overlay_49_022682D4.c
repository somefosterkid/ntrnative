#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

struct UnkStruct_ov70_0225CC54_t;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

void ov49_02259154(const UnkStruct_ov70_0225CC54 *param0, VecFx32 *param1);

typedef struct {
    s32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    fx32 unk_18;
    fx32 unk_1C;
    fx32 unk_20;
    fx32 unk_24;
} UnkStruct_ov70_022692B4;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
    fx32 unk_08;
} UnkStruct_ov70_0226949C;

typedef struct {
    s16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    UnkStruct_ov70_022692B4 unk_08[4];
    UnkStruct_ov70_0226949C unk_A8[4];
    Field3dObject *unk_D8[4];
    const UnkStruct_ov70_0225CC54 *unk_E8;
} UnkStruct_ov70_0226BD64;

void ov49_022683FC(UnkStruct_ov70_0226BD64 *param0, u32 param1, const VecFx32 *param2, fx32 param3, fx32 param4, fx32 param5, fx32 param6, u32 param7);

void ov49_022682D4(UnkStruct_ov70_0226BD64 *param0, const UnkStruct_ov70_0225CC54 *param1);

void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);
BOOL ov49_02265434(UnkStruct_ov70_022692B4 *param0, s32 param1);
void ov49_022655E0(const UnkStruct_ov70_022692B4 *param0, fx32 *param1, fx32 *param2, fx32 *param3);
BOOL ov49_02268334(UnkStruct_ov70_0226BD64 *param0);
void ov49_0226540C(UnkStruct_ov70_022692B4 *param0, fx32 param1, fx32 param2, fx32 param3, fx32 param4, fx32 param5, fx32 param6, s32 param7);

void ov49_022682D4(UnkStruct_ov70_0226BD64 *param0, const UnkStruct_ov70_0225CC54 *param1) {
    int v0;
    VecFx32 v1;

    ov49_02259154(param1, &v1);

    v1.x += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));
    v1.y += ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f))) + ((fx32)(((4) > 0) ? ((4) * (1 << 12) + 0.5f) : ((4) * (1 << 12) - 0.5f)));
    v1.z += (-((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))));

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        ov49_022683FC(param0, v0, &v1, ((fx32)(((13) > 0) ? ((13) * (1 << 12) + 0.5f) : ((13) * (1 << 12) - 0.5f))), ((fx32)(((5) > 0) ? ((5) * (1 << 12) + 0.5f) : ((5) * (1 << 12) - 0.5f))), ((fx32)(((7) > 0) ? ((7) * (1 << 12) + 0.5f) : ((7) * (1 << 12) - 0.5f))), ((fx32)(((14) > 0) ? ((14) * (1 << 12) + 0.5f) : ((14) * (1 << 12) - 0.5f))), 8);
    }
}

BOOL ov49_02268334(UnkStruct_ov70_0226BD64 *param0) {
    int v0;
    BOOL v1;
    VecFx32 v2;

    for (v0 = 0; v0 < param0->unk_04; v0++) {
        v1 = ov49_02265434(&param0->unk_08[v0], param0->unk_00);

        ov49_022655E0(&param0->unk_08[v0], &v2.x, &v2.y, &v2.z);
        sub_020182A8(param0->unk_D8[v0], v2.x, v2.y, v2.z);

        if (v1 == 1) {
            if (param0->unk_07 == 0) {
                ov49_022683FC(param0, v0, &v2, (-((fx32)(((3) > 0) ? ((3) * (1 << 12) + 0.5f) : ((3) * (1 << 12) - 0.5f)))), ((fx32)(((4) > 0) ? ((4) * (1 << 12) + 0.5f) : ((4) * (1 << 12) - 0.5f))), (-((fx32)(((3) > 0) ? ((3) * (1 << 12) + 0.5f) : ((3) * (1 << 12) - 0.5f)))), ((fx32)(((3) > 0) ? ((3) * (1 << 12) + 0.5f) : ((3) * (1 << 12) - 0.5f))), 3);
            } else {
                ov49_022683FC(param0, v0, &v2, ((fx32)(((10) > 0) ? ((10) * (1 << 12) + 0.5f) : ((10) * (1 << 12) - 0.5f))), ((fx32)(((5) > 0) ? ((5) * (1 << 12) + 0.5f) : ((5) * (1 << 12) - 0.5f))), ((fx32)(((6) > 0) ? ((6) * (1 << 12) + 0.5f) : ((6) * (1 << 12) - 0.5f))), ((fx32)(((10) > 0) ? ((10) * (1 << 12) + 0.5f) : ((10) * (1 << 12) - 0.5f))), 4);
            }
        }
    }

    if (v1 == 1) {
        if ((param0->unk_07 + 1) < 3) {
            param0->unk_07++;
            param0->unk_00 = 0;
        } else {
            return 1;
        }
    }

    return 0;
}

void ov49_022683FC(UnkStruct_ov70_0226BD64 *param0, u32 param1, const VecFx32 *param2, fx32 param3, fx32 param4, fx32 param5, fx32 param6, u32 param7) {
    VecFx32 v0;

    switch (param1) {
    case 0:
        v0.x = param2->x - param3;
        v0.y = param2->y + param4;
        v0.z = param2->z;
        break;
    case 1:
        v0.x = param2->x + param3;
        v0.y = param2->y + param4;
        v0.z = param2->z;
        break;
    case 2:
        v0.x = param2->x - param5;
        v0.y = param2->y + param6;
        v0.z = param2->z;
        break;
    case 3:
        v0.x = param2->x + param5;
        v0.y = param2->y + param6;
        v0.z = param2->z;
        break;
    }

    ov49_0226540C(&param0->unk_08[param1], param2->x, v0.x, param2->y, v0.y, param2->z, v0.z, param7);
    sub_020182A8(param0->unk_D8[param1], param2->x, param2->y, param2->z);
}
