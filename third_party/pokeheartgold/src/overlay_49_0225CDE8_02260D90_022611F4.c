#include "global.h"

#include "overlay_42.h"
#include "overlay_49_unk_struct_ov70_02263270.h"

struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_02261E10_t;

void ov45_0222AE08(u32 param0, u32 *param1, u32 *param2);

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

void ov49_02259154(const UnkStruct_ov70_0225CC54 *param0, VecFx32 *param1);

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

void ov49_0225E420(const UnkStruct_ov70_02261E10 *param0, u32 param1, u32 param2, VecFx32 *param3);

typedef struct {
    u32 unk_00;
    u16 unk_04;
    u16 unk_06;
    s16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54 *unk_0C;
    UnkStruct_ov70_02263270 unk_10;
} UnkStruct_ov70_02265104;

BOOL ov49_022611F4(const UnkStruct_ov70_02265104 *param0, const UnkStruct_ov70_02261E10 *param1);

BOOL ov49_022611F4(const UnkStruct_ov70_02265104 *param0, const UnkStruct_ov70_02261E10 *param1) {
    VecFx32 v0;
    VecFx32 v1;
    u32 v2, v3;

    ov45_0222AE08(param0->unk_00, &v2, &v3);
    ov49_0225E420(param1, v2, v3, &v1);
    ov49_02259154(param0->unk_0C, &v0);

    if ((v0.x + ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f)))) >= v1.x) {
        return 1;
    }

    return 0;
}

struct UnkStruct_ov70_02260AD4_t;

void ov49_02258800(const UnkStruct_ov44_02232914 *param0, VecFx32 *param1);

UnkStruct_ov44_02232914 ov49_02258E34(const UnkStruct_ov70_0225CC54 *param0);

void ov49_02259148(UnkStruct_ov70_0225CC54 *param0, const VecFx32 *param1);

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

void ov49_0225CC20(UnkStruct_ov70_02260AD4 *param0, fx32 param1, fx32 param2, fx32 param3);

BOOL ov49_02261234(UnkStruct_ov70_02265104 *param0, UnkStruct_ov70_02261E10 *param1, UnkStruct_ov70_02260AD4 *param2);

BOOL ov49_02261234(UnkStruct_ov70_02265104 *param0, UnkStruct_ov70_02261E10 *param1, UnkStruct_ov70_02260AD4 *param2) {
    VecFx32 v0;
    VecFx32 v1;
    u32 v2, v3;
    UnkStruct_ov44_02232914 v4;
    VecFx32 v5;
    u16 v6;
    BOOL v7;

    param0->unk_08++;

    if (param0->unk_08 < 24) {
        v7 = 0;
    } else {
        param0->unk_08 = 24;
        v7 = 1;
    }

    ov45_0222AE08(param0->unk_00, &v2, &v3);
    ov49_0225E420(param1, v2, v3, &v1);

    v4 = ov49_02258E34(param0->unk_0C);
    ov49_02258800(&v4, &v0);

    v5.x = 0;
    v5.y = FX_Div(FX_MulInline(((fx32)(((param0->unk_08) > 0) ? ((param0->unk_08) * (1 << 12) + 0.5f) : ((param0->unk_08) * (1 << 12) - 0.5f))), (v1.y - v0.y)), ((fx32)(((24) > 0) ? ((24) * (1 << 12) + 0.5f) : ((24) * (1 << 12) - 0.5f))));

    if (param0->unk_08 >= 4) {
        v5.z = FX_Div(FX_MulInline(((fx32)(((param0->unk_08 - 4) > 0) ? ((param0->unk_08 - 4) * (1 << 12) + 0.5f) : ((param0->unk_08 - 4) * (1 << 12) - 0.5f))), (v1.z - v0.z)), ((fx32)(((20) > 0) ? ((20) * (1 << 12) + 0.5f) : ((20) * (1 << 12) - 0.5f))));
    } else {
        v5.z = 0;
    }

    VEC_Add(&v5, &v0, &v5);
    ov49_0225CC20(param2, v5.x, v5.y, v5.z);

    v6 = (param0->unk_08 * 0x7fff) / 24;
    v5.y += FX_MulInline(FX_SinIdx(v6), ((fx32)(((16) > 0) ? ((16) * (1 << 12) + 0.5f) : ((16) * (1 << 12) - 0.5f))));

    ov49_02259148(param0->unk_0C, &v5);

    return v7;
}

BOOL ov49_022613AC(UnkStruct_ov70_02265104 *param0, UnkStruct_ov70_02261E10 *param1);

BOOL ov49_022613AC(UnkStruct_ov70_02265104 *param0, UnkStruct_ov70_02261E10 *param1) {
    s32 v0;
    BOOL v1;
    u16 v2;
    fx32 v3;
    VecFx32 v4;

    param0->unk_08++;

    if (param0->unk_08 < 16) {
        v1 = 0;
    } else {
        param0->unk_08 = 16;
        v1 = 1;
    }

    v0 = param0->unk_08 % 8;
    v2 = (v0 * 0x7fff) / 8;
    v3 = FX_MulInline(FX_SinIdx(v2), ((fx32)(((3) > 0) ? ((3) * (1 << 12) + 0.5f) : ((3) * (1 << 12) - 0.5f))));

    ov49_02259154(param0->unk_0C, &v4);
    v4.y += v3;
    ov49_02259148(param0->unk_0C, &v4);

    return v1;
}

struct UnkStruct_ov70_0225DEE8_t;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

void ov49_02261434(UnkStruct_ov70_02265104 *param0, UnkStruct_ov70_02261E10 *param1, UnkStruct_ov70_0225DEE8 *param2);

void ov49_02261434(UnkStruct_ov70_02265104 *param0, UnkStruct_ov70_02261E10 *param1, UnkStruct_ov70_0225DEE8 *param2) {
    VecFx32 v0;
    u32 v1;
    u32 v2;

    ov45_0222AE08(param0->unk_00, &v2, &v1);
    ov49_0225E420(param1, v2, v1, &v0);
    ov49_02259148(param0->unk_0C, &v0);
}

BOOL ov49_0225A520(const UnkStruct_ov70_0225DEE8 *param0, u32 param1);

void ov49_0225F374(UnkStruct_ov70_02263270 *param0);

fx32 ov49_0225F394(UnkStruct_ov70_02263270 *param0);

void ov49_0225F430(UnkStruct_ov70_02263270 *param0);

BOOL ov49_0225F438(const UnkStruct_ov70_02263270 *param0);

void ov49_02261460(UnkStruct_ov70_02265104 *param0, UnkStruct_ov70_02261E10 *param1, UnkStruct_ov70_0225DEE8 *param2);

void ov49_02261460(UnkStruct_ov70_02265104 *param0, UnkStruct_ov70_02261E10 *param1, UnkStruct_ov70_0225DEE8 *param2) {
    VecFx32 v0;
    fx32 v1;
    u32 v2, v3;

    ov45_0222AE08(param0->unk_00, &v2, &v3);

    if (ov49_0225A520(param2, v2) == 1) {
        if (ov49_0225F438(&param0->unk_10) == 0) {
            ov49_0225F374(&param0->unk_10);
        }
    } else {
        if (ov49_0225F438(&param0->unk_10) == 1) {
            ov49_0225F430(&param0->unk_10);
        }
    }

    v1 = ov49_0225F394(&param0->unk_10);

    ov49_02259154(param0->unk_0C, &v0);
    v0.y += v1;
    ov49_02259148(param0->unk_0C, &v0);
}
