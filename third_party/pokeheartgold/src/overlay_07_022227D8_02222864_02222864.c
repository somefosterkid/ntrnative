#include "global.h"

#include "gf_gfx_loader.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_afterimage_context.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);

static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);

s32 ov07_022220FC(void *, u16, u16, u16, u16, s32, s32, s32);
void ov07_02222A44(void *arg0, s32 arg1, s32 arg2);
s32 ov07_022222B4(void *);
void ov07_02222A7C(SysTask *arg0, void *arg1);

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

void ov07_02222864(AfterimageContext *ctx, XYTransformContext *transformCtx, XYTransformFunc transformFunc, s16 x, s16 y, u16 delay, u8 count, u8 mode, ManagedSprite *sprite0, ManagedSprite *sprite1, ManagedSprite *sprite2, ManagedSprite *sprite3);

void ov07_02222864(AfterimageContext *ctx, XYTransformContext *transformCtx, XYTransformFunc transformFunc, s16 x, s16 y, u16 delay, u8 count, u8 mode, ManagedSprite *sprite0, ManagedSprite *sprite1, ManagedSprite *sprite2, ManagedSprite *sprite3) {
    int i;

    ctx->x = x;
    ctx->y = y;
    ctx->transformFunc = transformFunc;
    ctx->delay = delay;
    ctx->delayCounter = 0;
    ctx->count = count;
    ctx->activeCount = 0;
    ctx->mode = mode;

    for (i = 0; i < count; i++) {
        ctx->transforms[i] = *transformCtx;
    }

    ctx->sprites[0] = sprite0;
    ctx->sprites[1] = sprite1;
    ctx->sprites[2] = sprite2;
    ctx->sprites[3] = sprite3;

    for (i = 0; i < ctx->count; i++) {
        ManagedSprite_SetDrawFlag(ctx->sprites[i], FALSE);
    }
}

void ov07_02222644(XYTransformContext *ctx, f32 *px, f32 *py);

BOOL ov07_02222914(AfterimageContext *ctx);

BOOL ov07_02222914(AfterimageContext *ctx) {
    int i;
    BOOL active[4];

    for (i = 0; i < 4; i++) {
        active[i] = 1;
    }

    if (ctx->delay <= ctx->delayCounter) {
        ManagedSprite_SetDrawFlag(ctx->sprites[ctx->activeCount], 1);
        ctx->activeCount++;
        ctx->delayCounter = 0;
    }

    if (ctx->activeCount < ctx->count) {
        ctx->delayCounter++;
    }

    for (i = 0; i < ctx->activeCount; i++) {
        active[i] = ctx->transformFunc(&ctx->transforms[i]);

        if (active[i]) {
            if (ctx->mode == 0) {
                ManagedSprite_SetPositionXY(ctx->sprites[i], ctx->x + ctx->transforms[i].x, ctx->y + ctx->transforms[i].y);
            } else {
                f32 x, y;
                ov07_02222644(&ctx->transforms[i], &x, &y);
                ManagedSprite_SetAffineScale(ctx->sprites[i], x, y);
            }
        } else {
            ManagedSprite_SetDrawFlag(ctx->sprites[i], 0);
        }
    }

    for (i = 0; i < ctx->count; i++) {
        if (active[i] == 1) {
            return 1;
        }
    }

    return 0;
}

void ov07_02222A44(void *arg0, s32 arg1, s32 arg2) {
    ov07_022220FC(arg0, 0, 0xFFFF, 0, 0xFFFF, 0x20000, 0xFFFF8000, arg2);
    *(s32 *)((u8 *)arg0 + 4) = (s32)(*(s32 *)((u8 *)arg0 + 4) * arg1);
}

void ov07_02222A7C(SysTask *task, void *arg1) {
    if (ov07_022222B4(arg1) == 0) {
        *((s32 *)&((AlphaFadeContext *)arg1)->done) = 1;
        SysTask_Destroy(task);
        return;
    }
    if ((s32) * (s16 *)arg1 < 0) {
        *(s16 *)arg1 = 0;
    }
    if ((s32) ((AlphaFadeContext *)arg1)->lerp.y < 0) {
        ((AlphaFadeContext *)arg1)->lerp.y = 0;
    }
    reg_G2_BLDALPHA = *(s16 *)arg1 | (((AlphaFadeContext *)arg1)->lerp.y << 8);
}
