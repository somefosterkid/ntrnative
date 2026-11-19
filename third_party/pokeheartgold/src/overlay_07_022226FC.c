#include "global.h"

#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "constants/heap.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

enum BattleAnimAnchorType {
    BATTLE_ANIM_ANCHOR_BOTTOM = 0,
    BATTLE_ANIM_ANCHOR_TOP,
};

s16 ov07_02222674(s16 baseY, int height, fx32 scaleY);

void ov07_022226FC(ManagedSprite *sprite, s16 y, s16 height, fx32 scale, enum BattleAnimAnchorType anchor);

s32 ov07_0222209C(void *, ManagedSprite *);
s32 ov07_02222558();
s32 ov07_02222748(void *arg0, void *arg1);
s32 ov07_022220DC(void *, void *);
s32 ov07_02222768(void *arg0, void *arg1);
s32 ov07_0222260C();
s32 ov07_02222788(void *arg0, void *arg1);

enum XYTransformParam {
    XY_PARAM_STEPS = 0,
    XY_PARAM_STEP_SIZE,
    XY_PARAM_STEP_SIZE_X = 1,
    XY_PARAM_STEP_SIZE_Y,
    XY_PARAM_CUR_X,
    XY_PARAM_CUR_Y,

    XY_PARAM_SHAKE_COUNTER = 0,
    XY_PARAM_SHAKE_INTERVAL,
    XY_PARAM_SHAKE_REMAINING,
    XY_PARAM_SHAKE_CYCLE,
    XY_PARAM_SHAKE_EXTENT_X,
    XY_PARAM_SHAKE_EXTENT_Y,
    XY_PARAM_SHAKE_PREV_X,
    XY_PARAM_SHAKE_PREV_Y,

    XY_PARAM_REV_STEPS = 0,
    XY_PARAM_REV_CUR_X,
    XY_PARAM_REV_RADIUS_X,
    XY_PARAM_REV_CUR_Y,
    XY_PARAM_REV_RADIUS_Y,
    XY_PARAM_REV_STEP_SIZE_X,
    XY_PARAM_REV_STEP_SIZE_Y,
};
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

void ov07_022226FC(ManagedSprite *sprite, s16 y, s16 height, fx32 scale, enum BattleAnimAnchorType anchor) {
    s16 offset = ov07_02222674(y, height, scale);

    if (anchor == BATTLE_ANIM_ANCHOR_TOP) {
        offset *= -1;
        y -= height;
    }

    s16 curX, curY;
    ManagedSprite_GetPositionXY(sprite, &curX, &curY);
    ManagedSprite_SetPositionXY(sprite, curX, y + offset);
}

s32 ov07_02222748(void *arg0, void *arg1) {
    if (ov07_02222558() != 0) {
        ov07_0222209C(arg0, arg1);
        return 1;
    }
    return 0;
}

s32 ov07_02222768(void *arg0, void *arg1) {
    if (ov07_02222558() != 0) {
        ov07_022220DC(arg0, arg1);
        return 1;
    }
    return 0;
}

s32 ov07_02222788(void *arg0, void *arg1) {
    if (ov07_0222260C() != 0) {
        ov07_022220DC(arg0, arg1);
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

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount) {
    ((ctx) ? (void)0 : GF_AssertFail());

    ctx->data[XY_PARAM_SHAKE_COUNTER] = interval;
    ctx->data[XY_PARAM_SHAKE_INTERVAL] = interval;
    ctx->data[XY_PARAM_SHAKE_REMAINING] = amount;
    ctx->data[XY_PARAM_SHAKE_CYCLE] = 0;
    ctx->data[XY_PARAM_SHAKE_EXTENT_X] = extentX;
    ctx->data[XY_PARAM_SHAKE_EXTENT_Y] = extentY;
    ctx->data[XY_PARAM_SHAKE_PREV_X] = -extentX;
    ctx->data[XY_PARAM_SHAKE_PREV_Y] = -extentY;
    ctx->x = 0;
    ctx->y = 0;
}
