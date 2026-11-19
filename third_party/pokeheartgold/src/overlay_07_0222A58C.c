#include "global.h"

#include "constants/heap.h"

#include "overlay_07_battle_anim_funcs.h"
#include "struct_defs/resdat_narc_structs.h"

#include "heap.h"
#include "math_util.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "bg_window.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "gf_gfx_planes.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "pokepic.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_bg_scroll_context.h"

struct BattleAnimSystem;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

ManagedSpriteTemplate ov07_0221F9E8(BattleAnimSystem *system);

void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

typedef struct AssistPawSprite {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    u8 state;
    u8 step;
    u8 delay;
    u8 destIndex;
    ManagedSprite *sprite;
    XYTransformContext pos;
    f32 scale;
    int *outerState;
} AssistPawSprite;

typedef struct AssistSpriteContext {
    u8 state;
    u8 unused0;
    u8 pawCount;
    u8 sLotoId;
    u8 unused2;
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    AssistPawSprite paws[20];
    int pawStates[20];
} AssistSpriteContext;

enum AssistSpriteState {
    ASSIST_SPRITE_STATE_WAIT_FOR_SCALE = 0,
    ASSIST_SPRITE_STATE_BEGIN_MOVE,
    ASSIST_SPRITE_STATE_WAIT_FOR_DONE,
    ASSIST_SPRITE_STATE_CLEANUP,
};

void ov07_0222A328(SysTask *task, void *param);

void ov07_0222A4B4(SysTask *task, void *param);

extern const s16 ov07_022367CE[6][4];

void ov07_0222A58C(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

struct BgScrollContext;
struct Pokepic;
struct PokepicDrawParam;
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
void ov07_0221EC7C(BattleAnimSystem *system, enum GFBgLayer bgLayer);
int ov07_0221FAF8(BattleAnimSystem *system, int bg);
BgScrollContext *ov07_02222CCC(u8 startY, u8 endY, u16 angleIncrement, fx32 amplitude, s16 speed, u32 bgID, u32 priority, u32 initValue, enum HeapID heapID);
void ov07_02222D3C(BgScrollContext *ctx);
u32 ov07_02222D88(s32 x, s32 y);
typedef struct CamouflageContext {
    int timer;
    BattleAnimScriptFuncCommon common;
    ManagedSprite *maskSprite;
    ManagedSprite *xluSprite;
    BgScrollContext *bgScroll;
    AlphaFadeContext alpha;
} CamouflageContext;
enum CamouflageState {
    CAMOUFLAGE_STATE_INIT = 0,
    CAMOUFLAGE_STATE_FADE_OUT,
    CAMOUFLAGE_STATE_SCROLL,
    CAMOUFLAGE_STATE_FADE_IN,
};
void ov07_0222A710(SysTask *task, void *param);
void ov07_0221EB98(BattleAnimSystem *system, enum GFBgLayer bgLayer);
void ov07_0222A838(BattleAnimSystem *system);

void ov07_0222A8D8(ManagedSprite *sprite);

void ov07_0222A58C(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    ManagedSpriteTemplate template;

    AssistSpriteContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(AssistSpriteContext));
    ((ctx != 0L) ? (void)0 : GF_AssertFail());

    ctx->unused0 = 0;
    ctx->state = ASSIST_SPRITE_STATE_WAIT_FOR_SCALE;
    ctx->spriteSys = spriteSys;
    ctx->spriteMan = spriteMan;
    ctx->battleAnimSys = system;

    template = ov07_0221F9E8(system);
    ov07_02231E08(ctx->battleAnimSys, -1, -1);

    ctx->pawCount = ov07_0221C4A8(ctx->battleAnimSys, 0);
    ctx->pawCount = 12;

    ctx->paws[0].sprite = sprite;
    int posOffsetIdx = 0;

    for (int i = 0; i < ctx->pawCount; i++) {
        if (i != 0) {
            ctx->paws[i].sprite = SpriteSystem_NewSprite(ctx->spriteSys, ctx->spriteMan, &template);
        }

        ManagedSprite *pawSprite = ctx->paws[i].sprite;

        ctx->paws[i].state = 0;
        ctx->paws[i].step = 0;
        ctx->paws[i].delay = 10 + (LCRandom() % 10) + i;
        ctx->paws[i].destIndex = LCRandom() % 6;
        ctx->paws[i].scale = 1.0f;
        ctx->paws[i].battleAnimSys = ctx->battleAnimSys;
        ctx->paws[i].spriteSys = ctx->spriteSys;
        ctx->paws[i].spriteMan = ctx->spriteMan;
        ctx->paws[i].outerState = &ctx->pawStates[i];

        s16 x = ov07_022367CE[posOffsetIdx][1] + (LCRandom() % ov07_022367CE[posOffsetIdx][3]);
        s16 y = ov07_022367CE[posOffsetIdx][0] + (LCRandom() % ov07_022367CE[posOffsetIdx][2]);

        ManagedSprite_SetPositionXY(pawSprite, x, y);
        ManagedSprite_SetAffineOverwriteMode(pawSprite, AFFINE_OVERWRITE_MODE_DOUBLE);
        ManagedSprite_SetAffineScale(pawSprite, ctx->paws[i].scale, ctx->paws[i].scale);
        ManagedSprite_SetDrawFlag(pawSprite, 0);

        ov07_0221C3F4(system, ov07_0222A328, &ctx->paws[i], 1100 - 1);

        posOffsetIdx++;
        posOffsetIdx %= 6;
    }

    ov07_0221C3F4(system, ov07_0222A4B4, ctx, 1100 + 1);
}

