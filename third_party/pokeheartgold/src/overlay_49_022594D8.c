#include "global.h"
#include "unk_02005D10.h"
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

void ov49_02258E7C(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, int param3);

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

s32 ov42_02228188(const UnkStruct_ov63_0222BEC0 *param0, int param1);

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;

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

u8 ov49_02259294(const u8 *param0, u32 param1);

BOOL ov49_022592A8(const UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, u32 param3);

extern const u8 ov49_02269658[4];

extern const u8 ov49_0226965C[4];

void ov49_022594D8(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1);

void ov45_0223089C(UnkStruct_ov66_02234798 *param0, BOOL param1, s32);
void ov45_022308E4(UnkStruct_ov66_02234798 *param0, const VecFx32 *param1);
void ov45_02230908(const UnkStruct_ov66_02234798 *param0, VecFx32 *param1);
void ov45_0223093C(UnkStruct_ov66_02234798 *param0, int param1);
void ov45_02230968(UnkStruct_ov66_02234798 *param0);
void ov49_02259320(UnkStruct_ov70_0225D220 *param0, fx32 param1, fx32 param2, u32 param3);
BOOL ov49_0225932C(UnkStruct_ov70_0225D220 *param0, u32 param1);
fx32 ov49_022593BC(const UnkStruct_ov70_0225D220 *param0);
void ov49_022595CC(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1, s32 arg2);

void ov49_022594D8(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1) {
    switch (param0->unk_08) {
    case 0:
        param0->unk_0C.val1.unk_00 = ov49_02259294(ov49_02269658, NELEMS(ov49_02269658));
        param0->unk_08++;
        break;
    case 1:
        param0->unk_0C.val1.unk_00--;

        if (param0->unk_0C.val1.unk_00 == 0) {
            param0->unk_08++;
        }
        break;
    case 2: {
        u32 v0;
        u32 v1;

        v0 = ov42_02228188(param0->unk_00, 4);
        v1 = ov42_02228188(param0->unk_00, 6);

        param0->unk_0C.val1.unk_02 = ov49_02259294(ov49_0226965C, NELEMS(ov49_0226965C));

        if (ov49_022592A8(param1, param0, param0->unk_0C.val1.unk_02, 4 + v0) == 1) {
            if (v1 == param0->unk_0C.val1.unk_02) {
                ov49_02258E7C(param1, param0, 2, param0->unk_0C.val1.unk_02);
                param0->unk_08 = 4;
            } else {
                ov49_02258E7C(param1, param0, 1, param0->unk_0C.val1.unk_02);
                param0->unk_08 = 3;
            }
        } else {
            ov49_02258E7C(param1, param0, 1, param0->unk_0C.val1.unk_02);
            param0->unk_08 = 4;
        }
    } break;
    case 3: {
        u32 v2;

        v2 = ov42_02228188(param0->unk_00, 5);

        if (v2 == 0) {
            ov49_02258E7C(param1, param0, 2, param0->unk_0C.val1.unk_02);
            param0->unk_08++;
        }
    } break;
    case 4: {
        u32 v3;

        v3 = ov42_02228188(param0->unk_00, 5);

        if (v3 == 0) {
            param0->unk_08 = 0;
        }
    } break;
    }
}

void ov49_022595CC(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov70_0225CA20 *param1, s32 arg2) {
    switch (param0->unk_08) {
    case 0:
        ov45_0223089C(param0->unk_04, 0, arg2);
        ov45_0223093C(param0->unk_04, 0);
        ov49_02259320(&param0->unk_0C.val2.unk_00, ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), ((fx32)(((500) > 0) ? ((500) * (1 << 12) + 0.5f) : ((500) * (1 << 12) - 0.5f))), 24);

        param0->unk_0C.val2.unk_10 = 32;
        param0->unk_08++;
    case 1:
        param0->unk_0C.val2.unk_10--;

        if (param0->unk_0C.val2.unk_10 == 0) {
            param0->unk_08++;
            param0->unk_0C.val2.unk_10 = 0;
            PlaySE(1614);
        }
        break;
    case 2: {
        BOOL v0;
        VecFx32 v1;

        v0 = ov49_0225932C(&param0->unk_0C.val2.unk_00, param0->unk_0C.val2.unk_10);

        param0->unk_0C.val2.unk_10++;

        ov45_02230908(param0->unk_04, &v1);
        v1.y = ov49_022593BC(&param0->unk_0C.val2.unk_00);
        ov45_022308E4(param0->unk_04, &v1);

        if (v0 == 1) {
            param0->unk_08++;
            ov45_02230968(param0->unk_04);
            param0->unk_0A = 1;
        }
    } break;
    case 3:
        break;
    }
}
