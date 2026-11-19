#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_02260AD4_t;

struct UnkStruct_ov70_02269204_t;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

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
    Field3dObject *unk_D8[4];
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
    Field3dObject unk_0C[18];
    const UnkStruct_ov70_0226E5A4 *unk_87C[18];
    fx32 unk_8C4[18][2];
    UnkStruct_ov70_0226BC40 unk_954;
} UnkStruct_ov70_02269800;

struct UnkStruct_ov70_02269204_t {
    WiFiPlazaSystem *unk_00;
    UnkStruct_ov70_0225CA20 *unk_04;
    UnkStruct_ov70_02260AD4 *unk_08;
    UnkStruct_ov70_0225C894 *unk_0C;
    UnkStruct_ov70_02269800 unk_10[20];
    Field3dModel unk_10550[15];
    void *unk_10640[39];
    Field3DModelAnimation unk_106DC[16];
    NNSFndAllocator unk_1081C;
};

void ov49_02265BE8(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3, fx32 param4);

BOOL ov49_0226786C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3);

BOOL ov49_0226786C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3) {
    if (param1->unk_954.val1.unk_0C[param2] != param3) {
        param1->unk_954.val1.unk_0C[param2] = param3;
        param1->unk_954.val1.unk_08[param2] = 0;
        ov49_02265BE8(param0, param1, param2, 0, (param3 * (FX32_ONE * 2)));
        return 1;
    }

    return 0;
}

void sub_020182A0(Field3dObject *obj, BOOL visible);

fx32 ov49_02265C40(const UnkStruct_ov70_02269204 *param0, const UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3);

void ov49_0226789C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2);

void ov49_0226789C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2) {
    fx32 v0;
    u32 v1 = param1->unk_954.val1.unk_0C[param2];

    if (param1->unk_954.val1.unk_08[param2] <= 0) {
        v0 = ov49_02265C40(param0, param1, param2, 0);

        if (v0 == (v1 * (FX32_ONE * 2))) {
            v0 = (v1 * (FX32_ONE * 2)) + FX32_ONE;
        } else {
            if (v1 != 4) {
                v0 = (v1 * (FX32_ONE * 2));
            } else {
                sub_020182A0(&param1->unk_0C[param2], 0);
            }
        }

        ov49_02265BE8(param0, param1, param2, 0, v0);

        param1->unk_954.val1.unk_08[param2] = 8;
    }

    param1->unk_954.val1.unk_08[param2]--;
}
