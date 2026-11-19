#include "global.h"
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

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225CC54_t UnkStruct_ov70_0225CC54;

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

UnkStruct_ov63_0222CC3C ov42_02228270(UnkStruct_ov63_0222CC3C param0, int param1);

UnkStruct_ov63_0222CC3C ov42_022282DC(const UnkStruct_ov63_0222BEC0 *param0);

const UnkStruct_ov63_0222BEC0 *ov42_022284A4(const UnkStruct_ov63_0222BE18 *param0, UnkStruct_ov63_0222CC3C param1);

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;

BOOL ov49_022589AC(const UnkStruct_ov70_0225C894 *param0, u16 param1, u16 param2);

u32 ov49_022589C4(const UnkStruct_ov70_0225C894 *param0, u16 param1, u16 param2);

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

BOOL ov49_0225904C(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, u32 *param2, UnkStruct_ov63_0222CC3C *param3);

BOOL ov49_0225904C(UnkStruct_ov70_0225CA20 *param0, const UnkStruct_ov70_0225CC54 *param1, u32 *param2, UnkStruct_ov63_0222CC3C *param3) {
    static const u8 v0[4] = {
        3,
        2,
        1,
        0,
    };
    int v1;
    UnkStruct_ov63_0222CC3C v2;
    UnkStruct_ov63_0222CC3C v3;
    BOOL v4;
    u32 v5;
    const UnkStruct_ov63_0222BEC0 *v6;

    v2 = ov42_022282DC(param1->unk_00);

    for (v1 = 0; v1 < 4; v1++) {
        v3 = ov42_02228270(v2, v0[v1]);
        v4 = ov49_022589AC(param0->unk_14, (v3.unk_00) / 16, (v3.unk_02) / 16);

        if (v4 == 1) {
            continue;
        }

        v5 = ov49_022589C4(param0->unk_14, (v3.unk_00) / 16, (v3.unk_02) / 16);

        if ((v5 != 0) && (v5 != 42)) {
            continue;
        }

        v6 = ov42_022284A4(param0->unk_00, v3);

        if (v6 != NULL) {
            continue;
        }

        *param2 = v0[v1];
        *param3 = v3;

        return 1;
    }

    return 0;
}
