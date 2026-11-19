#include "global.h"
#include "gf_gfx_planes.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "overlay_07_battle_anim_funcs.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_afterimage_context.h"
#include "overlay_07_quick_attack_context.h"

struct BattleAnimSystem;

struct ManagedSprite;

struct Pokepic;

struct PokepicDrawParam;

struct SpriteManager;

struct SysTask;

struct SysTaskQueue;

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

void Heap_Free(void *ptr);

typedef struct SketchContext {
    int viewHeight;
    int bottom;
    int left;
    int windowY2;
    int viewWidth;
    int origin;
    int defenderAlpha;
    Pokepic *defenderSprite;
    BattleAnimScriptFuncCommon common;
    BattleAnimSpriteInfo unk_3C;
} SketchContext;

enum SketchState {
    SKETCH_STATE_SET_PARTIAL_DRAW = 0,
    SKETCH_STATE_INCREASE_Y,
    SKETCH_STATE_INCREASE_Y_CHECK,
    SKETCH_STATE_DISABLE_PARTIAL_DRAW,
};

void ov07_0222BADC(Pokepic *sprite, int xOffset, int width, int yOffsetFromBottom, int height);

static void ov07_0222BAF4(SysTask *task, void *param);

static void ov07_0222BAF4(SysTask *task, void *param) {
    SketchContext *ctx = param;

    switch (ctx->common.state) {
    case SKETCH_STATE_SET_PARTIAL_DRAW:
        ctx->viewWidth += (10 * 8) - 1;
        if (ctx->viewWidth >= (10 * 8)) {
            ctx->common.state++;
            ctx->origin ^= 1;
            ctx->viewWidth = 0;
        } else {

            if (ctx->origin == 0) {
                ov07_0222BADC(
                    ctx->defenderSprite,
                    0,
                    ctx->viewWidth,
                    (ctx->viewHeight + 2) * 2,
                    2);
            } else {
                ov07_0222BADC(
                    ctx->defenderSprite,
                    (10 * 8) - ctx->viewWidth,
                    ctx->viewWidth,
                    (ctx->viewHeight + 2) * 2,
                    2);
            }
        }
        break;
    case SKETCH_STATE_INCREASE_Y:
        ctx->bottom -= 2 / 2;
        ctx->windowY2 += 2 / 2;
        G2_SetWnd0Position(ctx->left, ctx->bottom, ctx->left + (10 * 8), ctx->windowY2);
        ctx->common.state++;
        break;
    case SKETCH_STATE_INCREASE_Y_CHECK:
        ctx->bottom -= 2 / 2;
        ctx->windowY2 += 2 / 2;
        G2_SetWnd0Position(ctx->left, ctx->bottom, ctx->left + (10 * 8), ctx->windowY2);
        ctx->viewHeight++;

        if (ctx->viewHeight > (((10 * 8) / 2) - 2)) {
            ctx->common.state++;
        } else {
            ctx->common.state = SKETCH_STATE_SET_PARTIAL_DRAW;
        }
        break;
    case SKETCH_STATE_DISABLE_PARTIAL_DRAW:
        Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_PARTIAL_DRAW, 0);
        ctx->common.state++;
        break;
    default:
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_NONE, 0);
        G2_SetWndOutsidePlane(GX_WND_PLANEMASK_NONE, 0);
        G2_SetWnd0Position(0, 0, 0, 0);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_ALPHA, ctx->defenderAlpha);
        Heap_Free(ctx);
        break;
    }
}

void SetBgPriority(u8 bgLayer, u8 priority);

int ov07_0221FAE8(BattleAnimSystem *system);

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void ov07_0222BC7C(BattleAnimSystem *system);

void ov07_0222BC7C(BattleAnimSystem *system) {
    SketchContext *ctx = ov07_022324D8(system, sizeof(SketchContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->defenderSprite = ov07_0221FA48(system, ov07_0221C470(system));
    ctx->viewHeight = 0;

    int defenderType = ov07_02231924(system, ov07_0221C470(system));
    switch (defenderType) {
    case 3:
    case 4:
        SetBgPriority(GF_BG_LYR_MAIN_2, ov07_0221FAE8(system) - 1);
        break;
    }

    s16 defenderX = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_X_CENTER);
    s16 defenderY = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_Y_CENTER);
    defenderY -= Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_SHADOW_HEIGHT);

    ctx->defenderAlpha = Pokepic_GetAttr(ctx->defenderSprite, MON_SPRITE_ALPHA);
    Pokepic_SetAttr(ctx->defenderSprite, MON_SPRITE_ALPHA, 8);

    ctx->left = defenderX - ((10 * 8) / 2);
    ctx->bottom = defenderY + ((10 * 8) / 2);
    ctx->viewWidth = 0;
    ctx->windowY2 = ctx->bottom;
    ctx->origin = 0;

    GX_SetVisibleWnd(GX_WNDMASK_W0);
    G2_SetWnd0InsidePlane((GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3) | GX_WND_PLANEMASK_OBJ, 1);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 1);
    G2_SetWnd0Position(ctx->left, ctx->bottom, ctx->left + (10 * 8), ctx->windowY2);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222BAF4, ctx);
}

