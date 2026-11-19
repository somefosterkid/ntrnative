#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_stat_change_context.h"

struct BattleAnimSystem;

struct BgConfig;

struct SysTask;

struct SysTaskQueue;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


void ov07_0221C448(BattleAnimSystem *system, SysTask *task);

void ov07_02232508(void *ptr);

enum StatChangeState {
    STAT_CHANGE_STATE_LOAD_BG = 0,
    STAT_CHANGE_STATE_SET_BLENDING,
    STAT_CHANGE_STATE_SCROLL,
    STAT_CHANGE_STATE_FADE_OUT
};

void ov07_022315C0(SysTask *task, void *param);

void ov07_0223172C(BattleAnimSystem *system, StatChangeConfig *config);

BOOL ov07_0221BFC0(BattleAnimSystem *system);
SysTask *ov07_0221C410(BattleAnimSystem *system, SysTaskFunc func, void *param);
u16 ov07_0221C468(BattleAnimSystem *system);
u16 ov07_0221C470(BattleAnimSystem *system);
ManagedSprite *ov07_0221C4E8(BattleAnimSystem *system, int index);
int ov07_0221FB04(BattleAnimSystem *system, int bg);
u8 ov07_02231924(BattleAnimSystem *system, int battler);
void ov07_02231FE4(BattleAnimSystem *system, BattleAnimScriptFuncCommon *common);
void ov07_0223174C(BattleAnimSystem *system, StatChangeConfig *config);

void ov07_022315C0(SysTask *task, void *param) {
    StatChangeContext *ctx = param;

    switch (ctx->common.state) {
    case STAT_CHANGE_STATE_LOAD_BG:
        ov07_02231524(ctx, GF_BG_LYR_MAIN_2);
        ctx->common.state++;
        break;
    case STAT_CHANGE_STATE_SET_BLENDING:
        G2_SetBlendAlpha(
            GX_BLEND_PLANEMASK_BG2,
            GX_BLEND_PLANEMASK_BD | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG3,
            ctx->config.overlayAlpha,
            ctx->config.otherAlpha);
        ctx->common.state++;
        break;
    case STAT_CHANGE_STATE_SCROLL:
        if (ctx->timer > 20) {
            ctx->common.state++;
        } else {
            ctx->timer++;
        }
        break;
    case STAT_CHANGE_STATE_FADE_OUT:
        if (ctx->config.overlayAlpha > 0) {
            ctx->config.overlayAlpha--;
        }

        if (ctx->config.otherAlpha < 16) {
            ctx->config.otherAlpha++;
        }

        if (ctx->config.overlayAlpha == 0 && ctx->config.otherAlpha == 16) {
            BgClearTilemapBufferAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2);
            ManagedSprite_SetDrawFlag(ctx->attackerSprite, 0);
            ManagedSprite_SetDrawFlag(ctx->attackerSprite2, 0);
            ManagedSprite_SetDrawFlag(ctx->attackerPartnerSprite, 0);
            ctx->common.state++;
        }

        G2_ChangeBlendAlpha(ctx->config.overlayAlpha, ctx->config.otherAlpha);
        break;
    default:
        G2_SetWndOutsidePlane((GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3) | GX_WND_PLANEMASK_OBJ, 0);
        G2_SetWndOBJInsidePlane((GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3) | GX_WND_PLANEMASK_OBJ, 0);
        BgSetPosTextAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
        GX_SetVisibleWnd(GX_WNDMASK_NONE);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        ov07_02232508(ctx);
        ctx = 0L;
        return;
    }

    ctx->config.offsetX += ctx->config.stepX;
    ctx->config.offsetY += ctx->config.stepY;

    BgSetPosTextAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, ctx->config.offsetX);
    BgSetPosTextAndCommit(ctx->common.bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, ctx->config.offsetY);

    ManagedSprite_TickFrame(ctx->attackerSprite);
    ManagedSprite_TickFrame(ctx->attackerSprite2);
    ManagedSprite_TickFrame(ctx->attackerPartnerSprite);
    SpriteSystem_DrawSprites(ctx->common.pokemonSpriteManager);
}

void ov07_0223172C(BattleAnimSystem *system, StatChangeConfig *config) {
    config->narcID = NARC_a_0_0_7;
    config->memberTableIndex = ov07_0221C4A8(system, 0);
    config->mode = ov07_0221C4A8(system, 1);
}

void ov07_0223174C(BattleAnimSystem *system, StatChangeConfig *config) {
    StatChangeContext *ctx = ov07_022324D8(system, sizeof(StatChangeContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->config = *config;
    ctx->attackerSprite = ov07_0221C4E8(ctx->common.battleAnimSys, 0);
    ctx->attackerSprite2 = ov07_0221C4E8(ctx->common.battleAnimSys, 1);
    ctx->attackerPartnerSprite = ov07_0221C4E8(ctx->common.battleAnimSys, 2);

    int bgPriority = ov07_0221FB04(ctx->common.battleAnimSys, 2);
    bgPriority = 2;

    ManagedSprite_SetPriority(ctx->attackerSprite, bgPriority);
    ManagedSprite_SetPriority(ctx->attackerSprite2, bgPriority);

    int battler;
    if (ctx->config.mode == 0) {
        battler = ov07_0221C468(system);
    } else {
        battler = ov07_0221C470(system);
    }

    int battlerType = ov07_02231924(system, battler);
    if (ov07_0221BFC0(system) == 1) {
        ManagedSprite_SetDrawFlag(ctx->attackerPartnerSprite, 0);
        bgPriority = ov07_0221FB04(ctx->common.battleAnimSys, 2);
        ManagedSprite_SetPriority(ctx->attackerSprite, bgPriority);
        ManagedSprite_SetPriority(ctx->attackerSprite2, bgPriority);
    } else {
        if (battlerType == 0 || battlerType == 1) {
            ManagedSprite_SetDrawFlag(ctx->attackerPartnerSprite, 0);
        } else if (battlerType == 4 || battlerType == 3) {
            if (ctx->config.mode == 0) {
                ManagedSprite_SetPriority(ctx->attackerPartnerSprite, bgPriority + 1);
            } else {
                ManagedSprite_SetPriority(ctx->attackerPartnerSprite, bgPriority + 1);
            }
        } else {
            if (ctx->config.mode == 0) {
                ManagedSprite_SetPriority(ctx->attackerPartnerSprite, bgPriority - 1);
            } else {
                ManagedSprite_SetPriority(ctx->attackerPartnerSprite, bgPriority - 1);
            }
        }
    }

    G2_SetWndOBJInsidePlane(GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_OBJ | GX_WND_PLANEMASK_BG2, 1);
    G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 1);
    GX_SetVisibleWnd(GX_WNDMASK_OW);

    ManagedSprite_SetOamMode(ctx->attackerSprite2, GX_OAM_MODE_OBJWND);

    ctx->config.offsetX = 0;
    ctx->config.offsetY = 0;

    ov07_0221C410(ctx->common.battleAnimSys, ov07_022315C0, ctx);
}
