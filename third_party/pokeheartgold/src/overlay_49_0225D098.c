#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "overlay_42.h"

struct UnkStruct_ov70_02260BB8_t;

struct UnkStruct_ov70_02260E8C_t;

struct UnkStruct_ov70_022610B8_t;

typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

typedef void (*UnkFuncPtr_ov70_02261C60)(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *);

typedef struct UnkStruct_ov70_02260E8C_t UnkStruct_ov70_02260E8C;

void ov49_0225D1C4(UnkStruct_ov70_022610B8 *param0, UnkStruct_ov44_02232914 param1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_020180E8(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator);

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

UnkStruct_ov70_022610B8 *ov49_0225DBF8(UnkStruct_ov70_02260BB8 *param0);

UnkStruct_ov70_022610B8 *ov49_0225D098(UnkStruct_ov70_02260BB8 *param0, int param1, u32 param2, u32 param3);

u16 ov49_0225D1C0(void *arg0);

void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);
void ov49_02258800(const UnkStruct_ov44_02232914 *param0, VecFx32 *param1);
void sub_020182B0(const Field3dObject *obj, fx32 *outX, fx32 *outY, fx32 *outZ);
void ov49_02258814(const VecFx32 *param0, UnkStruct_ov44_02232914 *param1);
UnkStruct_ov44_02232914 ov49_0225D1EC(const UnkStruct_ov70_022610B8 *param0);

UnkStruct_ov70_022610B8 *ov49_0225D098(UnkStruct_ov70_02260BB8 *param0, int param1, u32 param2, u32 param3) {
    UnkStruct_ov70_022610B8 *v0;
    int v1;

    v0 = ov49_0225DBF8(param0);

    sub_020181B0(&v0->unk_04, &param0->unk_29C.unk_00[param1]);

    for (v1 = 0; v1 < 3; v1++) {
        if (param0->unk_29C.unk_120[param1][v1] != 0L) {
            sub_020180E8(&v0->unk_7C[v1], &param0->unk_29C.unk_00[param1], param0->unk_29C.unk_120[param1][v1], &param0->unk_494);
        }
    }

    sub_020182A0(&v0->unk_04, 1);

    {
        UnkStruct_ov44_02232914 v2;

        v2.unk0 = (param2) * 16;
        v2.unk2 = (param3) * 16;

        ov49_0225D1C4(v0, v2);
    }

    v0->unk_00 = 1;
    v0->unk_02 = param1;
    v0->unk_CC = 20;
    v0->unk_DC = ((fx32)0x0000000000001000L);
    v0->unk_E0 = 0;
    v0->unk_E1 = 31;
    v0->unk_E2 = 31;

    return v0;
}

void ov49_0225D160(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1);

void ov49_0225D160(UnkStruct_ov70_02260BB8 *param0, UnkStruct_ov70_022610B8 *param1) {
    int v0;

    sub_020182A0(&param1->unk_04, 0);

    for (v0 = 0; v0 < 3; v0++) {
        if (param0->unk_29C.unk_120[param1->unk_02][v0] != 0L) {
            sub_020180E8(&param1->unk_7C[v0], &param0->unk_29C.unk_00[param1->unk_02], param0->unk_29C.unk_120[param1->unk_02][v0], &param0->unk_494);
        }
    }

    param1->unk_00 = 0;
}

u16 ov49_0225D1C0(void *arg0) {
    return *(u16 *)((u8 *)arg0 + 2);
}

void ov49_0225D1C4(UnkStruct_ov70_022610B8 *param0, UnkStruct_ov44_02232914 param1) {
    VecFx32 v0;

    ov49_02258800(&param1, &v0);
    sub_020182A8(&param0->unk_04, v0.x, v0.y, v0.z);
}

UnkStruct_ov44_02232914 ov49_0225D1EC(const UnkStruct_ov70_022610B8 *param0) {
    VecFx32 v0;
    UnkStruct_ov44_02232914 v1;

    sub_020182B0(&param0->unk_04, &v0.x, &v0.y, &v0.z);
    ov49_02258814(&v0, &v1);

    return v1;
}
