#include "global.h"

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

typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;

typedef struct UnkStruct_ov66_02234798_t UnkStruct_ov66_02234798;

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

struct UnkStruct_ov70_0225CA20_t {
    UnkStruct_ov63_0222BE18 *unk_00;
    UnkStruct_ov66_02234548 *unk_04;
    UnkStruct_ov70_0225CC54 *unk_08;
    u16 unk_0C;
    u16 unk_0E;
    UnkStruct_ov63_0222D848 *unk_10;
    const UnkStruct_ov70_0225C894 *unk_14;
    UnkStruct_ov70_0225D93C unk_18;
    UnkStruct_ov70_0225CC54 *unk_20;
    UnkStruct_ov70_0225CC54 *unk_24;
};

void ov49_02259A20(UnkStruct_ov70_0225D93C *param0, UnkStruct_ov66_02234548 *param1, u32 param2);

extern const u32 ov49_02269660[6];

void ov49_02258BEC(UnkStruct_ov70_0225CA20 *param0, int param1);

typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov63_0222D894;
void ov42_02229A8C(UnkStruct_ov63_0222D848 *param0, const UnkStruct_ov63_0222D894 *param1);
UnkStruct_ov70_0225CC54 *ov49_02258C5C(UnkStruct_ov70_0225CA20 *param0, u32 param1, u32 param2, u32 param3);
BOOL ov49_022589D8(const UnkStruct_ov70_0225C894 *param0, u32 param1, u16 *param2, u16 *param3, u32 param4);
void ov49_02258EEC(UnkStruct_ov70_0225CA20 *param0, UnkStruct_ov70_0225CC54 *param1, int param2);
typedef struct {
    s16 unk_00;
    s16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
} UnkStruct_ov65_022376D0;
UnkStruct_ov63_0222BEC0 *ov42_022280B8(UnkStruct_ov63_0222BE18 *param0, const UnkStruct_ov65_022376D0 *param1);
UnkStruct_ov66_02234798 *ov45_0223070C(UnkStruct_ov66_02234548 *param0, const UnkStruct_ov63_0222BEC0 *param1);
UnkStruct_ov70_0225CC54 *ov49_022593C0(UnkStruct_ov70_0225CA20 *param0);
UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);
BOOL ov49_02258FDC(const UnkStruct_ov70_0225CC54 *param0, u16 param1, u16 param2);
void ov42_02228100(UnkStruct_ov63_0222BEC0 *param0);
void ov45_02230884(UnkStruct_ov66_02234798 *param0);
s32 ov42_02228188(const UnkStruct_ov63_0222BEC0 *param0, int param1);
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;
void ov42_022281F8(UnkStruct_ov63_0222BEC0 *param0, int param1, s32 param2);
void ov42_0222839C(UnkStruct_ov63_0222BEC0 *param0, UnkStruct_ov63_0222CC3C param1);
void ov42_022283AC(UnkStruct_ov63_0222BEC0 *param0, UnkStruct_ov63_0222CC3C param1);
void ov49_02258DB4(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov63_0222CC3C param1);
UnkStruct_ov63_0222CC3C ov42_022282F4(const UnkStruct_ov63_0222BEC0 *param0);
void ov49_0225927C(UnkStruct_ov70_0225CA20 *param0, s32 param1, s32 param2, s32 param3);
typedef struct {
    UnkStruct_ov63_0222CC3C unk_00;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov63_0222CCB8;
void ov42_02228068(UnkStruct_ov63_0222BE18 *param0, const UnkStruct_ov63_0222CCB8 *param1);
UnkStruct_ov63_0222CC3C ov42_022282DC(const UnkStruct_ov63_0222BEC0 *param0);
void ov49_02258C1C(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov63_0222D894 *param1);
UnkStruct_ov70_0225CC54 *ov49_02258C28(UnkStruct_ov70_0225CA20 *param0, u32 param1);
UnkStruct_ov70_0225CC54 *ov49_02258C5C(UnkStruct_ov70_0225CA20 *param0, u32 param1, u32 param2, u32 param3);
UnkStruct_ov70_0225CC54 *ov49_02258CB8(UnkStruct_ov70_0225CA20 *param0, u32 param1, u32 param2);
void ov49_02258D54(UnkStruct_ov70_0225CC54 *param0);
UnkStruct_ov70_0225CC54 *ov49_02258D70(UnkStruct_ov70_0225CA20 *param0, u32 param1);
UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0);
UnkStruct_ov70_0225CC54 *ov49_02258DB0(UnkStruct_ov70_0225CA20 *param0);
void ov49_02258DB4(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov63_0222CC3C param1);
void ov49_02258E04(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov63_0222CC3C param1, int param2);
UnkStruct_ov63_0222CC3C ov49_02258E34(const UnkStruct_ov70_0225CC54 *param0);
u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1);
void ov49_02258E7C(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, int param3);
void ov49_02258EAC(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, int param3);

void ov49_02258BEC(UnkStruct_ov70_0225CA20 *param0, int param1) {
    ov49_02259A20(&param0->unk_18, param0->unk_04, ov49_02269660[param1]);
}

void ov49_02259A3C(UnkStruct_ov70_0225D93C *param0, u32 param1);

void ov49_02258C08(UnkStruct_ov70_0225CA20 *param0, int param1);

void ov49_02258C08(UnkStruct_ov70_0225CA20 *param0, int param1) {
    ov49_02259A3C(&param0->unk_18, ov49_02269660[param1]);
}

void ov49_02258C1C(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov63_0222D894 *param1) {
    ov42_02229A8C(param0->unk_10, param1);
}

