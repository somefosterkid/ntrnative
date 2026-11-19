#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "error_handling.h"

struct UnkStruct_ov70_02260E8C_t;

typedef struct UnkStruct_ov70_02260E8C_t UnkStruct_ov70_02260E8C;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_02018198(Field3DModelAnimation *anim, fx32 frame);

void sub_020181D4(Field3dObject *obj, Field3DModelAnimation *anim);

void sub_020181E0(Field3dObject *obj, Field3DModelAnimation *anim);

void sub_020181EC(Field3dObject *obj);

BOOL ov49_022588A0(const Field3dModel *param0, const Field3dObject *param1);

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

void ov49_0225DAFC(UnkStruct_ov70_02260E8C *param0, UnkStruct_ov70_022618C8 *param1);

void ov49_0225DAFC(UnkStruct_ov70_02260E8C *param0, UnkStruct_ov70_022618C8 *param1) {
    BOOL v0;
    int v1;

    if (param0->unk_00 == 0) {
        return;
    }

    if (ov49_022588A0(&param1->unk_00[param0->unk_02], &param0->unk_04) == 0) {
        return;
    }

    GF_ASSERT(param0->unk_01 < 3);
    GF_ASSERT(param0->unk_02 < 2);

    param1->unk_00[param0->unk_02].texture = NNS_G3dGetTex(param1->unk_20[param0->unk_02][param0->unk_01]);
    v0 = NNS_G3dBindMdlSet(param1->unk_00[param0->unk_02].modelSet, param1->unk_00[param0->unk_02].texture);

    GF_ASSERT(v0);

    for (v1 = 0; v1 < 4; v1++) {
        if (param0->unk_7C[v1]) {
            sub_020181D4(&param0->unk_04, &param1->unk_38[param0->unk_02][v1]);
            sub_02018198(&param1->unk_38[param0->unk_02][v1], param0->unk_8C[v1]);
        }
    }

    sub_020181EC(&param0->unk_04);

    for (v1 = 0; v1 < 4; v1++) {
        if (param0->unk_7C[v1]) {
            sub_020181E0(&param0->unk_04, &param1->unk_38[param0->unk_02][v1]);
        }
    }

    NNS_G3dReleaseMdlSet(param1->unk_00[param0->unk_02].modelSet);
}

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_022610B8_t;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

typedef void (*UnkFuncPtr_ov70_02261C60)(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *);

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

UnkStruct_ov70_022610B8 *ov49_0225DBF8(UnkStruct_ov70_02260BB8 *param0);

UnkStruct_ov70_022610B8 *ov49_0225DBF8(UnkStruct_ov70_02260BB8 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_125; v0++) {
        if (param0->unk_120[v0].unk_00 == 0) {
            return &param0->unk_120[v0];
        }
    }

    GF_ASSERT(FALSE);
    return NULL;
}
