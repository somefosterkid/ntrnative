#include "global.h"

#include "error_handling.h"
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

typedef void (*UnkFuncPtr_ov70_0226D558)(struct UnkStruct_ov70_0225CC54_t *, UnkStruct_ov70_0225CA20 *);

extern const UnkFuncPtr_ov70_0226D558 ov49_022696C0[10];

extern const UnkFuncPtr_ov70_0226D558 ov49_02269698[10];

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);

UnkStruct_ov70_0225CC54 *ov49_02258D70(UnkStruct_ov70_0225CA20 *param0, u32 param1);
s32 ov42_02228188(const UnkStruct_ov63_0222BEC0 *param0, int param1);
const UnkStruct_ov63_0222BEC0 *ov42_022283BC(const UnkStruct_ov63_0222BEC0 *param0, const UnkStruct_ov63_0222BE18 *param1, int param2);
void ov45_022308B8(UnkStruct_ov66_02234548 *param0);
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;
const UnkStruct_ov63_0222BEC0 *ov42_022284A4(const UnkStruct_ov63_0222BE18 *param0, UnkStruct_ov63_0222CC3C param1);
UnkStruct_ov63_0222CC3C ov42_022282DC(const UnkStruct_ov63_0222BEC0 *param0);
UnkStruct_ov63_0222CC3C ov42_022282E8(const UnkStruct_ov63_0222BEC0 *param0);
BOOL ov49_02258F38(const UnkStruct_ov70_0225CC54 *param0);
int ov49_02258F3C(const UnkStruct_ov70_0225CC54 *param0);
UnkStruct_ov70_0225CC54 *ov49_02258F40(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1);
void ov49_02258F70(UnkStruct_ov70_0225CA20 *param0);
const UnkStruct_ov70_0225CC54 *ov49_02258F7C(const UnkStruct_ov70_0225CA20 *param0, u16 param1, u16 param2);
BOOL ov49_02258FDC(const UnkStruct_ov70_0225CC54 *param0, u16 param1, u16 param2);

void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2) {
    GF_ASSERT(param2 < 10);

    if (ov49_02269698[param1->unk_0B] != NULL) {
        ov49_02269698[param1->unk_0B](param1, param0);
    }

    param1->unk_08 = 0;
    param1->unk_0A = 0;
    param1->unk_0B = param2;

    memset(&param1->unk_0C, 0, sizeof(UnkStruct_ov70_0225CC54_sub1));

    param1->unk_24 = ov49_022696C0[param2];
}

BOOL ov49_02258F38(const UnkStruct_ov70_0225CC54 *param0) {
    return param0->unk_0A;
}

int ov49_02258F3C(const UnkStruct_ov70_0225CC54 *param0) {
    return param0->unk_0B;
}

UnkStruct_ov70_0225CC54 *ov49_02258F40(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1) {
    const UnkStruct_ov63_0222BEC0 *v0;
    u32 v1;
    u32 v2;

    v1 = ov42_02228188(param1->unk_00, 6);
    v0 = ov42_022283BC(param1->unk_00, param0->unk_00, v1);

    if (v0 == 0L) {
        return 0L;
    }

    v2 = ov42_02228188(v0, 4);
    return ov49_02258D70(param0, v2);
}

void ov49_02258F70(UnkStruct_ov70_0225CA20 *param0) {
    ov45_022308B8(param0->unk_04);
}

const UnkStruct_ov70_0225CC54 *ov49_02258F7C(const UnkStruct_ov70_0225CA20 *param0, u16 param1, u16 param2) {
    const UnkStruct_ov63_0222BEC0 *v0;
    int v1;
    UnkStruct_ov63_0222CC3C v2;

    v2.unk_00 = (param1) * 16;
    v2.unk_02 = (param2) * 16;

    v0 = ov42_022284A4(param0->unk_00, v2);

    if (v0 == 0L) {
        return 0L;
    }

    for (v1 = 0; v1 < param0->unk_0C; v1++) {
        if (param0->unk_08[v1].unk_00 == v0) {
            return &param0->unk_08[v1];
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
    return 0L;
}

BOOL ov49_02258FDC(const UnkStruct_ov70_0225CC54 *param0, u16 param1, u16 param2) {
    UnkStruct_ov63_0222CC3C v0;
    UnkStruct_ov63_0222CC3C v1;

    v0 = ov42_022282DC(param0->unk_00);
    v1 = ov42_022282E8(param0->unk_00);

    if ((v0.unk_00 == (param1) * 16) && (v0.unk_02 == (param2) * 16)) {
        return 1;
    }

    if ((v1.unk_00 == (param1) * 16) && (v1.unk_02 == (param2) * 16)) {
        return 1;
    }

    return 0;
}