UnkStruct_ov70_0225CC54 *ov49_02258C28(UnkStruct_ov70_0225CA20 *param0, u32 param1) {
    u16 v0, v1;
    BOOL v2 = ov49_022589D8(param0->unk_14, 3, &v0, &v1, 0);
    ((v2) ? (void)0 : GF_AssertFail());
    return ov49_02258C5C(param0, param1, v0, v1);
}

UnkStruct_ov70_0225CC54 *ov49_02258C5C(UnkStruct_ov70_0225CA20 *param0, u32 param1, u32 param2, u32 param3) {
    UnkStruct_ov70_0225CC54 *v0 = ov49_022593C0(param0);

    {
        UnkStruct_ov65_022376D0 v1;

        v1.unk_00 = (param2) * 16;
        v1.unk_02 = (param3) * 16;
        v1.unk_04 = param1;
        v1.unk_06 = 0;
        v1.unk_08 = 0;

        if (param0->unk_0E == 0) {
            v1.unk_0A = 0x0;
        } else {
            v1.unk_0A = 0x61;
        }

        v0->unk_00 = ov42_022280B8(param0->unk_00, &v1);
    }

    {
        v0->unk_04 = ov45_0223070C(param0->unk_04, v0->unk_00);
    }

    param0->unk_20 = v0;
    ov49_02258EEC(param0, v0, 0);

    return v0;
}

UnkStruct_ov70_0225CC54 *ov49_02258CB8(UnkStruct_ov70_0225CA20 *param0, u32 param1, u32 param2) {
    UnkStruct_ov70_0225CC54 *v0 = ov49_022593C0(param0);

    {
        UnkStruct_ov65_022376D0 v1;
        u16 v2, v3, v4;
        BOOL v5;
        BOOL v6 = 0;
        UnkStruct_ov70_0225CC54 *v7;

        v4 = 0;

        do {
            v5 = ov49_022589D8(param0->unk_14, 4 + param1, &v2, &v3, v4);

            if (v5 == 0) {
                return 0L;
            } else {
                v7 = ov49_02258DAC(param0);

                if (v7 == 0L) {
                    v6 = 1;
                } else {
                    if (ov49_02258FDC(v7, v2, v3) == 0) {
                        v6 = 1;
                    }
                }
            }

            v4++;
        } while (v6 == 0);

        v1.unk_00 = (v2) * 16;
        v1.unk_02 = (v3) * 16;
        v1.unk_04 = param1;
        v1.unk_06 = 0;
        v1.unk_08 = 1;
        v1.unk_0A = param2;
        v0->unk_00 = ov42_022280B8(param0->unk_00, &v1);
    }

    {
        v0->unk_04 = ov45_0223070C(param0->unk_04, v0->unk_00);
    }

    ov49_02258EEC(param0, v0, 0);

    return v0;
}

void ov49_02258D54(UnkStruct_ov70_0225CC54 *param0) {
    ov45_02230884(param0->unk_04);
    ov42_02228100(param0->unk_00);
    memset(param0, 0, sizeof(UnkStruct_ov70_0225CC54));
}

UnkStruct_ov70_0225CC54 *ov49_02258D70(UnkStruct_ov70_0225CA20 *param0, u32 param1) {
    int v0;
    u32 v1;

    for (v0 = 0; v0 < param0->unk_0C; v0++) {
        if (param0->unk_08[v0].unk_00) {
            v1 = ov42_02228188(param0->unk_08[v0].unk_00, 4);

            if (v1 == param1) {
                return &param0->unk_08[v0];
            }
        }
    }

    return 0L;
}

UnkStruct_ov70_0225CC54 *ov49_02258DAC(UnkStruct_ov70_0225CA20 *param0) {
    return param0->unk_20;
}

UnkStruct_ov70_0225CC54 *ov49_02258DB0(UnkStruct_ov70_0225CA20 *param0) {
    return param0->unk_24;
}

void ov49_02258DB4(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov63_0222CC3C param1) {
    ((param0->unk_00) ? (void)0 : GF_AssertFail());
    ov42_0222839C(param0->unk_00, param1);
    ov42_022283AC(param0->unk_00, param1);
    ov42_022281F8(param0->unk_00, 5, 0);
}

void ov49_02258E04(UnkStruct_ov70_0225CC54 *param0, UnkStruct_ov63_0222CC3C param1, int param2) {
    ov49_02258DB4(param0, param1);
    ov42_022281F8(param0->unk_00, 6, param2);
}

UnkStruct_ov63_0222CC3C ov49_02258E34(const UnkStruct_ov70_0225CC54 *param0) {
    ((param0->unk_00) ? (void)0 : GF_AssertFail());
    return ov42_022282F4(param0->unk_00);
}

u32 ov49_02258E60(const UnkStruct_ov70_0225CC54 *param0, int param1) {
    ((param0->unk_00) ? (void)0 : GF_AssertFail());
    return ov42_02228188(param0->unk_00, param1);
}

void ov49_02258E7C(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, int param3) {
    u32 v0;
    u32 v1;

    v0 = ov42_02228188(param1->unk_00, 5);
    ((v0 == 0) ? (void)0 : GF_AssertFail());
    v1 = ov42_02228188(param1->unk_00, 4);
    ov49_0225927C(param0, param2, param3, v1);
}

void ov49_02258EAC(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, int param2, int param3) {
    UnkStruct_ov63_0222CCB8 v0;

    v0.unk_00 = ov42_022282DC(param1->unk_00);
    v0.unk_04 = param2;
    v0.unk_06 = param3;
    v0.unk_07 = ov42_02228188(param1->unk_00, 4);

    ov42_02228068(param0->unk_00, &v0);
}
