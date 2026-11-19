#include "global.h"

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    fx32 unk_08;
    MtxFx33 unk_0C;
    MtxFx33 unk_30;
    MtxFx33 unk_54;
    VecFx32 unk_78;
    VecFx32 unk_84;
    fx32 unk_90;
    fx32 unk_94;
    fx32 unk_98;
    VecFx32 unk_9C;
    fx32 unk_A8;
    fx32 unk_AC;
    fx32 unk_B0;
    fx32 unk_B4;
    fx32 unk_B8;
    fx32 unk_BC;
    fx32 unk_C0;
    fx32 unk_C4;
    fx32 unk_C8;
    fx32 unk_CC;
    fx32 unk_D0;
    fx32 unk_D4;
    VecFx32 unk_D8;
    fx32 unk_E4;
} UnkStruct_ov115_02261C18;

extern const VecFx32 ov91_02261C4C;

void ov91_0225DD50(UnkStruct_ov115_02261C18 *param0, u16 param1);

void ov91_02261208(const VecFx32 *param0, const VecFx32 *param1, const VecFx32 *param2, fx32 param3, VecFx32 *param4, fx32 *param5);
u32 ov91_0225DE8C(const UnkStruct_ov115_02261C18 *param0, const VecFx32 *param1, const VecFx32 *param2, VecFx32 *param3);
BOOL ov91_0225DFB0(const UnkStruct_ov115_02261C18 *param0, const VecFx32 *param1);
fx32 ov91_0225E044(const UnkStruct_ov115_02261C18 *param0, u16 param1);
fx32 ov91_0225E070(const UnkStruct_ov115_02261C18 *param0, u16 param1);
void ov91_0225E09C(UnkStruct_ov115_02261C18 *param0, fx32 param1);
void ov91_0225E1E0(UnkStruct_ov115_02261C18 *param0, u16 param1);

void ov91_0225DD50(UnkStruct_ov115_02261C18 *param0, u16 param1) {
    VecFx32 v0;

    param0->unk_02 = param1;

    MTX_Identity33(&param0->unk_54);
    MTX_Identity33(&param0->unk_30);
    MTX_RotY33(&param0->unk_30, FX_SinIdx(param1), FX_CosIdx(param1));
    MTX_Concat33(&param0->unk_0C, &param0->unk_30, &param0->unk_54);
    MTX_MultVec33(&ov91_02261C4C, &param0->unk_54, &param0->unk_78);

    v0.x = 0;
    v0.y = 0;
    v0.z = -param0->unk_08;

    MTX_MultVec33(&v0, &param0->unk_54, &param0->unk_84);

    v0.x = param0->unk_B8 + param0->unk_9C.x;
    v0.y = param0->unk_BC;
    v0.z = param0->unk_9C.z + ((FX32_CONST(50)) + (FX32_CONST(30)));

    MTX_MultVec33(&v0, &param0->unk_30, &v0);
    VEC_Add(&v0, &param0->unk_84, &param0->unk_84);

    param0->unk_90 = FX_Mul(param0->unk_78.x, param0->unk_84.x) + FX_Mul(param0->unk_78.y, param0->unk_84.y) + FX_Mul(param0->unk_78.z, param0->unk_84.z);

    MTX_MultVec33(&param0->unk_9C, &param0->unk_30, &param0->unk_D8);
}

u32 ov91_0225DE8C(const UnkStruct_ov115_02261C18 *param0, const VecFx32 *param1, const VecFx32 *param2, VecFx32 *param3) {
    fx32 v0;
    VecFx32 v1;
    fx32 v2;
    fx32 v3;
    VecFx32 v4;
    VecFx32 v5;
    u32 v6 = 0;

    if (param0->unk_00 == 0) {
        return 0;
    }

    ov91_02261208(param1, param2, &param0->unk_78, param0->unk_90, &v1, &v0);
    VEC_Subtract(&param0->unk_84, param1, &v4);

    v2 = VEC_Mag(&v4);

    if ((v0 >= 0) && (v0 <= ((fx32)0x0000000000001000L))) {
        if (v2 <= param0->unk_08) {
            if (param0->unk_01 == 1) {
                v6 = 1;
            } else {
                v6 = 2;
            }
        } else {
            return 2;
        }
    }

    if ((v0 > 0) && (v2 < param0->unk_94)) {
        v3 = FX_MulInline(param0->unk_94 - v2, param0->unk_98);
        VEC_Normalize(&v4, &v5);

        param3->x = FX_MulInline(v5.x, v3);
        param3->y = FX_MulInline(v5.y, v3);
        param3->z = FX_MulInline(v5.z, v3);

        if ((v2 - v3) <= param0->unk_08) {
            if (param0->unk_01 == 1) {
                v6 = 1;
            } else {
                v6 = 2;
            }
        } else {
            v6 = 3;
        }
    }

    return v6;
}

