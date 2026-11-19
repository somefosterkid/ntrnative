#include "global.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "constants/heap.h"
#include "field/overlay_01_021FB878.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

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

void sub_020182A0(Field3dObject *obj, BOOL visible);

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_ov70_0226E514;

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

extern const UnkStruct_ov70_0226E514 ov49_0226A67C[3][12];

UnkStruct_ov70_02269204 *ov70_02269190(WiFiPlazaSystem *param0, UnkStruct_ov70_0225C894 *param1, UnkStruct_ov70_02260AD4 *param2, UnkStruct_ov70_0225CA20 *param3, enum HeapID heapID, u32 heapID2);

BOOL ov49_02267AF0(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2);

BOOL ov49_02267AF0(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2) {
    u32 v1;
    UnkStruct_ov70_0226E514 v2;

    v1 = (param1->unk_02 * 12) / 48;

    if (v1 != param1->unk_954.val5.unk_00) {
        param1->unk_954.val5.unk_00 = v1;

        if (v1 > 0) {
            v2 = ov49_0226A67C[param2][v1 - 1];
            sub_020182A0(&param1->unk_0C[v2.unk_00], 0);
        }

        v2 = ov49_0226A67C[param2][v1];

        if (0 != v2.unk_02) {
            ov49_02265BE8(param0, param1, v2.unk_00, 0, ((fx32)(((v2.unk_02) > 0) ? ((v2.unk_02) * (1 << 12) + 0.5f) : ((v2.unk_02) * (1 << 12) - 0.5f))));
            ov49_02265BE8(param0, param1, v2.unk_00, 1, ((fx32)(((v2.unk_02) > 0) ? ((v2.unk_02) * (1 << 12) + 0.5f) : ((v2.unk_02) * (1 << 12) - 0.5f))));
            sub_020182A0(&param1->unk_0C[v2.unk_00], 1);
        } else {
            sub_020182A0(&param1->unk_0C[v2.unk_00], 0);
        }
    }

    if (param1->unk_02 < 48) {
        param1->unk_02++;
    }

    if (param1->unk_02 >= 48) {
        return 1;
    }

    return 0;
}
