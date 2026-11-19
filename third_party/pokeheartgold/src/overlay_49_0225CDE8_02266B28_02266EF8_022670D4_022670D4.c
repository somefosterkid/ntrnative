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

void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void sub_020182B0(const Field3dObject *obj, fx32 *outX, fx32 *outY, fx32 *outZ);

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);

u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);

void ov49_02259130(UnkStruct_ov70_0225CC54 *param0, BOOL param1);

void ov49_02259148(UnkStruct_ov70_0225CC54 *param0, const VecFx32 *param1);

void ov49_02259154(const UnkStruct_ov70_0225CC54 *param0, VecFx32 *param1);

void ov49_02259160(UnkStruct_ov70_0225CC54 *param0, int param1);

void ov49_0225919C(UnkStruct_ov70_0225CC54 *param0, BOOL param1);

void ov49_0225CC40(UnkStruct_ov70_02260AD4 *param0, const UnkStruct_ov70_0225CC54 *param1);

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

BOOL ov49_02265434(UnkStruct_ov70_022692B4 *param0, s32 param1);

void ov49_022655E0(const UnkStruct_ov70_022692B4 *param0, fx32 *param1, fx32 *param2, fx32 *param3);

void ov49_02265628(UnkStruct_ov70_0226949C *param0);

void ov49_02265660(const UnkStruct_ov70_0226949C *param0, fx32 *param1);

BOOL ov49_0226786C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2, u32 param3);

void ov49_0226789C(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 param2);

void ov49_02265668(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1, u32 seqID);

BOOL ov49_022670D4(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_022670D4(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    int v0;

    switch (param1->unk_954.val1.unk_00) {
    case 0:
        param1->unk_954.val1.unk_02++;

        if (param1->unk_954.val1.unk_02 >= 8) {
            param1->unk_954.val1.unk_00 = 1;
        }
        break;
    case 1:
        ov49_0225919C(param1->unk_08, 1);
        param1->unk_954.val1.unk_02 = 16;
        param1->unk_954.val1.unk_00 = 2;
        break;
    case 2:
        param1->unk_954.val1.unk_02--;

        if (param1->unk_954.val1.unk_02 <= 0) {
            ov49_0225919C(param1->unk_08, 0);
            param1->unk_954.val1.unk_00 = 3;
            param1->unk_954.val1.unk_02 = 0;

            {
                u32 v1;

                v1 = ov49_02258E60(param1->unk_08, 6);
                ov49_02259160(param1->unk_08, v1);
            }
        }
        break;
    case 3: {
        BOOL v2;
        VecFx32 v3;

        v2 = ov49_02265434(&param1->unk_954.val1.unk_B0, param1->unk_954.val1.unk_02);
        param1->unk_954.val1.unk_02++;

        ov49_02259154(param1->unk_08, &v3);
        ov49_022655E0(&param1->unk_954.val1.unk_B0, &v3.x, &v3.y, &v3.z);
        ov49_02259148(param1->unk_08, &v3);

        if (v2 == 1) {
            param1->unk_954.val1.unk_00 = 4;
            param1->unk_954.val1.unk_02 = 0;
            param1->unk_954.val1.unk_04 = v3.y;

            ov49_02265668(param0, param1, 1474);
        }
    } break;
    case 4: {
        u16 v4;
        fx32 v5;
        VecFx32 v6;

        v4 = (0x7fff * param1->unk_954.val1.unk_02) / 10;
        v5 = FX_MulInline(FX_SinIdx(v4), ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))));

        ov49_02259154(param1->unk_08, &v6);
        v6.y = param1->unk_954.val1.unk_04 + v5;
        ov49_02259148(param1->unk_08, &v6);

        if ((param1->unk_954.val1.unk_02 + 1) > 10) {
            ov49_02259130(param1->unk_08, 1);

            {
                if (param1->unk_08 == ov49_02258DAC(param0->unk_04)) {
                    ov49_0225CC40(param0->unk_08, param1->unk_08);
                }
            }

            return 1;
        } else {
            param1->unk_954.val1.unk_02++;
        }
    } break;
    }

    param1->unk_954.val1.unk_10++;

    for (v0 = 0; v0 < param1->unk_954.val1.unk_01; v0++) {
        {
            BOOL v7;
            VecFx32 v8;
            fx32 v9;
            BOOL v10;

            v7 = ov49_02265434(&param1->unk_954.val1.unk_38[v0], param1->unk_954.val1.unk_10);

            if (v7 == 0) {
                ov49_02265628(&param1->unk_954.val1.unk_14[v0]);
                sub_020182B0(&param1->unk_0C[v0], &v8.x, &v8.y, &v8.z);
                ov49_022655E0(&param1->unk_954.val1.unk_38[v0], &v8.x, &v8.y, &v8.z);
                ov49_02265660(&param1->unk_954.val1.unk_14[v0], &v9);

                v8.x += v9;

                sub_020182A8(&param1->unk_0C[v0], v8.x, v8.y, v8.z);
            } else {
                v10 = ov49_0226786C(param0, param1, v0, 4);

                if (v10 == 1) {
                    ov49_02265668(param0, param1, 1448);
                }
            }
        }

        ov49_0226789C(param0, param1, v0);
    }

    return 0;
}

BOOL ov49_02267328(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1);

BOOL ov49_02267328(UnkStruct_ov70_02269204 *param0, UnkStruct_ov70_02269800 *param1) {
    int v0;

    for (v0 = 0; v0 < param1->unk_954.val1.unk_01; v0++) {
        ov49_0226789C(param0, param1, v0);
    }

    switch (param1->unk_954.val1.unk_00) {
    case 0:
        ov49_02265668(param0, param1, 1448);

        for (v0 = 0; v0 < param1->unk_954.val1.unk_01; v0++) {
            ov49_0226786C(param0, param1, v0, 4);
        }

        param1->unk_954.val1.unk_00 = 1;
        break;
    case 1:
        ov49_0225919C(param1->unk_08, 1);
        param1->unk_954.val1.unk_02 = 8;
        param1->unk_954.val1.unk_00 = 2;
        break;
    case 2:
        param1->unk_954.val1.unk_02--;

        if (param1->unk_954.val1.unk_02 <= 0) {
            ov49_0225919C(param1->unk_08, 0);
            param1->unk_954.val1.unk_00 = 3;
            param1->unk_954.val1.unk_02 = 0;

            {
                u32 v1;

                v1 = ov49_02258E60(param1->unk_08, 6);
                ov49_02259160(param1->unk_08, v1);
            }
        }
        break;
    case 3: {
        BOOL v2;
        VecFx32 v3;

        v2 = ov49_02265434(&param1->unk_954.val1.unk_B0, param1->unk_954.val1.unk_02);
        param1->unk_954.val1.unk_02++;

        ov49_02259154(param1->unk_08, &v3);
        ov49_022655E0(&param1->unk_954.val1.unk_B0, &v3.x, &v3.y, &v3.z);
        ov49_02259148(param1->unk_08, &v3);

        if (v2 == 1) {
            param1->unk_954.val1.unk_02 = 0;
            param1->unk_954.val1.unk_04 = v3.y;

            ov49_02265668(param0, param1, 1474);
            ov49_02259130(param1->unk_08, 1);

            {
                if (param1->unk_08 == ov49_02258DAC(param0->unk_04)) {
                    ov49_0225CC40(param0->unk_08, param1->unk_08);
                }
            }

            return 1;
        }
    } break;
    }

    return 0;
}
