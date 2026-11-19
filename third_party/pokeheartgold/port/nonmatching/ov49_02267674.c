#include "global.h"
struct Easy3DObject;
struct UnkStruct_ov70_0225CC54_t;
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
typedef struct Easy3DObject {
    NNSG3dRenderObj renderObj;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[ROTATION_AXIS_COUNT];
    u8 padding_76[2];
} Easy3DObject;
void Field3dObject_SetPosEx(Easy3DObject *obj, fx32 x, fx32 y, fx32 z);
typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;
void ov70_0225D054(const UnkStruct_ov70_0225CC54 *param0, VecFx32 *param1);
typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov70_0226E2EC;
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
    u8 unk_00;
    u8 unk_01;
    u8 unk_02[2];
} UnkStruct_ov70_0226E5A4;
typedef struct {
    s16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    UnkStruct_ov70_022692B4 unk_08[4];
    UnkStruct_ov70_0226949C unk_A8[4];
    Easy3DObject *unk_D8[4];
    const UnkStruct_ov70_0225CC54 *unk_E8;
} UnkStruct_ov70_0226BD64;
typedef union {
    struct {
        s8 unk_00;
        s8 unk_01;
        s16 unk_02;
        fx32 unk_04;
        u8 unk_08[3];
        u8 unk_0B;
        u8 unk_0C[3];
        u8 unk_0F;
        u8 unk_10;
        u8 unk_11;
        u8 unk_12[2];
        UnkStruct_ov70_0226949C unk_14[3];
        UnkStruct_ov70_022692B4 unk_38[3];
        UnkStruct_ov70_022692B4 unk_B0;
        UnkStruct_ov70_0226949C unk_D8;
    } val1;
    struct {
        s32 unk_00;
        s32 unk_04[3];
    } val2;
    struct {
        s8 unk_00;
        s8 unk_01;
        u8 unk_02;
        u8 unk_03;
        s8 unk_04[16];
        UnkStruct_ov70_022692B4 unk_14[16];
    } val3;
    struct {
        s8 unk_00;
        s8 unk_01;
        s8 unk_02[2];
    } val4;
    struct {
        s32 unk_00;
    } val5;
    struct {
        s16 unk_00;
        s16 unk_02;
    } val6;
    struct {
        UnkStruct_ov70_0226BD64 unk_00[4];
        const UnkStruct_ov70_0226E2EC *unk_3B0;
        u16 unk_3B4;
        u16 unk_3B6;
        u16 unk_3B8;
        u16 unk_3BA;
    } val7;
} UnkStruct_ov70_0226BC40;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    u32 unk_04;
    UnkStruct_ov70_0225CC54 *unk_08;
    Easy3DObject unk_0C[18];
    const UnkStruct_ov70_0226E5A4 *unk_87C[18];
    fx32 unk_8C4[18][2];
    UnkStruct_ov70_0226BC40 unk_954;
} UnkStruct_ov70_02269800;
static const UnkStruct_ov63_0222CC3C Unk_ov70_0226E340[3] = { { 2, 44 }, { 14, 44 }, { 8, 48 } };

void ov49_02267674(UnkStruct_ov70_02269800 *param0);
void ov49_02259154(const UnkStruct_ov70_0225CC54 *param0, VecFx32 *param1);
void sub_020182A8(Easy3DObject *obj, fx32 x, fx32 y, fx32 z);
void ov49_02267674(UnkStruct_ov70_02269800 *param0)
{
    VecFx32 v0;

    ov49_02259154(param0->unk_08, &v0);

    switch (param0->unk_954.val1.unk_01) {
    case 1:
        sub_020182A8(&param0->unk_0C[0], v0.x + FX32_CONST(Unk_ov70_0226E340[2].unk_00), v0.y + FX32_CONST(Unk_ov70_0226E340[2].unk_02), v0.z + (-FX32_CONST(8)));
        break;
    case 2:
        sub_020182A8(&param0->unk_0C[0], v0.x + FX32_CONST(Unk_ov70_0226E340[0].unk_00), v0.y + FX32_CONST(Unk_ov70_0226E340[0].unk_02), v0.z + (-FX32_CONST(8)));
        sub_020182A8(&param0->unk_0C[1], v0.x + FX32_CONST(Unk_ov70_0226E340[1].unk_00), v0.y + FX32_CONST(Unk_ov70_0226E340[1].unk_02), v0.z + (-FX32_CONST(8)));
        break;
    default:
    case 3:
        sub_020182A8(&param0->unk_0C[0], v0.x + FX32_CONST(Unk_ov70_0226E340[2].unk_00), v0.y + FX32_CONST(Unk_ov70_0226E340[2].unk_02), v0.z + (-FX32_CONST(8)));
        sub_020182A8(&param0->unk_0C[1], v0.x + FX32_CONST(Unk_ov70_0226E340[0].unk_00), v0.y + FX32_CONST(Unk_ov70_0226E340[0].unk_02), v0.z + (-FX32_CONST(8)));
        sub_020182A8(&param0->unk_0C[2], v0.x + FX32_CONST(Unk_ov70_0226E340[1].unk_00), v0.y + FX32_CONST(Unk_ov70_0226E340[1].unk_02), v0.z + (-FX32_CONST(8)));
        break;
    }
}
