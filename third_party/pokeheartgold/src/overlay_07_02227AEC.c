#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_bulk_up_context.h"
#include "pokepic.h"
#include "pokemon_types_def.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"

void ov07_02222590(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, s16 ref, u32 steps);

struct BattleAnimSystem;
struct Pokepic;
struct PokepicDrawParam;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
void ov07_0221C448(BattleAnimSystem *system, SysTask *task);
void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);
BOOL ov07_022222B4(XYTransformContext *ctx);
typedef struct DoubleTeamContext {
    u8 iteration;
    u8 state;
    u16 sLotoId;
    u8 blendA;
    u8 blendB;
    u16 unused2;
    s16 attackerX;
    s16 attackerY;
    BattleAnimSystem *battleAnimSys;
    Pokepic *attackerSprite;
    SpriteManager *pokemonSpriteManager;
    ManagedSprite *monSprites[4];
    XYTransformContext pos[4];
} DoubleTeamContext;
enum DoubleTeamState {
    DOUBLE_TEAM_STATE_INIT = 0,
    DOUBLE_TEAM_STATE_UPDATE_SPRITES,
    DOUBLE_TEAM_STATE_FADE_OUT_SPRITES,
};
enum DoubleTeamTableIndex {
    DOUBLE_TEAM_TABLE_BASEPOS = 0,
    DOUBLE_TEAM_TABLE_RANGE_X,
    DOUBLE_TEAM_TABLE_ITERATION_FRAMES,
    DOUBLE_TEAM_TABLE_COUNT
};
extern const s16 ov07_02236732[4][DOUBLE_TEAM_TABLE_COUNT];
void ov07_02227B58(SysTask *task, void *param);

void ov07_02227AEC(BattleAnimSystem *system) {
    BulkUpContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(BulkUpContext));

    ctx->state = 0;
    ctx->battleAnimSys = system;
    ctx->sprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(system));
    ctx->baseY = Pokepic_GetAttr(ctx->sprite, 1);
    ctx->spriteHeight = ov07_0221FAA0(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ov07_02222590(&ctx->scale, 0x64, 0x96, 0x64, 0x32, 0x64, 0xA);
    ov07_0221C410(ctx->battleAnimSys, ov07_022278B0, ctx);
}

void ov07_02227B58(SysTask *task, void *param) {
    int i;
    u8 numComplete;
    DoubleTeamContext *ctx = param;

    switch (ctx->state) {
    case DOUBLE_TEAM_STATE_INIT: {
        int posIndex = (ctx->iteration % 2) * 2;
        int frameReduction = (ctx->iteration + 1) / 2;

        ov07_02222268(
            &ctx->pos[0],
            ctx->attackerX + ov07_02236732[posIndex][DOUBLE_TEAM_TABLE_BASEPOS],
            ctx->attackerX + ov07_02236732[posIndex][DOUBLE_TEAM_TABLE_RANGE_X],
            ctx->attackerY,
            ctx->attackerY,
            ov07_02236732[posIndex][DOUBLE_TEAM_TABLE_ITERATION_FRAMES] - frameReduction);

        ov07_02222268(
            &ctx->pos[1],
            ctx->attackerX - ov07_02236732[posIndex][DOUBLE_TEAM_TABLE_BASEPOS],
            ctx->attackerX - ov07_02236732[posIndex][DOUBLE_TEAM_TABLE_RANGE_X],
            ctx->attackerY,
            ctx->attackerY,
            ov07_02236732[posIndex][DOUBLE_TEAM_TABLE_ITERATION_FRAMES] - frameReduction);

        ov07_02222268(
            &ctx->pos[2],
            ctx->attackerX + ov07_02236732[posIndex + 1][DOUBLE_TEAM_TABLE_BASEPOS],
            ctx->attackerX + ov07_02236732[posIndex + 1][DOUBLE_TEAM_TABLE_RANGE_X],
            ctx->attackerY,
            ctx->attackerY,
            ov07_02236732[posIndex + 1][DOUBLE_TEAM_TABLE_ITERATION_FRAMES] - frameReduction);

        ov07_02222268(
            &ctx->pos[3],
            ctx->attackerX - ov07_02236732[posIndex + 1][DOUBLE_TEAM_TABLE_BASEPOS],
            ctx->attackerX - ov07_02236732[posIndex + 1][DOUBLE_TEAM_TABLE_RANGE_X],
            ctx->attackerY,
            ctx->attackerY,
            ov07_02236732[posIndex + 1][DOUBLE_TEAM_TABLE_ITERATION_FRAMES] - frameReduction);
    }
        ctx->iteration++;
        ctx->state++;
    case DOUBLE_TEAM_STATE_UPDATE_SPRITES:
        numComplete = 0;

        for (i = 0; i < 4; i++) {
            if (ov07_022222B4(&ctx->pos[i]) == 0) {
                numComplete++;
            } else {
                ManagedSprite_SetPositionXY(ctx->monSprites[i], ctx->pos[i].x, ctx->pos[i].y);
            }

            ManagedSprite_TickFrame(ctx->monSprites[i]);
        }

        if (numComplete == 4) {
            if (ctx->iteration == 8 + 1) {
                ctx->state++;
            } else {
                ctx->state = DOUBLE_TEAM_STATE_INIT;
            }
        }
        break;
    case DOUBLE_TEAM_STATE_FADE_OUT_SPRITES:
        if (ctx->blendA > 0) {
            ctx->blendA--;
        }

        if (ctx->blendB < 15) {
            ctx->blendB++;
        }

        if (ctx->blendA == 0 && ctx->blendB == 15) {
            ctx->state++;
        }

        G2_ChangeBlendAlpha(ctx->blendA, ctx->blendB);
        break;
    default:
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    SpriteSystem_DrawSprites(ctx->pokemonSpriteManager);
}