void SpriteSystem_DrawSprites(SpriteManager *spriteMan);

BOOL ov07_02222240(XYTransformContext *ctx, s32 cx, s32 cy, Pokepic *sprite);

BOOL ov07_02222914(AfterimageContext *ctx);

static void ov07_0222BD90(SysTask *task, void *param);

static void ov07_0222BD90(SysTask *task, void *param) {
    QuickAttackContext *ctx = param;

    BOOL transformsActive[3];
    transformsActive[0] = ov07_02222240(&ctx->rev, ctx->attackerX, ctx->attackerY, ctx->attackerSprite);
    transformsActive[1] = ov07_02222914(&ctx->afterImageCtx);
    transformsActive[2] = 0;

    SpriteSystem_DrawSprites(ctx->pokemonSpriteManager);

    for (int i = 0; i < ((int)(sizeof(transformsActive) / sizeof(*(transformsActive)))); i++) {
        if (transformsActive[i] == 1) {
            return;
        }
    }

    Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER, ctx->attackerX);
    Pokepic_SetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER, ctx->attackerY + (((fx32)(((-8) > 0) ? ((-8) * (1 << 12) + 0.5f) : ((-8) * (1 << 12) - 0.5f))) >> 12));
    ov07_0221C448(ctx->battleAnimSys, task);
    Heap_Free(ctx);
}

void ManagedSprite_SetPriority(ManagedSprite *managedSprite, int priority);

void ManagedSprite_SetOamMode(ManagedSprite *managedSprite, GXOamMode mode);

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

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

BOOL ov07_02222180(XYTransformContext *ctx);

void ov07_02222864(AfterimageContext *ctx, XYTransformContext *transformCtx, XYTransformFunc transformFunc, s16 x, s16 y, u16 delay, u8 count, u8 mode, ManagedSprite *sprite0, ManagedSprite *sprite1, ManagedSprite *sprite2, ManagedSprite *sprite3);

void ov07_02222A44(XYTransformContext *ctx, int revs, int stepsPerRev);

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

void ov07_0222BE00(BattleAnimSystem *system);

void ov07_0222BE00(BattleAnimSystem *system) {
    QuickAttackContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(QuickAttackContext));

    ctx->battleAnimSys = system;
    ctx->pokemonSpriteManager = ov07_0221C514(ctx->battleAnimSys);
    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->attackerX = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER);
    ctx->attackerY = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER);
    ctx->attackerShadowHeight = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_SHADOW_HEIGHT);
    ctx->attackerY -= ((fx32)(((-8) > 0) ? ((-8) * (1 << 12) + 0.5f) : ((-8) * (1 << 12) - 0.5f))) >> 12;

    ov07_02222A44(&ctx->rev, 1, 8);

    XYTransformContext afterImageRev;
    ov07_02222A44(&afterImageRev, 1, 10);

    int dir = ov07_02222004(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->rev.data[XY_PARAM_REV_RADIUS_X] *= dir;
    afterImageRev.data[XY_PARAM_REV_RADIUS_X] *= dir;

    for (int i = 0; i < 2; i++) {
        ctx->afterimageSprites[i] = ov07_0221C4E8(ctx->battleAnimSys, i);
        ManagedSprite_SetDrawPriority(ctx->afterimageSprites[i], 100);
        ManagedSprite_SetPriority(ctx->afterimageSprites[i], 1);
        ManagedSprite_SetOamMode(ctx->afterimageSprites[i], GX_OAM_MODE_XLU);
    }

    ov07_02222864(
        &ctx->afterImageCtx,
        &afterImageRev,
        ov07_02222180,
        ctx->attackerX,
        ctx->attackerY - ctx->attackerShadowHeight,
        2,
        2,
        0,
        ctx->afterimageSprites[0],
        ctx->afterimageSprites[1],
        0L,
        0L);

    ov07_02231E08(ctx->battleAnimSys, -1, -1);
    ov07_0221C410(ctx->battleAnimSys, ov07_0222BD90, ctx);
}
