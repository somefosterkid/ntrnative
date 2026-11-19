#include "global.h"

#include "constants/heap.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sys_task_api.h"

typedef struct XYTransformContext {
    s16 x;
    s16 y;
    s32 data[8];
} XYTransformContext;

typedef struct AlphaFadeContext {
    XYTransformContext lerp;
    int done;
} AlphaFadeContext;

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

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
int ov07_02222AF4(const AlphaFadeContext *ctx);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void ov07_02222A7C(SysTask *task, void *param);

void ov07_02222AC4(AlphaFadeContext *ctx, s16 startEv1, s16 endEv1, s16 startEv2, s16 endEv2, int steps);

void ov07_02222AC4(AlphaFadeContext *ctx, s16 startEv1, s16 endEv1, s16 startEv2, s16 endEv2, int steps) {
    ov07_02222268(&ctx->lerp, startEv1, endEv1, startEv2, endEv2, steps);

    ctx->done = FALSE;
    SysTask_CreateOnMainQueue(ov07_02222A7C, ctx, 0);
}

int ov07_02222AF4(const AlphaFadeContext *ctx) {
    return ctx->done;
}