BOOL ov91_0225DFB0(const UnkStruct_ov115_02261C18 *param0, const VecFx32 *param1) {
    VecFx32 v0;
    VecFx32 v1, v2;
    fx32 v3;
    fx32 v4;
    s32 v5;

    if (param0->unk_00 == 0) {
        return 0;
    }

    if ((param0->unk_D8.y < param1->y) && ((param0->unk_D8.y + param0->unk_E4) > param1->y)) {
        v1.x = param0->unk_D8.x;
        v1.z = param0->unk_D8.z;
        v1.y = 0;
        v2.x = param1->x;
        v2.z = param1->z;
        v2.y = 0;

        VEC_Subtract(&v1, &v2, &v0);

        v3 = VEC_Mag(&v0);
        v5 = param1->y - param0->unk_D8.y;
        v5 = v5 >> 12;
        v4 = (v5 * param0->unk_D0) / (param0->unk_E4 >> 12);
        v4 = param0->unk_CC - v4;

        if (v3 < v4) {
            return 1;
        }
    }

    return 0;
}

fx32 ov91_0225E044(const UnkStruct_ov115_02261C18 *param0, u16 param1) {
    s32 v0;
    fx32 v1;

    v0 = (param1 * 360) / 0xffff;
    v0 = 90 - v0;
    v1 = (v0 * param0->unk_AC) / 90;
    v1 += param0->unk_B0;

    return v1;
}

fx32 ov91_0225E070(const UnkStruct_ov115_02261C18 *param0, u16 param1) {
    s32 v0;
    fx32 v1;

    v0 = (param1 * 360) / 0xffff;
    v1 = (v0 * param0->unk_C4) / 90;
    v1 += param0->unk_C0;

    return v1;
}

void ov91_0225E09C(UnkStruct_ov115_02261C18 *param0, fx32 param1) {
    param0->unk_A8 = param1;
    param0->unk_AC = FX_MulInline(((fx32)(((30) > 0) ? ((30) * (1 << 12) + 0.5f) : ((30) * (1 << 12) - 0.5f))), param1);
    param0->unk_B0 = FX_MulInline(((fx32)(((50) > 0) ? ((50) * (1 << 12) + 0.5f) : ((50) * (1 << 12) - 0.5f))), param1);
    param0->unk_B4 = FX_MulInline(((fx32)(((50) > 0) ? ((50) * (1 << 12) + 0.5f) : ((50) * (1 << 12) - 0.5f))) + ((fx32)(((30) > 0) ? ((30) * (1 << 12) + 0.5f) : ((30) * (1 << 12) - 0.5f))), param1);
    param0->unk_B8 = FX_MulInline(0, param1);
    param0->unk_BC = FX_MulInline(((fx32)(((55) > 0) ? ((55) * (1 << 12) + 0.5f) : ((55) * (1 << 12) - 0.5f))), param1);
    param0->unk_C0 = FX_MulInline(((fx32)(((150) > 0) ? ((150) * (1 << 12) + 0.5f) : ((150) * (1 << 12) - 0.5f))), param1);
    param0->unk_C4 = FX_MulInline(((fx32)(((60) > 0) ? ((60) * (1 << 12) + 0.5f) : ((60) * (1 << 12) - 0.5f))), param1);
    param0->unk_C8 = FX_MulInline(((fx32)(((80) > 0) ? ((80) * (1 << 12) + 0.5f) : ((80) * (1 << 12) - 0.5f))), param1);
    param0->unk_CC = FX_MulInline(((fx32)(((120) > 0) ? ((120) * (1 << 12) + 0.5f) : ((120) * (1 << 12) - 0.5f))), param1);
    param0->unk_D0 = param0->unk_CC - param0->unk_C8;
    param0->unk_D4 = FX_MulInline(((fx32)(((1.50f) > 0) ? ((1.50f) * (1 << 12) + 0.5f) : ((1.50f) * (1 << 12) - 0.5f))), param1);
}

void ov91_0225E1E0(UnkStruct_ov115_02261C18 *param0, u16 param1) {
    param0->unk_06 = param1;
}
