#include "global.h"
#include "field/overlay_01_021FB878.h"
#include "error_handling.h"

void sub_020181EC(s32 *);

void ov49_0225D804(s32 *param_1);

struct UnkStruct_ov70_02260BB8_t;
struct UnkStruct_ov70_02260E8C_t;
struct UnkStruct_ov70_022610B8_t;
typedef struct UnkStruct_ov70_02260BB8_t UnkStruct_ov70_02260BB8;
typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;
typedef void (*UnkFuncPtr_ov70_02261C60)(UnkStruct_ov70_02260BB8 *, UnkStruct_ov70_022610B8 *);
typedef struct UnkStruct_ov70_02260E8C_t UnkStruct_ov70_02260E8C;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
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

void ov49_0225D804(s32 *param_1) {
    s32 iVar1;
    s32 *piVar2;

    if (*param_1 != 0) {
        iVar1 = 0;
        piVar2 = param_1 + 1;
        do {
            sub_020181EC(piVar2);
            iVar1 = iVar1 + 1;
            piVar2 = piVar2 + 0x1e;
        } while (iVar1 < 2);
    }
    return;
}

UnkStruct_ov70_02260E8C *ov49_0225D820(UnkStruct_ov70_02260BB8 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_124; v0++) {
        if (param0->unk_11C[v0].unk_00 == 0) {
            return &param0->unk_11C[v0];
        }
    }

    GF_ASSERT(FALSE);
    return NULL;
}
