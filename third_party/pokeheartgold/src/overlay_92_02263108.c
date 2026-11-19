#include "global.h"

typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
    };
    f32 val2[4];
} UnkUnion_ov116_02266FEC;
static f32 ov92_02263218(const UnkUnion_ov116_02266FEC *param0);
typedef union {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
        f32 val1_10;
        f32 val1_14;
        f32 val1_18;
        f32 val1_1C;
        f32 val1_20;
        f32 val1_24;
        f32 val1_28;
        f32 val1_2C;
        f32 val1_30;
        f32 val1_34;
        f32 val1_38;
        f32 val1_3C;
    };
    f32 val2[4][4];
    f32 val3[16];
} UnkUnion_ov116_022671EC;

void ov92_02263108(UnkUnion_ov116_02266FEC *param0, const UnkUnion_ov116_02266FEC *param1, const UnkUnion_ov116_02266FEC *param2);
static f32 ov92_02263218(const UnkUnion_ov116_02266FEC *param0);
f32 ov92_0226325C(const UnkUnion_ov116_02266FEC *param0);
void ov92_022632B4(UnkUnion_ov116_02266FEC *param0, const UnkUnion_ov116_02266FEC *param1, f32 param2);
void ov92_022632E8(UnkUnion_ov116_022671EC *param0, const UnkUnion_ov116_02266FEC *param1);
void ov92_022634F8(const UnkUnion_ov116_022671EC *param0, MtxFx44 *param1);

void ov92_02263108(UnkUnion_ov116_02266FEC *param0, const UnkUnion_ov116_02266FEC *param1, const UnkUnion_ov116_02266FEC *param2) {
    param0->val1_00 = (param1->val1_00 * param2->val1_00) - (param1->val1_04 * param2->val1_04) - (param1->val1_08 * param2->val1_08) - (param1->val1_0C * param2->val1_0C);
    param0->val1_04 = (param1->val1_00 * param2->val1_04) + (param1->val1_04 * param2->val1_00) + (param1->val1_08 * param2->val1_0C) - (param1->val1_0C * param2->val1_08);
    param0->val1_08 = (param1->val1_00 * param2->val1_08) - (param1->val1_04 * param2->val1_0C) + (param1->val1_08 * param2->val1_00) + (param1->val1_0C * param2->val1_04);
    param0->val1_0C = (param1->val1_00 * param2->val1_0C) + (param1->val1_04 * param2->val1_08) - (param1->val1_08 * param2->val1_04) + (param1->val1_0C * param2->val1_00);
}

static f32 ov92_02263218(const UnkUnion_ov116_02266FEC *param0) {
    return (param0->val1_00 * param0->val1_00) + (param0->val1_04 + param0->val1_04) + (param0->val1_08 + param0->val1_08) + (param0->val1_0C + param0->val1_0C);
}

f32 ov92_0226325C(const UnkUnion_ov116_02266FEC *param0) {
    fx32 v0;

    v0 = ((fx32)(((ov92_02263218(param0)) > 0) ? ((ov92_02263218(param0)) * (1 << 12) + 0.5f) : ((ov92_02263218(param0)) * (1 << 12) - 0.5f)));
    return (f32)((FX_Sqrt(v0)) / (f32)(1 << 12));
}

void ov92_022632B4(UnkUnion_ov116_02266FEC *param0, const UnkUnion_ov116_02266FEC *param1, f32 param2) {
    param0->val1_00 = param1->val1_00 / param2;
    param0->val1_04 = param1->val1_04 / param2;
    param0->val1_08 = param1->val1_08 / param2;
    param0->val1_0C = param1->val1_0C / param2;
}

