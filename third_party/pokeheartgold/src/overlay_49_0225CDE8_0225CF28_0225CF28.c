#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "error_handling.h"

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_02260E8C_t;

struct UnkStruct_ov70_022610B8_t;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

typedef void (*UnkFuncPtr_ov70_02261C60)(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *);

typedef struct UnkStruct_ov70_02260E8C_t UnkStruct_ov70_02260E8C;

void ov49_0225CFA8(UnkStruct_ov70_02260E8C *param0, const VecFx32 *param1);

void ov49_0225CFEC(UnkStruct_ov70_02260E8C *param0, const VecFx32 *param1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_020181B0(Field3dObject *obj, Field3dModel *model);

void sub_020182A0(Field3dObject *obj, BOOL visible);

typedef struct {
    BOOL unk_00;
    Field3dObject unk_04[2];
    u32 unk_F4[5];
    fx32 unk_108[5];
} UnkStruct_ov70_022615E8;

typedef struct {
    Field3dModel unk_00[2];
    Field3DModelAnimation unk_20[5];
    BOOL unk_84[5];
} UnkStruct_ov70_022615A4;

struct UnkStruct_ov70_02260E8C_t {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    Field3dObject unk_04;
    u32 unk_7C[4];
    fx32 unk_8C[4];
    VecFx32 unk_9C;
    VecFx32 unk_A8;
};

typedef struct {
    Field3dModel unk_00[2];
    void *unk_20[2][3];
    Field3DModelAnimation unk_38[2][4];
} UnkStruct_ov70_022618C8;

struct UnkStruct_ov70_022610B8_t {
    u16 unk_00;
    u16 unk_02;
    Field3dObject unk_04;
    Field3DModelAnimation unk_7C[3];
    u8 unk_B8[3];
    u8 unk_BB;
    u8 unk_BC[3];
    u8 unk_BF;
    fx32 unk_C0[3];
    u8 unk_CC;
    u8 unk_CD[3];
    UnkFuncPtr_ov70_02261C60 unk_D0[3];
    fx32 unk_DC;
    u8 unk_E0;
    u8 unk_E1;
    u8 unk_E2;
    u8 unk_E3;
};

typedef struct {
    Field3dModel unk_00[18];
    void *unk_120[18][3];
} UnkStruct_ov70_02261BB4;

struct UnkStruct_ov70_02260BB8_t {
    UnkStruct_ov70_022615E8 unk_00;
    UnkStruct_ov70_02260E8C *unk_11C;
    UnkStruct_ov70_022610B8 *unk_120;
    u8 unk_124;
    u8 unk_125;
    u8 unk_126;
    u8 unk_127;
    BOOL unk_128;
    UnkStruct_ov70_022615A4 unk_12C;
    UnkStruct_ov70_022618C8 unk_1C4;
    UnkStruct_ov70_02261BB4 unk_29C;
    NNSFndAllocator unk_494;
};

UnkStruct_ov70_02260E8C *ov49_0225D820(UnkStruct_ov70_02260BB8 *param0);

UnkStruct_ov70_02260E8C *ov49_0225CF28(UnkStruct_ov70_02260BB8 *param0, int param1, int param2, const VecFx32 *param3);

UnkStruct_ov70_02260E8C *ov49_0225CF28(UnkStruct_ov70_02260BB8 *param0, int param1, int param2, const VecFx32 *param3) {
    UnkStruct_ov70_02260E8C *v0;

    GF_ASSERT(param1 <= 2);
    GF_ASSERT(param2 <= 3);

    v0 = ov49_0225D820(param0);

    sub_020181B0(&v0->unk_04, &param0->unk_1C4.unk_00[param1]);
    sub_020182A0(&v0->unk_04, 1);

    ov49_0225CFA8(v0, param3);
    {
        VecFx32 v1 = { 0, 0, 0 };
        ov49_0225CFEC(v0, &v1);
    }

    v0->unk_01 = param2;
    v0->unk_02 = param1;
    v0->unk_00 = 1;

    {
        v0->unk_7C[0] = 1;
        v0->unk_7C[2] = 1;
    }

    return v0;
}

void ov49_0225CF94(UnkStruct_ov70_02260E8C *param0);

void ov49_0225CF94(UnkStruct_ov70_02260E8C *param0) {
    sub_020182A0(&param0->unk_04, 0);

    param0->unk_00 = 0;
}

void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void ov49_0225CFA8(UnkStruct_ov70_02260E8C *param0, const VecFx32 *param1) {
    param0->unk_9C = *param1;
    sub_020182A8(&param0->unk_04, param0->unk_9C.x + param0->unk_A8.x, param0->unk_9C.y + param0->unk_A8.y, param0->unk_9C.z + param0->unk_A8.z);
}

void ov49_0225CFEC(UnkStruct_ov70_02260E8C *param0, const VecFx32 *param1) {
    param0->unk_A8 = *param1;
    sub_020182A8(&param0->unk_04, param0->unk_9C.x + param0->unk_A8.x, param0->unk_9C.y + param0->unk_A8.y, param0->unk_9C.z + param0->unk_A8.z);
}