void ov07_0222A710(SysTask *task, void *param) {
    CamouflageContext *ctx = param;

    switch (ctx->common.state) {
    case CAMOUFLAGE_STATE_INIT: {
        Pokepic *attackerSprite = ov07_0221FA48(
            ctx->common.battleAnimSys,
            ov07_0221C468(ctx->common.battleAnimSys));
        Pokepic_SetAttr(attackerSprite, MON_SPRITE_HIDE, 1);
        ctx->common.state++;
        break;
    }
    case CAMOUFLAGE_STATE_FADE_OUT:
        if (ov07_02222AF4(&ctx->alpha) == 0) {
            break;
        }

        ctx->timer = 0;
        ctx->bgScroll = ov07_02222CCC(
            0,
            160,
            (((2) * 0xFFFF) / 360),
            ((fx32)(((12) > 0) ? ((12) * (1 << 12) + 0.5f) : ((12) * (1 << 12) - 0.5f))),
            200,
            ov07_0221FAF8(ctx->common.battleAnimSys, 1),
            0,
            ov07_02222D88(0, 0),
            ov07_0221BFD0(ctx->common.battleAnimSys));
        ctx->common.state++;
        break;
    case CAMOUFLAGE_STATE_SCROLL:
        ctx->timer++;
        if (ctx->timer < 120) {
            break;
        }

        ov07_02222D3C(ctx->bgScroll);
        ov07_02222AC4(
            &ctx->alpha,
            2,
            16,
            16,
            2,
            16);
        ctx->common.state++;
        break;
    case CAMOUFLAGE_STATE_FADE_IN: {
        if (ov07_02222AF4(&ctx->alpha) == 0) {
            break;
        }

        Pokepic *attackerSprite = ov07_0221FA48(
            ctx->common.battleAnimSys,
            ov07_0221C468(ctx->common.battleAnimSys));
        Pokepic_SetAttr(attackerSprite, MON_SPRITE_HIDE, 0);
        ctx->common.state++;
        break;
    }
    default:
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        ov07_0221EC7C(ctx->common.battleAnimSys, GF_BG_LYR_MAIN_2);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    ManagedSprite_TickFrame(ctx->xluSprite);
    ManagedSprite_TickFrame(ctx->maskSprite);
    SpriteSystem_DrawSprites(ctx->common.pokemonSpriteManager);
}

void ov07_0222A838(BattleAnimSystem *system) {
    CamouflageContext *ctx = ov07_022324D8(system, sizeof(CamouflageContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->maskSprite = ov07_0221C4E8(ctx->common.battleAnimSys, 0);
    ManagedSprite_SetOamMode(ctx->maskSprite, GX_OAM_MODE_OBJWND);

    GX_SetVisibleWnd(GX_WNDMASK_OW);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 0);
    G2_SetWndOBJInsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_OBJ, 0);

    ctx->xluSprite = ov07_0221C4E8(ctx->common.battleAnimSys, 1);
    ManagedSprite_SetOamMode(ctx->xluSprite, GX_OAM_MODE_XLU);

    ov07_02222AC4(
        &ctx->alpha,
        16,
        2,
        2,
        16,
        16);

    ov07_0221EB98(ctx->common.battleAnimSys, GF_BG_LYR_MAIN_2);

    ToggleBgLayer(GF_BG_LYR_MAIN_2, 1);
    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222A710, ctx);
}

void ov07_0222A8D8(ManagedSprite *sprite) {
    if (ManagedSprite_GetDrawFlag(sprite) == 1) {
        ManagedSprite_TickFrame(sprite);
    }
}