void ov92_022632E8(UnkUnion_ov116_022671EC *param0, const UnkUnion_ov116_02266FEC *param1) {
    f32 v0 = param1->val1_04 * param1->val1_04 * 2.0;
    f32 v1 = param1->val1_08 * param1->val1_08 * 2.0;
    f32 v2 = param1->val1_0C * param1->val1_0C * 2.0;
    f32 v3 = param1->val1_04 * param1->val1_08 * 2.0;
    f32 v4 = param1->val1_08 * param1->val1_0C * 2.0;
    f32 v5 = param1->val1_0C * param1->val1_04 * 2.0;
    f32 v6 = param1->val1_04 * param1->val1_00 * 2.0;
    f32 v7 = param1->val1_08 * param1->val1_00 * 2.0;
    f32 v8 = param1->val1_0C * param1->val1_00 * 2.0;

    param0->val1_00 = 1.0 - v1 - v2;
    param0->val1_04 = v3 + v8;
    param0->val1_08 = v5 - v7;
    param0->val1_0C = 0.0;
    param0->val1_10 = v3 - v8;
    param0->val1_14 = 1.0 - v2 - v0;
    param0->val1_18 = v4 + v6;
    param0->val1_1C = 0.0;
    param0->val1_20 = v5 + v7;
    param0->val1_24 = v4 - v6;
    param0->val1_28 = 1.0 - v0 - v1;
    param0->val1_2C = 0.0;
    param0->val1_30 = 0.0;
    param0->val1_34 = 0.0;
    param0->val1_38 = 0.0;
    param0->val1_3C = 1.0;
}

void ov92_022634F8(const UnkUnion_ov116_022671EC *param0, MtxFx44 *param1) {
    param1->_00 = ((fx32)(((param0->val1_00) > 0) ? ((param0->val1_00) * (1 << 12) + 0.5f) : ((param0->val1_00) * (1 << 12) - 0.5f)));
    param1->_01 = ((fx32)(((param0->val1_04) > 0) ? ((param0->val1_04) * (1 << 12) + 0.5f) : ((param0->val1_04) * (1 << 12) - 0.5f)));
    param1->_02 = ((fx32)(((param0->val1_08) > 0) ? ((param0->val1_08) * (1 << 12) + 0.5f) : ((param0->val1_08) * (1 << 12) - 0.5f)));
    param1->_03 = ((fx32)(((param0->val1_0C) > 0) ? ((param0->val1_0C) * (1 << 12) + 0.5f) : ((param0->val1_0C) * (1 << 12) - 0.5f)));
    param1->_10 = ((fx32)(((param0->val1_10) > 0) ? ((param0->val1_10) * (1 << 12) + 0.5f) : ((param0->val1_10) * (1 << 12) - 0.5f)));
    param1->_11 = ((fx32)(((param0->val1_14) > 0) ? ((param0->val1_14) * (1 << 12) + 0.5f) : ((param0->val1_14) * (1 << 12) - 0.5f)));
    param1->_12 = ((fx32)(((param0->val1_18) > 0) ? ((param0->val1_18) * (1 << 12) + 0.5f) : ((param0->val1_18) * (1 << 12) - 0.5f)));
    param1->_13 = ((fx32)(((param0->val1_1C) > 0) ? ((param0->val1_1C) * (1 << 12) + 0.5f) : ((param0->val1_1C) * (1 << 12) - 0.5f)));
    param1->_20 = ((fx32)(((param0->val1_20) > 0) ? ((param0->val1_20) * (1 << 12) + 0.5f) : ((param0->val1_20) * (1 << 12) - 0.5f)));
    param1->_21 = ((fx32)(((param0->val1_24) > 0) ? ((param0->val1_24) * (1 << 12) + 0.5f) : ((param0->val1_24) * (1 << 12) - 0.5f)));
    param1->_22 = ((fx32)(((param0->val1_28) > 0) ? ((param0->val1_28) * (1 << 12) + 0.5f) : ((param0->val1_28) * (1 << 12) - 0.5f)));
    param1->_23 = ((fx32)(((param0->val1_2C) > 0) ? ((param0->val1_2C) * (1 << 12) + 0.5f) : ((param0->val1_2C) * (1 << 12) - 0.5f)));
    param1->_30 = ((fx32)(((param0->val1_30) > 0) ? ((param0->val1_30) * (1 << 12) + 0.5f) : ((param0->val1_30) * (1 << 12) - 0.5f)));
    param1->_31 = ((fx32)(((param0->val1_34) > 0) ? ((param0->val1_34) * (1 << 12) + 0.5f) : ((param0->val1_34) * (1 << 12) - 0.5f)));
    param1->_32 = ((fx32)(((param0->val1_38) > 0) ? ((param0->val1_38) * (1 << 12) + 0.5f) : ((param0->val1_38) * (1 << 12) - 0.5f)));
    param1->_33 = ((fx32)(((param0->val1_3C) > 0) ? ((param0->val1_3C) * (1 << 12) + 0.5f) : ((param0->val1_3C) * (1 << 12) - 0.5f)));
}
