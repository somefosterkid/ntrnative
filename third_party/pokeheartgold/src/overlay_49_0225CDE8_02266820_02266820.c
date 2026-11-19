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

void sub_020182A0(Field3dObject *obj, BOOL visible);

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

BOOL ov49_02265968(const UnkStruct_ov70_02269800 *param0, u32 param1);

BOOL ov49_02265B28(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3);

BOOL ov49_02265B94(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3, fx32 param4);

void ov49_02265BE8(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3, fx32 param4);

BOOL ov49_02266820(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_02266820(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    BOOL v0;
    BOOL v1;
    int v2;

    ((param1->unk_87C[0]->unk_02[0] != 17) ? (void)0 : GF_AssertFail());

    if (param1->unk_954.val2.unk_00 > 0) {
        param1->unk_954.val2.unk_00--;
        sub_020182A0(&param1->unk_0C[0], 0);
    } else {
        if (param1->unk_02 < 3) {
            if (ov49_02265968(param1, param1->unk_02 + 1) == 1) {
                sub_020182A0(&param1->unk_0C[0], 1);
                v0 = ov49_02265B28(param0, param1, 0, 0);

                if (v0 == 1) {
                    param1->unk_02++;
                    param1->unk_954.val2.unk_00 = 3;

                    if (param1->unk_02 < 3) {
                        ov49_02265BE8(param0, param1, 0, 0, 0);
                    } else {
                        sub_020182A0(&param1->unk_0C[0], 0);
                    }
                }
            }
        }
    }

    v1 = 1;

    for (v2 = 1; v2 <= 3; v2++) {
        if (ov49_02265968(param1, v2) == 1) {
            if (param1->unk_02 > (v2 - 1)) {
                sub_020182A0(&param1->unk_0C[v2], 1);
                v0 = ov49_02265B94(param0, param1, v2, 0, ((fx32)(((2.50f) > 0) ? ((2.50f) * (1 << 12) + 0.5f) : ((2.50f) * (1 << 12) - 0.5f))));

                if (v0 == 0) {
                    v1 = 0;
                } else {
                    s32 v3;
                    s32 v4;

                    v4 = v2 - 1;

                    if (param1->unk_954.val2.unk_04[v4] == 0) {
                        sub_020182A0(&param1->unk_0C[v2], 0);
                        NNS_G3dMdlSetMdlAlphaAll(param0->unk_10550[param1->unk_87C[v2]->unk_00].model, 31);
                    } else {
                        param1->unk_954.val2.unk_04[v4]--;
                        v3 = (param1->unk_954.val2.unk_04[v4] * 20) / 8;
                        NNS_G3dMdlSetMdlAlphaAll(param0->unk_10550[param1->unk_87C[v2]->unk_00].model, v3);
                        v1 = 0;
                    }
                }
            } else {
                v1 = 0;
            }
        }
    }

    return v1;
}

BOOL ov49_02266D60(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

void ov49_02266EF8(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_022670B8(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_02266978(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_02266978(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    BOOL v0;

    switch (param1->unk_01) {
    case 0:
        v0 = ov49_02266D60(param0, param1);

        if (v0) {
            param1->unk_01 = 1;
            ov49_02266EF8(param0, param1);
        }
        break;
    case 1:
        v0 = ov49_022670B8(param0, param1);

        if (v0) {
            return 1;
        }
        break;
    }

    return 0;
}

BOOL sub_020182A4(const Field3dObject *obj);

BOOL ov49_02267A1C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2);

BOOL ov49_022669B0(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_022669B0(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    BOOL v0;
    u32 v1;
    int v2;

    if (param1->unk_954.val3.unk_01 < param1->unk_954.val3.unk_02) {
        param1->unk_954.val3.unk_00++;

        if (param1->unk_954.val3.unk_00 >= 8) {
            param1->unk_954.val3.unk_00 = 0;
            param1->unk_954.val3.unk_01++;
        }

        v1 = (param1->unk_954.val3.unk_00 * 4) / 8;
        v1 += param1->unk_954.val3.unk_01 * 4;

        for (v2 = param1->unk_954.val3.unk_03; v2 < v1; v2++) {
            sub_020182A0(&param1->unk_0C[v2], 1);
        }

        param1->unk_954.val3.unk_03 = v1;
    }

    for (v2 = 0; v2 < param1->unk_954.val3.unk_03; v2++) {
        if (sub_020182A4(&param1->unk_0C[v2]) == 1) {
            v0 = ov49_02267A1C(param0, param1, v2);

            if (v0 == 1) {
                sub_020182A0(&param1->unk_0C[v2], 0);
            }
        }
    }

    if ((param1->unk_954.val3.unk_01 == param1->unk_954.val3.unk_02) && (v0 == 1)) {
        return 1;
    }

    return 0;
}

int ov49_02267E18(UnkStruct_ov70_0226BC40 *param0, u32 param1);

BOOL ov49_02266A88(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_02266A88(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    u32 v0 = ov49_02267E18(&param1->unk_954, param1->unk_02);
    param1->unk_02++;

    if (v0 == 2) {
        return 1;
    }

    return 0;
}
