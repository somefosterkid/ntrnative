#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "overlay_07_battle_anim_funcs.h"
#include "pokepic.h"
#include "overlay_07_acid_armor_context.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_bg_scroll_context.h"

struct BattleAnimSystem;

struct BgScrollContext;

struct Pokepic;

struct PokepicDrawParam;

struct SysTask;

struct SysTaskQueue;

void SetBgPriority(u8 bgLayer, u8 priority);

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

u32 SysTask_GetPriority(SysTask *task);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

int ov07_0221FAE8(BattleAnimSystem *system);

enum GFBgLayer ov07_0221FAEC(BattleAnimSystem *system, int bg);

int ov07_0221FAF8(BattleAnimSystem *system, int bg);

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

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

BgScrollContext *ov07_02222CCC(u8 startY, u8 endY, u16 angleIncrement, fx32 amplitude, s16 speed, u32 bgID, u32 priority, u32 initValue, enum HeapID heapID);

u32 ov07_02222D88(s32 x, s32 y);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void ov07_0222C918(SysTask *task, void *param);

void ov07_0222CA8C(BattleAnimSystem *system);

void ov07_0222CA8C(BattleAnimSystem *system) {
    AcidArmorContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(AcidArmorContext));
    memset(ctx, 0, sizeof(AcidArmorContext));

    ctx->battleAnimSys = system;

    SysTask *task = ov07_0221C410(ctx->battleAnimSys, ov07_0222C918, ctx);

    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->attackerX = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER);
    ctx->attackerY = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER);
    ctx->attackerY -= Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_SHADOW_HEIGHT);

    ctx->attackerX -= (10 * 8) / 2;
    ctx->attackerY -= (10 * 8) / 2;

    Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_HIDE, 1);

    int dir = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));

    ctx->scrollStartY = ctx->attackerY - 8;
    ctx->scrollEndY = ctx->attackerY + (10 * 8) + 8;

    if (ctx->scrollStartY < 0) {
        ctx->scrollStartY = 0;
    }

    if (ctx->scrollEndY > 191) {
        ctx->scrollEndY = 191;
    }

    ctx->bgScroll = ov07_02222CCC(
        ctx->scrollStartY,
        ctx->scrollEndY,
        (((5) * 0xFFFF) / 360),
        ((fx32)(((8) > 0) ? ((8) * (1 << 12) + 0.5f) : ((8) * (1 << 12) - 0.5f))) * dir,
        80,
        ov07_0221FAF8(system, 1),
        SysTask_GetPriority(task) + 1,
        ov07_02222D88((u16)ctx->attackerX, (u16)ctx->attackerY),
        ov07_0221BFD0(system));

    ov07_02222268(&ctx->pos, 0, 80, 0, 40, 24);
    ctx->pos.data[XY_PARAM_STEP_SIZE_X] *= dir;

    int battlerType = ov07_02231924(ctx->battleAnimSys, ov07_0221C468(system));
    if (battlerType == 3 || battlerType == 4) {
        SetBgPriority(
            ov07_0221FAEC(ctx->battleAnimSys, 1),
            ov07_0221FAE8(ctx->battleAnimSys));
        SetBgPriority(GF_BG_LYR_MAIN_0, ov07_0221FAE8(ctx->battleAnimSys) + 1);
    }

    ov07_0222C918(task, ctx);
}
