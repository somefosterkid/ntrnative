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

void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

void ov49_02259154(const UnkStruct_ov70_0225CC54 *param0, VecFx32 *param1);

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

void ov49_02265980(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, const UnkStruct_ov70_0226E5A4 *param3);

extern const UnkStruct_ov70_0226E5A4 ov49_0226A70C[39];

UnkStruct_ov70_02269204 *ov70_02269190(WiFiPlazaSystem *param0, UnkStruct_ov70_0225C894 *param1, UnkStruct_ov70_02260AD4 *param2, UnkStruct_ov70_0225CA20 *param3, enum HeapID heapID, u32 heapID2);

void ov49_02265CB0(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02265CB0(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    int v0;
    VecFx32 v1;

    for (v0 = 0; v0 < 3; v0++) {
        ov49_02265980(param0, param1, v0, &ov49_0226A70C[3 + v0]);
        ov49_02259154(param1->unk_08, &v1);

        v1.y += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))) + (FX_MulInline(((fx32)0x0000000000001000L) * v0, ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f)))));

        sub_020182A8(&param1->unk_0C[v0], v1.x, v1.y, v1.z);
        sub_020182A0(&param1->unk_0C[v0], 0);
    }

    param1->unk_954.val5.unk_00 = 0xff;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_020182E0(Field3dObject *obj, s16 angle, enum RotationAxis axis);
#else
void sub_020182E0(Field3dObject *obj, u16 angle, enum RotationAxis axis);
#endif

u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);

void ov49_02265D10(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02265D10(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    VecFx32 v0, v1;
    u32 v2;

    ov49_02265980(param0, param1, 1, &ov49_0226A70C[9]);
    ov49_02265980(param0, param1, 0, &ov49_0226A70C[12]);
    sub_020182A0(&param1->unk_0C[1], 0);
    ov49_02259154(param1->unk_08, &v0);

    v0.y += ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)));
    v0.x += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));
    v1 = v0;
    v1.y += ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)));

    param1->unk_954.val2.unk_04[0] = 8;

    v2 = ov49_02258E60(param1->unk_08, 6);

    switch (v2) {
    case 0:
        v0.z -= ((fx32)(((28) > 0) ? ((28) * (1 << 12) + 0.5f) : ((28) * (1 << 12) - 0.5f)));
        v1.z -= ((fx32)(((14) > 0) ? ((14) * (1 << 12) + 0.5f) : ((14) * (1 << 12) - 0.5f)));
        sub_020182E0(&param1->unk_0C[1], ((270 * 0xffff) / 360), (enum RotationAxis)(1));
        break;
    case 1:
        v0.z += ((fx32)(((24) > 0) ? ((24) * (1 << 12) + 0.5f) : ((24) * (1 << 12) - 0.5f)));
        v1.z += ((fx32)(((11) > 0) ? ((11) * (1 << 12) + 0.5f) : ((11) * (1 << 12) - 0.5f)));
        sub_020182E0(&param1->unk_0C[1], ((90 * 0xffff) / 360), (enum RotationAxis)(1));
        break;
    case 2:
        v0.x -= ((fx32)(((23) > 0) ? ((23) * (1 << 12) + 0.5f) : ((23) * (1 << 12) - 0.5f)));
        v1.x -= ((fx32)(((7) > 0) ? ((7) * (1 << 12) + 0.5f) : ((7) * (1 << 12) - 0.5f)));
        v1.z += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));
        sub_020182E0(&param1->unk_0C[1], ((0 * 0xffff) / 360), (enum RotationAxis)(1));
        break;
    case 3:
        v0.x += ((fx32)(((23) > 0) ? ((23) * (1 << 12) + 0.5f) : ((23) * (1 << 12) - 0.5f)));
        v1.x += ((fx32)(((7) > 0) ? ((7) * (1 << 12) + 0.5f) : ((7) * (1 << 12) - 0.5f)));
        v1.z += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));
        sub_020182E0(&param1->unk_0C[1], ((180 * 0xffff) / 360), (enum RotationAxis)(1));
        break;
    default:
        break;
    }

    sub_020182A8(&param1->unk_0C[1], v0.x, v0.y, v0.z);
    sub_020182A8(&param1->unk_0C[0], v1.x, v1.y, v1.z);
}

