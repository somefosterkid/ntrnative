#include "global.h"
#include "error_handling.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sys_task.h"
#include "overlay_07_value_lerp_context.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

s32 ov07_0222207C(void *, void *, s16, s16);

s32 ov07_02222384(void *arg0, void *arg1);

s32 ov07_022223CC(void *arg0, void *arg1, void *arg2);

s32 ov07_02222040(s32, s32, s32);
void ov07_022223F0(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void ov07_02222418(void *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ov07_02222440(void *arg0);
s32 ov07_02222468(void *arg0);

enum ValueLerpParam {
    VALUE_PARAM_STEPS = 0,
    VALUE_PARAM_STEP_SIZE,

    VALUE_PARAM_FX32_STEPS = 0,
    VALUE_PARAM_FX32_STEP_SIZE,
    VALUE_PARAM_FX32_CUR_ANGLE,

    VALUE_PARAM_COS_STEPS = 0,
    VALUE_PARAM_COS_CUR_ANGLE,
    VALUE_PARAM_COS_AMPLITUDE,
    VALUE_PARAM_COS_STEP_SIZE,
};
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
void ov07_02222494(ValueLerpContext *ctx, u16 start, u16 end, fx32 amplitude, u32 steps);
BOOL ov07_022224AC(ValueLerpContext *ctx);

s32 ov07_022223CC(void *arg0, void *arg1, void *arg2) {
    if (ov07_02222384(arg0, arg1) != 0) {
        ov07_0222207C(arg0, arg2, 0, 0);
        return 1;
    }
    return 0;
}

void ov07_022223F0(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    *(s32 *)arg0 = arg1;
    *(s32 *)((u8 *)arg0 + 4) = arg3;
    *((s32 *)&((ValueLerpContext *)arg0)->data[1]) = (s32)(ov07_02222040(arg1 << 0xC, arg2 << 0xC, arg3) >> 0xC);
}

void ov07_02222418(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 v0;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    *(s32 *)arg0 = arg1;
    v0 = arg1 << 0xC;
    *(s32 *)((u8 *)arg0 + 4) = arg3;
    *(s32 *)((u8 *)arg0 + 8) = ov07_02222040(v0, arg2 << 0xC, arg3);
    *(s32 *)((u8 *)arg0 + 0xC) = v0;
}

s32 ov07_02222440(void *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (*(s32 *)((u8 *)arg0 + 4) != 0) {
        *(s32 *)arg0 = (s32)(*(s32 *)arg0 + *((s32 *)&((ValueLerpContext *)arg0)->data[1]));
        *(s32 *)((u8 *)arg0 + 4) = (s32)(*(s32 *)((u8 *)arg0 + 4) - 1);
        return 1;
    }
    return 0;
}

s32 ov07_02222468(void *arg0) {
    s32 v0;

    if (arg0 == NULL) {
        GF_AssertFail();
    }
    if (*(s32 *)((u8 *)arg0 + 4) != 0) {
        v0 = *((s32 *)&((ValueLerpContext *)arg0)->data[2]) + *((s32 *)&((ValueLerpContext *)arg0)->data[1]);
        *((s32 *)&((ValueLerpContext *)arg0)->data[2]) = v0;
        *(s32 *)arg0 = (s32)(v0 >> 0xC);
        *(s32 *)((u8 *)arg0 + 4) = (s32)(*(s32 *)((u8 *)arg0 + 4) - 1);
        return 1;
    }
    return 0;
}

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

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

void ov07_02222494(ValueLerpContext *ctx, u16 start, u16 end, fx32 amplitude, u32 steps) {
    ctx->data[VALUE_PARAM_COS_STEPS] = steps;
    ctx->data[VALUE_PARAM_COS_CUR_ANGLE] = start;
    ctx->data[VALUE_PARAM_COS_AMPLITUDE] = amplitude;
    ctx->data[VALUE_PARAM_COS_STEP_SIZE] = (end - start) / steps;
}

BOOL ov07_022224AC(ValueLerpContext *ctx) {
    ((ctx) ? (void)0 : GF_AssertFail());

    if (ctx->data[VALUE_PARAM_COS_STEPS]) {
        ctx->data[VALUE_PARAM_COS_CUR_ANGLE] += ctx->data[VALUE_PARAM_COS_STEP_SIZE];
        ctx->data[VALUE_PARAM_COS_CUR_ANGLE] &= 0xFFFF;
        ctx->data[VALUE_PARAM_COS_STEPS]--;
        ctx->value = FX_MulInline(FX_CosIdx(ctx->data[VALUE_PARAM_COS_CUR_ANGLE]), ctx->data[VALUE_PARAM_COS_AMPLITUDE]) >> 12;

        return 1;
    }

    return 0;
}
