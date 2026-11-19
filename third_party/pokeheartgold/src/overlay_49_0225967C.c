#include "global.h"

#include "unk_02005D10.h"
#include "math_util.h"
#include "overlay_90_unk_struct_ov70_0225_ca20_t.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct UnkStruct_ov63_0222BE18_t;

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov63_0222D848_t;

struct UnkStruct_ov66_02234548_t;

struct UnkStruct_ov66_02234798_t;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225CC54_t;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;

void ov45_0223089C(UnkStruct_ov66_02234798 *param0, BOOL param1);

void ov45_022308E4(UnkStruct_ov66_02234798 *param0, const VecFx32 *param1);

void ov45_02230908(const UnkStruct_ov66_02234798 *param0, VecFx32 *param1);

void ov45_0223093C(UnkStruct_ov66_02234798 *param0, int param1);

void ov45_02230968(UnkStruct_ov66_02234798 *param0);

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    u32 unk_0C;
} UnkStruct_ov70_0225D220;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    s8 unk_02;
    s8 unk_03;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov70_0225D93C;

typedef union {
    struct {
        s16 unk_00;
        s16 unk_02;
    } val1;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
    } val2;
    struct {
        UnkStruct_ov70_0225D220 unk_00;
        u32 unk_10;
        fx32 unk_14;
    } val3;
    struct {
        s32 unk_00;
        VecFx32 unk_04;
    } val4;
} UnkStruct_ov70_0225CC54_sub1;

struct UnkStruct_ov70_0225CC54_t {
    UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov66_02234798 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    UnkStruct_ov70_0225CC54_sub1 unk_0C;
    void (*unk_24)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);
};

void ov49_02259320(UnkStruct_ov70_0225D220 *param0, fx32 param1, fx32 param2, u32 param3);

BOOL ov49_0225932C(UnkStruct_ov70_0225D220 *param0, u32 param1);

fx32 ov49_022593BC(const UnkStruct_ov70_0225D220 *param0);

void ov49_0225967C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);

void ov49_02259764(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1, int param2);
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;
void ov49_02258E04(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov63_0222CC3C param1, int param2);
UnkStruct_ov63_0222CC3C ov42_02228270(UnkStruct_ov63_0222CC3C param0, int param1);
u8 ov42_022282A4(int param0);
UnkStruct_ov63_0222CC3C ov42_022282DC(const UnkStruct_ov63_0222BEC0 *param0);
void ov45_02230974(UnkStruct_ov66_02234798 *param0, s8 param1);
void ov45_02230920(UnkStruct_ov66_02234798 *param0, int param1);
void ov45_02230700(UnkStruct_ov66_02234548 *param0, u32 param1);
void ov49_02259734(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);
void ov49_02259740(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);
void ov49_0225974C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);
void ov49_02259758(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);
void ov49_02259764(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1, int param2);
void ov49_0225991C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);
void ov49_022599F8(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);
void ov49_02259A20(UnkStruct_ov70_0225D93C *param0, UnkStruct_ov66_02234548 *param1, u32 param2);
void ov49_02259A3C(UnkStruct_ov70_0225D93C *param0, u32 param1);
BOOL ov49_02259A54(UnkStruct_ov70_0225D93C *param0, UnkStruct_ov66_02234548 *param1);

void ov49_0225967C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    switch (param0->unk_08) {
    case 0:
        ov45_0223089C(param0->unk_04, 0);
        ov45_0223093C(param0->unk_04, 0);
        ov49_02259320(&param0->unk_0C.val2.unk_00, ((fx32)(((500) > 0) ? ((500) * (1 << 12) + 0.5f) : ((500) * (1 << 12) - 0.5f))), ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), 24);

        param0->unk_0C.val2.unk_10 = 0;
        param0->unk_08++;

        PlaySE(1614);
    case 1: {
        BOOL v0;
        VecFx32 v1;

        v0 = ov49_0225932C(&param0->unk_0C.val2.unk_00, param0->unk_0C.val2.unk_10);

        param0->unk_0C.val2.unk_10++;

        ov45_02230908(param0->unk_04, &v1);
        v1.y = ov49_022593BC(&param0->unk_0C.val2.unk_00);
        ov45_022308E4(param0->unk_04, &v1);

        if (v0 == 1) {
            param0->unk_0C.val2.unk_10 = 32;
            param0->unk_08++;
        }
    } break;
    case 2:
        param0->unk_0C.val2.unk_10--;

        if (param0->unk_0C.val2.unk_10 == 0) {
            param0->unk_08++;

            ov45_02230968(param0->unk_04);
            ov45_0223089C(param0->unk_04, 1);
            param0->unk_0A = 1;
        }
        break;
    case 3:
        break;
    }
}

void ov49_02259734(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    ov49_02259764(param0, param1, 2);
}

void ov49_02259740(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    ov49_02259764(param0, param1, 3);
}

void ov49_0225974C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    ov49_02259764(param0, param1, 0);
}

void ov49_02259758(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    ov49_02259764(param0, param1, 1);
}