void ov49_02265E54(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02265E54(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    VecFx32 v0, v1;
    u32 v2;

    ov49_02265980(param0, param1, 1, &ov49_0226A70C[9]);
    ov49_02265980(param0, param1, 0, &ov49_0226A70C[12]);
    ov49_02265980(param0, param1, 2, &ov49_0226A70C[10]);
    sub_020182A0(&param1->unk_0C[1], 0);
    ov49_02259154(param1->unk_08, &v0);

    v0.y += ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)));
    v0.x += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));
    v1 = v0;
    v1.y += ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)));

    param1->unk_954.val2.unk_04[0] = 8;
    param1->unk_954.val2.unk_04[1] = 8;

    v2 = ov49_02258E60(param1->unk_08, 6);

    switch (v2) {
    case 0:
        v0.z -= ((fx32)(((28) > 0) ? ((28) * (1 << 12) + 0.5f) : ((28) * (1 << 12) - 0.5f)));
        v1.z -= ((fx32)(((14) > 0) ? ((14) * (1 << 12) + 0.5f) : ((14) * (1 << 12) - 0.5f)));
        sub_020182E0(&param1->unk_0C[1], (((270 - 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[2], (((270 + 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182A8(&param1->unk_0C[1], v0.x + ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))), v0.y, v0.z);
        sub_020182A8(&param1->unk_0C[2], v0.x - ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))), v0.y, v0.z);
        break;
    case 1:
        v0.z += ((fx32)(((24) > 0) ? ((24) * (1 << 12) + 0.5f) : ((24) * (1 << 12) - 0.5f)));
        v1.z += ((fx32)(((11) > 0) ? ((11) * (1 << 12) + 0.5f) : ((11) * (1 << 12) - 0.5f)));

        sub_020182E0(&param1->unk_0C[1], (((90 - 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[2], (((90 + 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182A8(&param1->unk_0C[1], v0.x - ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))), v0.y, v0.z);
        sub_020182A8(&param1->unk_0C[2], v0.x + ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))), v0.y, v0.z);
        break;
    case 2:
        v0.x -= ((fx32)(((23) > 0) ? ((23) * (1 << 12) + 0.5f) : ((23) * (1 << 12) - 0.5f)));
        v1.x -= ((fx32)(((7) > 0) ? ((7) * (1 << 12) + 0.5f) : ((7) * (1 << 12) - 0.5f)));
        v1.z += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));

        sub_020182E0(&param1->unk_0C[1], (((0 - 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[2], (((0 + 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182A8(&param1->unk_0C[1], v0.x, v0.y, v0.z - ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))));
        sub_020182A8(&param1->unk_0C[2], v0.x, v0.y, v0.z + ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))));
        break;
    case 3:
        v0.x += ((fx32)(((23) > 0) ? ((23) * (1 << 12) + 0.5f) : ((23) * (1 << 12) - 0.5f)));
        v1.x += ((fx32)(((7) > 0) ? ((7) * (1 << 12) + 0.5f) : ((7) * (1 << 12) - 0.5f)));
        v1.z += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));

        sub_020182E0(&param1->unk_0C[1], (((180 - 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[2], (((180 + 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182A8(&param1->unk_0C[1], v0.x, v0.y, v0.z + ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))));
        sub_020182A8(&param1->unk_0C[2], v0.x, v0.y, v0.z - ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))));
        break;
    default:
        break;
    }

    sub_020182A8(&param1->unk_0C[0], v1.x, v1.y, v1.z);
}

