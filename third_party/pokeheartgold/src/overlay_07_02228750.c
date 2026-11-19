#include "global.h"

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_minimize_context.h"

#include "heap.h"
#include "obj_pltt_transfer.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_harden_context.h"

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
enum BattleAnimAnchorType {
    BATTLE_ANIM_ANCHOR_BOTTOM = 0,
    BATTLE_ANIM_ANCHOR_TOP,
};
void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);
BOOL ov07_0222260C(XYTransformContext *ctx);
void ov07_02222644(XYTransformContext *ctx, f32 *px, f32 *py);
void ov07_022226FC(ManagedSprite *sprite, s16 y, s16 height, fx32 scale, enum BattleAnimAnchorType anchor);
enum MiminizeSpriteState {
    MINIMIZE_SPRITE_STATE_INIT = 0,
    MINIMIZE_SPRITE_STATE_SHRINK,
};
enum MinimizeState {
    MINIMIZE_STATE_INIT = 0,
    MINIMIZE_STATE_SHRINK,
    MINIMIZE_STATE_SETUP_GROW,
    MINIMIZE_STATE_GROW,
};
extern const u8 ov07_022366C8[4];
void ov07_02228834(SysTask *task, void *param);

void ov07_02228750(BattleAnimSystem *system);

void ov07_02228750(BattleAnimSystem *system) {
    PaletteData *paletteData;
    enum NarcId narcId;
    PaletteData *data;
    s32 memberNo;
    u32 v1;
    HardenContext *ctx;

    ctx = ov07_022324D8(system, sizeof(HardenContext));
    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(system);
    ctx->attackerSprite1 = ov07_0221C4E8(ctx->battleAnimSys, 0);
    ctx->attackerSprite2 = ov07_0221C4E8(ctx->battleAnimSys, 1);
    reg_GX_DISPCNT = (s32)((0x8000) | (reg_GX_DISPCNT & 0xFFFF1FFF));
    reg_G2_WINOUT = (u16)(0x1F | (reg_G2_WINOUT & ~0x3F));
    reg_G2_WINOUT = (u16)(0x1200 | (reg_G2_WINOUT & 0xFFFFC0FF));
    memberNo = ov07_0221FA80(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    narcId = ov07_0221FA90(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    v1 = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(ctx->attackerSprite1->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    data = ov07_0221FA78(ctx->battleAnimSys);
    paletteData = data;
    PaletteData_LoadNarc_CustomTint(data, narcId, memberNo, ov07_0221BFD0(system), PLTTBUF_MAIN_OBJ, 0x20U, (u16)(u16)(v1 * 0x10), 0x100, 0x100, 0x100);
    ManagedSprite_SetDrawFlag(ctx->attackerSprite1, 0);
    ManagedSprite_SetDrawFlag(ctx->attackerSprite2, 0);
    ov07_0221C410(ctx->battleAnimSys, ov07_02228684, ctx);
}

void ov07_02228834(SysTask *task, void *param) {
    int i;
    int done;
    MinimizeContext *ctx = param;

    switch (ctx->state) {
    case MINIMIZE_STATE_INIT:
        Pokepic_SetAttr(ctx->attackerSprite, POKEPIC_VISIBLE, 1);
        ctx->state++;
        break;
    case MINIMIZE_STATE_SHRINK:
        done = 0;

        for (i = 0; i < 3; i++) {
            switch (ctx->sprites[i].state) {
            case MINIMIZE_SPRITE_STATE_INIT:
                if (++ctx->sprites[i].frameCounter >= ov07_022366C8[i]) {
                    ov07_02222590(
                        &ctx->sprites[i].scale,
                        100,
                        5,
                        100,
                        5,
                        100,
                        5);
                    ctx->sprites[i].state++;
                }
                break;
            case MINIMIZE_SPRITE_STATE_SHRINK:
                if (ov07_0222260C(&ctx->sprites[i].scale) == 0) {
                    ctx->sprites[i].state++;
                } else {
                    f32 scaleX, scaleY;
                    ov07_02222644(&ctx->sprites[i].scale, &scaleX, &scaleY);

                    ManagedSprite_SetAffineScale(ctx->sprites[i].sprite, scaleX * ctx->spriteDir, scaleY);
                    ov07_022226FC(
                        ctx->sprites[i].sprite,
                        ctx->attackerY,
                        ctx->attackerSpriteHeight,
                        ctx->sprites[i].scale.data[XY_PARAM_CUR_Y],
                        BATTLE_ANIM_ANCHOR_BOTTOM);
                }
                break;
            default:
                done++;
                break;
            }

            ManagedSprite_TickFrame(ctx->sprites[i].sprite);
        }

        if (done >= 3) {
            ctx->cycles++;

            if (ctx->cycles < 3) {
                for (i = 0; i < 3; i++) {
                    s16 x, y;
                    ManagedSprite_GetPositionXY(ctx->sprites[i].sprite, &x, &y);
                    ManagedSprite_SetPositionXY(ctx->sprites[i].sprite, x, ctx->attackerY);
                    ManagedSprite_SetAffineScale(ctx->sprites[i].sprite, 1.0f * ctx->spriteDir, 1.0f);

                    ctx->sprites[i].state = MINIMIZE_SPRITE_STATE_INIT;
                    ctx->sprites[i].frameCounter = 0;
                }

                ctx->state--;
            } else {
                ctx->state++;
            }
        }
        break;
    case MINIMIZE_STATE_SETUP_GROW:
        for (i = 0; i < 3; i++) {
            ManagedSprite_SetOamMode(ctx->sprites[i].sprite, GX_OAM_MODE_NORMAL);
            ov07_02222590(
                &ctx->sprites[i].scale,
                5,
                100,
                5,
                100,
                100,
                5);
        }

        ctx->state++;
        break;
    case MINIMIZE_STATE_GROW:
        done = 0;

        for (i = 0; i < 3; i++) {
            if (ov07_0222260C(&ctx->sprites[i].scale) == 0) {
                done++;
            } else {
                f32 scaleX, scaleY;
                ov07_02222644(&ctx->sprites[i].scale, &scaleX, &scaleY);
                ManagedSprite_SetAffineScale(ctx->sprites[i].sprite, scaleX * ctx->spriteDir, scaleY);

                ov07_022226FC(
                    ctx->sprites[i].sprite,
                    ctx->attackerY,
                    ctx->attackerSpriteHeight,
                    ctx->sprites[i].scale.data[XY_PARAM_CUR_Y],
                    BATTLE_ANIM_ANCHOR_BOTTOM);
            }
        }

        if (done >= 3) {
            ctx->state++;
        }
        break;
    default:
        Pokepic_SetAttr(ctx->attackerSprite, POKEPIC_VISIBLE, 0);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->pokemonSpriteManager);
}