void ov49_02259764(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1, int param2) {
    switch (param0->unk_08) {
    case 0:
        ov45_0223089C(param0->unk_04, 0);
        ov45_0223093C(param0->unk_04, 0);
        ov45_02230974(param0->unk_04, 2);
        ov49_02259320(&param0->unk_0C.val3.unk_00, 0, (16 * ((fx32)0x0000000000001000L)), 4);

        {
            VecFx32 v0;

            ov45_02230908(param0->unk_04, &v0);

            switch (param2) {
            case 0:
            case 1:
                param0->unk_0C.val3.unk_14 = v0.z;
                break;
            case 2:
            case 3:
                param0->unk_0C.val3.unk_14 = v0.x;
                break;
            }
        }
        param0->unk_0C.val3.unk_10 = 0;
        param0->unk_08++;
    case 1: {
        BOOL v1;
        VecFx32 v2;

        v1 = ov49_0225932C(&param0->unk_0C.val3.unk_00, param0->unk_0C.val3.unk_10);
        param0->unk_0C.val3.unk_10++;
        ov45_02230908(param0->unk_04, &v2);

        switch (param2) {
        case 0:
            v2.z = param0->unk_0C.val3.unk_14 - ov49_022593BC(&param0->unk_0C.val3.unk_00);
            break;
        case 1:
            v2.z = param0->unk_0C.val3.unk_14 + ov49_022593BC(&param0->unk_0C.val3.unk_00);
            break;
        case 2:
            v2.x = param0->unk_0C.val3.unk_14 - ov49_022593BC(&param0->unk_0C.val3.unk_00);
            break;
        case 3:
            v2.x = param0->unk_0C.val3.unk_14 + ov49_022593BC(&param0->unk_0C.val3.unk_00);
            break;
        }

        ov45_022308E4(param0->unk_04, &v2);

        if (v1 == 1) {
            u32 v3;

            v3 = ov42_022282A4(param2);
            param0->unk_0C.val3.unk_10 = 4;

            switch (v3) {
            case 3:
                param0->unk_0C.val3.unk_10 += 2;
            case 0:
                param0->unk_0C.val3.unk_10 += 2;
            case 2:
                param0->unk_0C.val3.unk_10 += 2;
            case 1:
                break;
            }

            param0->unk_08++;
        }
    } break;
    case 2:
        param0->unk_0C.val3.unk_10--;

        if (param0->unk_0C.val3.unk_10 == 0) {
            param0->unk_08++;

            {
                UnkStruct_ov63_0222CC3C v4;
                u32 v5;

                v4 = ov42_022282DC(param0->unk_00);
                v4 = ov42_02228270(v4, param2);
                v5 = ov42_022282A4(param2);

                ov49_02258E04(param0, v4, v5);
            }

            ov45_02230968(param0->unk_04);
            ov45_0223089C(param0->unk_04, 1);

            param0->unk_0A = 1;
        }
        break;
    case 3:
        break;
    }
}

void ov49_0225991C(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    switch (param0->unk_08) {
    case 0:
        ov45_0223089C(param0->unk_04, 0);
        ov45_02230908(param0->unk_04, &param0->unk_0C.val4.unk_04);
        param0->unk_0C.val4.unk_00 = ((6 * 2) + 16);
        param0->unk_08++;
        break;
    case 1:
        param0->unk_0C.val4.unk_00--;

        if (param0->unk_0C.val4.unk_00 < 0) {
            param0->unk_0C.val4.unk_00 = ((6 * 2) + 16);
        }

        {
            u32 v0;
            VecFx32 v1;
            s16 v2;

            v1 = param0->unk_0C.val4.unk_04;
            v2 = param0->unk_0C.val4.unk_00 - 16;

            if (v2 > 0) {
                v2 = v2 % 6;
                v0 = (180 * v2) / 6;

                v1.z += FX_MulInline(GF_SinDegNoWrap(v0), -2 * ((fx32)0x0000000000001000L));
                v1.y += FX_MulInline(GF_SinDegNoWrap(v0), 10 * ((fx32)0x0000000000001000L));
            }

            ov45_022308E4(param0->unk_04, &v1);
            ov45_02230920(param0->unk_04, 1);
        }
        break;
    }
}

void ov49_022599F8(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    VecFx32 v0;

    ov45_0223089C(param0->unk_04, 1);
    ov45_02230908(param0->unk_04, &v0);

    v0.y = ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f)));

    ov45_022308E4(param0->unk_04, &v0);
}

void ov49_02259A20(UnkStruct_ov70_0225D93C *param0, UnkStruct_ov66_02234548 *param1, u32 param2) {
    param0->unk_01 = param2;
    param0->unk_02 = param2;
    param0->unk_03 = 0;
    param0->unk_04 = 0;
    param0->unk_06 = 0;
    param0->unk_00 = 0;

    ov45_02230700(param1, param2);
}

void ov49_02259A3C(UnkStruct_ov70_0225D93C *param0, u32 param1) {
    param0->unk_00 = 1;
    param0->unk_02 = param0->unk_01;
    param0->unk_03 = param1 - param0->unk_01;
    param0->unk_04 = 0;
    param0->unk_06 = 60;
}

BOOL ov49_02259A54(UnkStruct_ov70_0225D93C *param0, UnkStruct_ov66_02234548 *param1) {
    if (param0->unk_00 == 0) {
        return 1;
    }

    param0->unk_04++;

    if (param0->unk_04 >= param0->unk_06) {
        param0->unk_00 = 0;
    }

    param0->unk_01 = (param0->unk_04 * param0->unk_03) / param0->unk_06;
    param0->unk_01 += param0->unk_02;

    ov45_02230700(param1, param0->unk_01);

    return 0;
}
