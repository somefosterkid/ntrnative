#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_odor_sleuth_context.h"

struct BattleAnimSystem;

struct Pokepic;

struct PokepicDrawParam;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void ov07_022227A8(XYTransformContext *ctx, s16 extentX, s16 extentY, u32 interval, u32 amount);

BOOL ov07_022227D8(XYTransformContext *ctx);

void ov07_0222915C(SysTask *task, void *param);

void ov07_0222915C(SysTask *task, void *param) {
    OdorSleuthContext *ctx = param;

    switch (ctx->common.state) {
    case 0:
        ov07_022227A8(
            &ctx->shake[0],
            20,
            0,
            0,
            10);
        ov07_022227A8(
            &ctx->shake[1],
            2,
            0,
            0,
            10);
        ctx->common.state++;
        break;
    case 1:
        if (ov07_022227D8(&ctx->shake[0]) == 0) {
            ctx->common.state++;
        } else {
            ManagedSprite_SetPositionXY(
                ctx->monSprites[0],
                +ctx->spritePositions[1].x + ctx->shake[0].x,
                ctx->spritePositions[1].y + ctx->shake[0].y);
            ManagedSprite_SetPositionXY(
                ctx->monSprites[1],
                -ctx->spritePositions[1].x + ctx->shake[0].x,
                ctx->spritePositions[1].y + ctx->shake[0].y);

            ManagedSprite_TickFrame(ctx->monSprites[0]);
            ManagedSprite_TickFrame(ctx->monSprites[1]);
        }

        SpriteSystem_DrawSprites(ctx->common.pokemonSpriteManager);
        break;
    default:
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        break;
    }
}