void ov49_02266088(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02266088(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    VecFx32 v0, v1;
    u32 v2;

    ov49_02265980(param0, param1, 1, &ov49_0226A70C[9]);
    ov49_02265980(param0, param1, 0, &ov49_0226A70C[12]);
    ov49_02265980(param0, param1, 2, &ov49_0226A70C[10]);
    ov49_02265980(param0, param1, 3, &ov49_0226A70C[11]);

    sub_020182A0(&param1->unk_0C[1], 0);

    param1->unk_954.val2.unk_04[0] = 8;
    param1->unk_954.val2.unk_04[1] = 8;
    param1->unk_954.val2.unk_04[2] = 8;

    ov49_02259154(param1->unk_08, &v0);

    v0.y += ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)));
    v0.x += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));
    v1 = v0;
    v1.y += ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)));
    v2 = ov49_02258E60(param1->unk_08, 6);

    switch (v2) {
    case 0:
        v0.z -= ((fx32)(((28) > 0) ? ((28) * (1 << 12) + 0.5f) : ((28) * (1 << 12) - 0.5f)));
        v1.z -= ((fx32)(((14) > 0) ? ((14) * (1 << 12) + 0.5f) : ((14) * (1 << 12) - 0.5f)));

        sub_020182E0(&param1->unk_0C[1], (((270 - 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[2], (((270 + 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[3], ((270 * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182A8(&param1->unk_0C[1], v0.x + ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))), v0.y, v0.z);
        sub_020182A8(&param1->unk_0C[2], v0.x - ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))), v0.y, v0.z);
        sub_020182A8(&param1->unk_0C[3], v0.x, v0.y, v0.z);
        break;
    case 1:
        v0.z += ((fx32)(((24) > 0) ? ((24) * (1 << 12) + 0.5f) : ((24) * (1 << 12) - 0.5f)));
        v1.z += ((fx32)(((11) > 0) ? ((11) * (1 << 12) + 0.5f) : ((11) * (1 << 12) - 0.5f)));

        sub_020182E0(&param1->unk_0C[1], (((90 - 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[2], (((90 + 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[3], ((90 * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182A8(&param1->unk_0C[1], v0.x - ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))), v0.y, v0.z);
        sub_020182A8(&param1->unk_0C[2], v0.x + ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))), v0.y, v0.z);
        sub_020182A8(&param1->unk_0C[3], v0.x, v0.y, v0.z);
        break;
    case 2:
        v0.x -= ((fx32)(((23) > 0) ? ((23) * (1 << 12) + 0.5f) : ((23) * (1 << 12) - 0.5f)));
        v1.x -= ((fx32)(((7) > 0) ? ((7) * (1 << 12) + 0.5f) : ((7) * (1 << 12) - 0.5f)));
        v1.z += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));

        sub_020182E0(&param1->unk_0C[1], (((0 - 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[2], (((0 + 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[3], ((0 * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182A8(&param1->unk_0C[1], v0.x, v0.y, v0.z - ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))));
        sub_020182A8(&param1->unk_0C[2], v0.x, v0.y, v0.z + ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))));
        sub_020182A8(&param1->unk_0C[3], v0.x, v0.y, v0.z);
        break;
    case 3:
        v0.x += ((fx32)(((23) > 0) ? ((23) * (1 << 12) + 0.5f) : ((23) * (1 << 12) - 0.5f)));
        v1.x += ((fx32)(((7) > 0) ? ((7) * (1 << 12) + 0.5f) : ((7) * (1 << 12) - 0.5f)));
        v1.z += ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f)));

        sub_020182E0(&param1->unk_0C[1], (((180 - 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[2], (((180 + 40) * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182E0(&param1->unk_0C[3], ((180 * 0xffff) / 360), (enum RotationAxis)(1));
        sub_020182A8(&param1->unk_0C[1], v0.x, v0.y, v0.z + ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))));
        sub_020182A8(&param1->unk_0C[2], v0.x, v0.y, v0.z - ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))));
        sub_020182A8(&param1->unk_0C[3], v0.x, v0.y, v0.z);
        break;
    default:
        break;
    }

    sub_020182A8(&param1->unk_0C[0], v1.x, v1.y, v1.z);
}
